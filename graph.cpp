#include "graph.h"
#include <iostream>
#include <sstream>
#include <algorithm>

/*---------------------------------------------------------*/
void GRAPH::read(char * filename){
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while(std::getline(file, line)) {
        std::istringstream iss(line);

        char ch;
        int partitionId, calcTimes;
        std::vector<Edge> edges;

        // 读取 partitionId 和 calcTimes
        iss >> ch >> partitionId >> ch >> calcTimes >> ch;

        // 读取每个子字符串 "[x,y]"
        std::string token;
        while (std::getline(iss, line, '[')){
            if(line.empty())    continue;
            std::istringstream edgeIss(line);
            int targetPartitionId, sendMessageTimes;
            char comma;
            edgeIss >> targetPartitionId >> comma >> sendMessageTimes;
            edges.push_back({targetPartitionId, sendMessageTimes});
        }
        
        // 将 partition 添加到 graph.partitions 中
        this->partitions.push_back({partitionId, calcTimes, edges});
    }

    this->V = this->partitions.size();
    this->par2node = std::vector<int>(this->V);
    //初始化partitionId到nodeId的映射
    for(int i=0; i<V; ++i) {
        par2node[i] = Partition::getInitNodeId(i);
    }

    //统计失效分区个数
    this->V2 = 0;
    for(auto &P : this->partitions) {
        if(Partition::isFailedPartition(P.Id)){
            ++this->V2;
        }
    }

    //输出读取的图结构
    // for(const auto& partition : partitions) {
    //     std::cout<<partition.Id<<" "<<partition.calcTimes<<"   ";

    //     for(const auto& edge: partition.edges){
    //         std::cout<<edge.targetPartitionId<<" "<<edge.sendMessageTimes<<"  ";
    //     }
    //     std::cout<<std::endl;
    // }
}

float swap_rate = 0.5;      //50%概率交换，50%概率移动
void GRAPH::perturb(){
    int p, n;
    //只选取失效分区进行交换/移动
    do{
        n = rand() % V;
    }while(!Partition::isFailedPartition(n));
    
    if(swap_rate > rand_01()){
        do{
            p = rand() % V;
        }while(n==p || !Partition::isFailedPartition(p));

        std::swap(par2node[n], par2node[p]);
    }
    else{
        do{
          p = rand() % nodeNumber;  
        } while (par2node[n] == p);

        par2node[n] = p;
    }
}

void GRAPH::get_solution(Solution &sol){
    sol.par2node = this->par2node;
    sol.cost = this->getCost();
}

void GRAPH::keep_sol(){
    get_solution(last_sol);
}

void GRAPH::keep_best(){
    get_solution(best_sol);
}

void GRAPH::recover(){
    recover(last_sol);
}

void GRAPH::recover(Solution &sol){
    this->par2node = sol.par2node;
}

void GRAPH::recover_best(){
    recover(best_sol);
}

double GRAPH::getCost(){
    double totalCost = 0;
    int totalMesTimes = 0, totalCalcTimes = 0;
    std::vector<int> perNodeCalcTimes(nodeNumber);  //各个node上的总计算代价
    //遍历所有分区
    for(const auto & partition : this->partitions) {
        //如果该partition是失效partition，将其计算成本计入代价中
        if(Partition::isFailedPartition(partition.Id)){
            perNodeCalcTimes[par2node[partition.Id]] += partition.calcTimes;
        }

        //若src和dest在同一node上，通信次数为0。否则：
        //若src为未失效分区，dest为未失效分区，通信次数为0
        //若src为未失效分区，dest为失效分区，通信次数为边权值
        //若src为失效分区，dest为未失效分区，通信次数为1
        //若src为失效分区，dest为失效分区，通信次数为边权值

        bool isSrcFailed = Partition::isFailedPartition(partition.Id);
        bool isDestFailed;
        for(const auto& edge: partition.edges) {
            //如果两个分区在同一个node上，通信次数为0
            //TODO()    如果partitionId和partitions数组的下标不对应，这里需要修改！！
            if(par2node[partition.Id] == par2node[partitions[edge.targetPartitionId].Id])
                continue;
            
            isDestFailed = Partition::isFailedPartition(edge.targetPartitionId);
            
            if(isDestFailed){
                totalMesTimes += edge.sendMessageTimes;
            }
            else if(isSrcFailed){
                totalMesTimes += 1;
            }

        }
    }

    //获取各个node计算次数的最大值，作为总计算次数
    totalCalcTimes = *std::max_element(perNodeCalcTimes.begin(), perNodeCalcTimes.end());

    totalCost += gamma*(perMesCost*totalMesTimes + perCalcCost*totalCalcTimes);

    std::vector<double> perNodeWeights(nodeNumber);    //各个node上的失效分区权重
    for(const auto & P : this->partitions) {
        //如果该分区是失效分区，将其所在node的权重加1
        if(Partition::isFailedPartition(P.Id)) {
            perNodeWeights[par2node[P.Id]] += 1;
        }
    }

    double Tib = 0;
    for(double Wi : perNodeWeights) {
        Tib += (Wi - 1.0*V2/nodeNumber) * (Wi - 1.0*V2/nodeNumber);
    }

    totalCost += (1-gamma) * Tib / V2;

    return totalCost / normCost;
}

int GRAPH::size(){
    return V;
}

void GRAPH::printSol() {
    printf("Failed PartitionId     nodeID\n");
    for(int i=0; i<V; ++i){
        if(Partition::isFailedPartition(i)){
            printf("\t%d\t\t%d\n", i, par2node[i]);
        }
    }
    printf("Cost = %lf", getCost()*normCost);
}

void GRAPH::normalizeCost(int t) {
    //getCost() return总代价除以normCost的值
    //设置normCost为1，则返回值等于总代价
    normCost = 1.0;

    double tempNormCost = 0;
    for(int i=0; i < t; ++i){
        perturb();
        tempNormCost += getCost();
    }
    normCost = tempNormCost / t;
    printf("normalize cost=%.0f\n", normCost);
}

/*-------------------------------------------------*/

float rand_01(){
    return float(rand()%10000)/10000;
}

//TODO()
double seconds(){
    return 0;
}
#include "graph.h"

/*---------------------------------------------------------*/
void GRAPH::read(char *){

}

void GRAPH::packing(){
    
}

void GRAPH::perturb(){

}

void GRAPH::get_solution(Solution &sol){
    sol.reassign = reassign;
    sol.cost = getCost();
}

void GRAPH::keep_sol(){
    get_solution(last_sol);
}

void GRAPH::keep_best(){
    get_solution(best_sol);
}

void GRAPH::recover(Solution &sol){
    reassign = sol.reassign;
}

void GRAPH::recover_best(){
    recover(best_sol);
}

//时间复杂度O(V^2)
double GRAPH::getCost(){
    int mesTimes = 0, calcTimes = 0;
    //计算总通信次数
    for(int src=0; src<V; ++src){
        for(int dest=0; dest<V; ++dest){
            if(reassign[src] == reassign[dest])     continue;
            //当两个partition不在同一个node上时，
            //这两个partition间的通信纳入通信成本
            mesTimes += adjMat[src][dest];
        }
    }
    //计算最大计算次数
    std::vector<int> calcTimesArr;
    for(int parID=0; parID<V; ++parID){
        int nodeID = reassign[parID];
        while(nodeID >= calcTimesArr.size()){
            calcTimesArr.push_back(0);
        }
        calcTimesArr[nodeID] += value[parID];
        calcTimes = value[parID]>calcTimes ? value[parID] : calcTimes;
    }

    return mesTimes*perMesCost + calcTimes*perCalcCost;
}

int GRAPH::size(){
    return V;
}

/*-------------------------------------------------*/

float rand_01(){
    return float(rand()%10000)/10000;
}

double seconds(){
    

}
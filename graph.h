#ifndef graphH
#define graphH

/*-----------------------------------------------------------*/
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cstdio>
/*-----------------------------------------------------------*/

const int nodeNumber = 3;       //后面要改成通过命令行/文件输入的形式
const int failedNodeId = 2;

struct Edge{
    int targetPartitionId;//目标分区ID
    int sendMessageTimes ;//消息发送次数
};

struct Partition{
    int Id;//分区Id
    int calcTimes;      //分区的计算次数
    std::vector<Edge> edges;

    /**
     * 通过partitionId获取初始nodeId
     * 静态成员函数，方便在没有实例化对象之前调用
    */
    static int getInitNodeId(int partitionId) {
        return partitionId % nodeNumber;
    }

    /**
     * 判断partition是否是失效partition
    */
    static bool isFailedPartition(int partitionId) {
        return partitionId % nodeNumber == failedNodeId;
    }
};

class GRAPH {
    public:
    int V;                              //顶点个数
    std::vector<Partition>  partitions; //顶点（分区）列表

    double perMesCost, perCalcCost; //单次通信成本和单次计算成本
    double normCost;   //归一化总成本

    std::vector<int>    par2node;   //partitionId到nodeId的映射

    struct Solution{
        std::vector<int>    par2node;
        double cost;
    };

    Solution best_sol, last_sol;    //保存的最优解和最新解

    public:
    GRAPH(double c1 = 1.0, double c2 = 1.0){
        perCalcCost = c1, perMesCost = c2;
    }

    /**
     * 读取文件
    */
    void read(char *);

    /**
     * 计算代价之前的预处理，应该可以不用
    */
    void packing();

    void perturb();

    /**
     * 将当前解保存到sol中
    */
    void get_solution(Solution &sol);

    /**
     * 将当前解进行保存
    */
    void keep_sol();

    /**
     * 将当前解作为最优解进行保存
    */
    void keep_best();

    /**
     * 将当前解恢复到保存的最新解
    */
   void recover();

    /**
     * 将当前解恢复为sol
    */
    void recover(Solution &sol);

    /**
     * 将当前解恢复到保存的最优解
    */
    void recover_best();

    /**
     * 返回当前解的代价
    */
    double getCost();

    /**
     * 返回顶点个数
    */
    int size();

    /**
     * 输出当前保存的解
    */
    void printSol();

    /**
     * 归一化cost
    */
    void normalizeCost(int t) ;

};

/**
 * 返回0~1的一个随机数
*/
float rand_01();

/**
 * 返回当前时间
*/
double seconds();


#endif
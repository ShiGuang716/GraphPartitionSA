#ifndef graphH
#define graphH

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cstdio>

class GRAPH {
    //先列出大概要用到的变量，后面具体实现时可能会改
    //先不考虑什么访问权限了，全部public
    public:
    /*————图结构所需变量————*/
    int V;                          //顶点个数
    std::vector<int>    value;      //顶点的计算成本
    std::vector<std::vector<int>>   adjMat;     //邻接矩阵  也可以用邻接表
    /*————图结构所需变量————*/

    std::vector<int>    reassign;   //重分区映射

    struct Solution{
        std::vector<int>    reassign;
        int cost;
    };

    Solution best_sol, last_sol;    //保存的最优解和最新解

    public:
    GRAPH(){};
    /**
     * 读取文件
    */
    void read(char *){}

    /**
     * 计算代价之前的预处理，应该可以不用
    */
    void packing(){}

    void perturb(){}
    /**
     * 将当前解进行保存
    */
    void keep_sol(){}

    /**
     * 将当前解作为最优解进行保存
    */
    void keep_best(){}

    /**
     * 恢复到保存的最后一个解
    */
    void recover(){}

    /**
     * 恢复到保存的最优解
    */
    void recover_best(){}

    /**
     * 返回当前解的代价
    */
    double getCost(){}

    /**
     * 返回顶点个数
    */
    int size(){}
};

/**
 * 返回0~1的一个随机数
*/
float rand_01() {

}

/**
 * 返回当前时间
*/
double seconds(){

}


#endif
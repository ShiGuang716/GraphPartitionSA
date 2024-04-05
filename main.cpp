
#include <iostream>
#include <cstring>
#include "graph.h"
#include "sa_graph.h"

int main(){
    char filename[80], outfile[80]="";
    int times=400, local=7;
    float init_temp=0.9, term_temp=0.1;
    float alpha=1;  //TODO  这个参数应该可以不用

    //TODO() 输入的提示以及命令行参数读取

    try{
        double time = seconds();
        GRAPH graph;      //TODO  可能是graph(alpha)
        graph.read(filename);

        //graph.init();   //TODO    应该不需要初始化

        double last_time = SA_Partition(graph, times, local, term_temp);

        //graph.list_information(); //TODO  暂时不考虑输出

        //TODO()    输出运行结果  

    }catch(...){}

    return 1;
}
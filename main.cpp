
#include <iostream>
#include <cstring>
#include "graph.h"
#include "sa_graph.h"

int main(){
    char filename[80], outfile[80]="";
    int times=40, local=7;
    float term_temp=0.1;

    //TODO() 输入的提示以及命令行参数读取
    std::cout<<"Please enter filename:"<<std::endl;
    std::cin>>filename;

    try{
        double time = seconds();
        GRAPH graph(1.0, 1.0, 0.5);
        graph.read(filename);
        graph.normalizeCost(10);
        //graph.init();   //TODO    做个归一化

        double last_time = SA_Partition(graph, times, local, term_temp);

        //graph.list_information(); //TODO  暂时不考虑输出

        //TODO()    输出结果  
        graph.printSol();

    }catch(...){}

    return 1;
}

#include <cmath>
#include <cassert>
#include <iostream>
#include "sa_graph.h"

float init_avg = 0.00001;
float avg_ratio=150;
float lamda=1.3;

/**
 * 求平均
*/
double mean(std::vector<double> &chain){
  double sum=0;
  for(int i=0; i < chain.size();i++)
     sum+= chain[i];
  return sum/chain.size();
}

/**
 * 求方差
*/
double std_var(std::vector<double> &chain){
  double m = mean(chain);
  double sum=0;
  double var;
  int N= chain.size();

  for(int i=0; i < N;i++)
     sum += (chain[i]-m)*(chain[i]-m);

  var = sqrt(sum/(N-1));
  printf("  m=%.4f ,v=%.4f\n",m,var);

  return var;
}

double SA_Partition(GRAPH &graph, int k, int local=0, float term_T=0.1){
    int MT, uphill, reject;
    double pre_cost, best, cost;
    float d_cost, reject_rate;

    int N = k * graph.size();
    float P=0.9;
    float T, actual_T=1;
    double avg=init_avg;
    float conv_rate = 1;
    double time=seconds();

    double estimate_avg = 0.08 / avg_ratio;
    std::cout << "Estimate Average Delta Cost = " << estimate_avg << std::endl;

    if(local==0)    avg = estimate_avg;

    T = avg / log(P);

    //获得初始解
    graph.packing();
    graph.keep_sol();
    graph.keep_best();
    pre_cost = best = graph.getCost();

    int good_num=0, bad_num=0;
    double total_cost=0;
    int count=0;
    std::ofstream of("/tmp/graph_debug");

    do{
        count++;
        total_cost = 0;
        MT=uphill=reject=0;
        printf("Iteration %d, T=%.2f\n", count, actual_T);

        std::vector<double> chain;  //还是要用到chain，用于温度变化

        for(; uphill < N && MT < 2*N; MT++){
            graph.perturb();
            graph.packing();
            cost = graph.getCost();
            d_cost = cost - pre_cost;
            float p = exp(d_cost/T);

            chain.push_back(cost);

            if(d_cost <=0 || rand_01() < p){
                graph.keep_sol();
                pre_cost = cost;

                if(d_cost > 0){
                    uphill++, bad_num++;
                    of << d_cost << ": " << p << std::endl;
                }
                else if(d_cost < 0){
                    good_num++;
                }

                if(cost < best) {
                    graph.keep_best();
                    best = cost;
                    //TODO  在这里输出解
                    time = seconds();
                }
            }
            else{
                reject++;
                graph.recover();
            }
        }

        double sv = std_var(chain);
        float r_t = exp(lamda*T/sv);
        T = r_t*T;

        if(count == local) {
            T = estimate_avg/log(P);
            T *= pow(0.9, local);
            actual_T = exp(estimate_avg/T);
        }
        if(count > local) {
            actual_T = exp(estimate_avg/T);
            conv_rate = 0.95;    
        }

        reject_rate = float(reject)/MT;
        printf("  T= %.2f, r= %.2f, reject= %.2f\n\n", actual_T, r_t, reject_rate);
        
    }while(reject_rate < conv_rate && actual_T > term_T);

    if(reject_rate >= conv_rate)
    std::cout << "\n  Convergent!\n";
    else if(actual_T <= term_T)
    std::cout << "\n Cooling Enough!\n";

    printf("\n good = %d, bad=%d\n\n", good_num, bad_num);

    graph.recover_best();
    graph.packing();

    return time;
}
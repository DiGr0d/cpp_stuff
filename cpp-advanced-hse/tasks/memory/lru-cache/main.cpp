#include <iostream>
#include <string>
#include "lru_cache.h"
#include <vector> 
#include <random>
#include <chrono>

std::ostream& operator<<(std::ostream& os, LruCache& cache){
    os<<"======================\n";
    for(const auto& p: cache.data_)
    {
        std::cout<< "key " << p.first <<  " value "<< p.second->value<<std::endl;
    }
    os<<"----------Now List --------------\n";
    Node* p = cache.lru_.Front();
    while(p != &cache.lru_.dummy){
        os<<p->key << " -> ";
        p = p->next;
    }
    os<<std::endl;
    os<<"========================\n";

    return os;
}

int GetMinUsageKey(std::vector<int>& vec)
{
    int min = 100000;int n = vec.size();int ans = 0;
    for(int i = 0; i<n; i++){
        if(vec[i] < min && vec[i] > 0)
        {
            min = vec[i]; ans = i;
        }
    }
    return ans;
}

int main(){
   int maxsz; std::cin>>maxsz;
   std::vector<int> usage(21, 0);
    std::random_device rd;
    std::mt19937 rnd(rd());
    std::uniform_int_distribution<int> dist(1, 2);
    std::uniform_int_distribution<int> keys(1, 20);
    LruCache cache(maxsz);
    for(int i = 0; i > -1; i++)
    {
        int val = dist(rnd);
        std::cout<<"op "<< val << "\n";std::cout.flush();
        std::string input;
        std::getline(std::cin, input);
        std::getline(std::cin, input);
        if(input.empty()){break;}
        switch(val)
        {
            case 1:
            {
                int vl = keys(rd); usage[vl]++;
                cache.Set(std::to_string(vl), "iteraions" + std::to_string(i));
            }
            break;
            default:
            {
                std::string s;
                int val1 = keys(rd); usage[val1]++;
                int p = cache.Get(std::to_string(val1),&s);
                if(p){
                    std::cout<< "Got  key = " <<val1<<  " value = "<<s<<std::endl;
                }
                else{
                    std::cout<<"Got nothing"<<std::endl;
                }
            }
        }
        std::cout<<cache<<std::endl;
    }
}
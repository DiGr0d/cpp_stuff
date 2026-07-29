#include <iostream>
#include "cow_vector.h"

const std::string kBank[10] = {"first",
"second", "third", "fourth", "sixth", "fifth", "seventh", "eight", "ninght", "tens"};

int main(){
    COWVector vec1;
    {
        COWVector vec;
        vec.PushBack(kBank[1]);
        //std::cout<<vec.Size()<<std::endl;
        for(size_t i = 0; i < 5; i++){
            vec.PushBack(kBank[i%10]);
        }
        for(size_t i = 0; i < vec.Size(); i++){
            std::cout<<vec.Get(i)<<std::endl;
        }
        vec1 = vec;
    }
    std::cout<<"=========================\n";
    for(size_t i = 0; i < vec1.Size(); i++){
        std::cout<<vec1.Get(i)<<std::endl;
    }
}
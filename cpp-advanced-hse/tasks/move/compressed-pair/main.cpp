#include <iostream>
#include "compressed_pair.h"
struct EmptyBase{
};

int main(){
   CompressedPair<int, EmptyBase> pair;
   std::cout<<sizeof(pair);
}

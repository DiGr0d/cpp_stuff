#include <iostream>
#include <type_traits>
#include "harakiri.h"

int main(){
    OneTimeCallback* obj = new AwesomeCallback("sasha govnoed");
    std::cout<<std::move(*obj)();
}
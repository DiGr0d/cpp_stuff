#include <bits/atomic_wait.h>
#include <iostream>
#include <fstream>
#include "string_operations.h"
#include <numeric>

std::string StrCatWrapper(const std::string& a, std::string_view b, const std::string_view& c) {
    return StrCat(std::string_view(a), b, c);  // вызывает твой шаблон
}

int main(){
    const char* t = "abadfd";
    std::string s = "   pon  ";
    std::string_view k = "  nepon ";
   auto tt = StrCat(" ", -3, 4 , 123243, t, s, k, 324322, -123223, "aaffja aals aa");
    std::cout<<tt<<std::endl;
}
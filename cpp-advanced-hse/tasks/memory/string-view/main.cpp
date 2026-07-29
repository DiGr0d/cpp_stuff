#include <iostream>
#include <string>
#include "string_view.h"

int main()
{
    std::string s = "abacaba";
    StringView view("abacaba");
    for(size_t i = 0; i < view.Size(); i++)
    {
        std::cout<<view[i];
    }
    StringView vs(s, 3, 3);
    for(size_t i = 0; i < vs.Size(); i++)
    {
        std::cout<<vs[i];
    }
}
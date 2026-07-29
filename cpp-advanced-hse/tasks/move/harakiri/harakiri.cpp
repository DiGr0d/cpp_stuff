#include "harakiri.h"

AwesomeCallback::AwesomeCallback(const std::string& s):OneTimeCallback(), str_(s){}

std::string AwesomeCallback::operator()()const &&{
    auto ans = str_ + "awesomeness";
    delete this;
    return ans;
}
// Your code goes here

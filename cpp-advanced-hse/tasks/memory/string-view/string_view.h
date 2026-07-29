#pragma once
#include <cstring>
#include <string>
#include <string>

class StringView {
    public:
    StringView(const std::string& str, size_t begin = 0, size_t len = std::string::npos);
    StringView(const char* str);
    StringView(const char* str, size_t len);

    size_t Size() const{return size_;}

    const char& operator[](size_t pos) const;
    ~StringView() = default;
    private:
    const char* ptr_;
    size_t size_;
};

StringView::StringView(const std::string& str, size_t begin, size_t len):ptr_(&str[begin]){
    size_t sz = str.size();
    if(len == std::string::npos || begin + len >= sz)
    {
        size_ = str.size() - begin;
    }
    else {
        size_ = len;
    }
}

StringView::StringView(const char* str):ptr_(str), size_(strlen(str)){}

StringView::StringView(const char* str, size_t len):ptr_(str), size_(len){}

const char& StringView::operator[](size_t pos) const{
    return ptr_[pos];
}

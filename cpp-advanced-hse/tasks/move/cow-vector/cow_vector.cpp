#include "cow_vector.h"
#include <string>

COWVector::COWVector():info_(new State{1, std::vector<std::string>()}){
}
COWVector::~COWVector(){
    if(--info_->ref_count == 0)
    {
        delete info_;
    }
}
COWVector::COWVector(const COWVector& other){
    info_ = other.info_;
    ++info_->ref_count;
}
COWVector& COWVector::operator=(const COWVector& other){
    if(this != &other){
        if(--info_->ref_count == 0){
            delete info_;
        }
        info_ = other.info_;
        ++info_->ref_count;
    }
    return *this;
}
size_t COWVector::Size() const{
    return info_ ->data.size();
}

void COWVector::DeepCopy(){
    if(info_->ref_count == 1) { return;}
    --info_->ref_count;
    info_ = new State{1, std::vector<std::string>(info_->data)};
}

void COWVector::Resize(size_t size){
    DeepCopy();
    info_->data.resize(size);
}

const std::string& COWVector::Get(size_t at){
    return info_->data.at(at);
}

const std::string& COWVector::Back(){
    return info_->data.back();
}

void COWVector::PushBack(const std::string& value){
    DeepCopy();
    info_->data.push_back(value);
}

void COWVector::Set(size_t at, const std::string& value){
    DeepCopy();
    info_->data[at] = value;
}

// Your code goes here

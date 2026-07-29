#include "lru_cache.h"
#include <iostream>

LruCache::LruCache(size_t max_size): size_(0), max_size_(max_size), data_(), lru_(){
}; 


void LruCache::Set(const std::string& key, const std::string& value) {
    //std::cout<<"Debug: Set: " << key <<" "<<value<< " count: " <<(data_.count(key)?"true":"false")<<std::endl;
    if(data_.count(key)){
        data_[key]->value = value;
        data_[key]->Unlink();
    }
    else{
        data_[key] = nullptr;
        data_[key] = new Node(key, value);
        size_++;
        //std::cout<<"size increased to "<<size_<<std::endl;
    }
    lru_.PushBack(data_[key]);
    if(size_ > max_size_)
    {
        //std::cout<<"size_  > max_size_ "<<std::endl;
        auto node = lru_.Front();
        //std::cout<<"erasing key: "<<node->key  << " value " << node->value<<std::endl; 
        data_.erase(node->key);
        lru_.PopFront();
        delete node;
        size_--;
    }
}

bool LruCache::Get(const std::string& key, std::string* value) {
    //std::cout<<"Debut: Get: " << key<<std::endl;
    if(!data_.count(key))
    {
        return false;
    }
    auto& nod = data_[key];
    *value = nod -> value;
    AddToList(nod);
    return true;
}

void LruCache::AddToList(Node* nd){
    nd->Unlink();
    lru_.PushBack(nd);
}

LruCache::~LruCache(){
    lru_.Clear();
    for(auto& p: data_){delete p.second;}
}
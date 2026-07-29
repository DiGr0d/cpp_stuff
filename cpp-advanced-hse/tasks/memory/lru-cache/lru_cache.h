#pragma once

#include <unordered_map>
#include <string>

class LruCache;

struct Node{
    Node():value(), key(),next(this), prev(this){};
    Node(std::string v, const std::string& val):value(val), key(v), next(this), prev(this){}

    void Unlink(){next->prev = prev; prev->next = next; next = prev = this;}
    void LinkBefore(Node* other){
        Unlink();
        other->prev->next = this;
        prev = other->prev;
        other->prev = this;
        next = other;
    }
    std::string value;
    std::string  key;
    Node* next;
    Node* prev;
};

struct List{
    friend std::ostream& operator<<(std::ostream& os, LruCache& cache);
    List():dummy(){}
    void PopFront(){
        dummy.next->Unlink();
    }
    void PushBack(Node* nd)
    {
        nd->LinkBefore(&dummy);
    }
    Node* Front()
    {
        return dummy.next;
    }
    inline void Clear()
    {
        while(dummy.next != &dummy)
        {
            PopFront();
        }
    }
    ~List()
    {
        Clear();
    }
    Node dummy;
};


class LruCache {
public:
friend std::ostream& operator<<(std::ostream& os, LruCache& cache);
    ~LruCache();
    LruCache(size_t max_size); 

    void Set(const std::string& key, const std::string& value);

    bool Get(const std::string& key, std::string* value);

    int Size() const{return size_;}

private:
    void AddToList(Node* nd);
    size_t size_;
    size_t max_size_;
    std::unordered_map<std::string, Node*> data_;
    List lru_;
};

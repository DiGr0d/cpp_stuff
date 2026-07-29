#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <iostream>

class ListHook {
public:
    ListHook();

    bool IsLinked() const;

    void Unlink();

    // Must unlink element from list
    ~ListHook();

    ListHook(const ListHook&) = delete;

    ListHook* prev_;
    ListHook* next_;

private:
    template <class T>
    friend class List;
    // that helper function might be useful
    void LinkBefore(ListHook* other);
};

ListHook::ListHook():prev_(this), next_(this){}
bool ListHook::IsLinked() const {return prev_ != this && next_ != this;}
void ListHook::Unlink(){if(IsLinked()){
    prev_->next_ = next_; next_->prev_ = prev_;prev_ = next_ = this;
};}
ListHook::~ListHook(){Unlink();}
void ListHook::LinkBefore(ListHook* other){Unlink();other->prev_->next_ = this; prev_ = other->prev_; other->prev_ = this; this->next_ = other;}


template <typename T>
class List {
public:
    class Iterator {
        using IteratorTag = std::bidirectional_iterator_tag;
    public:
        typedef T           value_type;
        typedef ptrdiff_t   difference_type;
        typedef T*          pointer;
        typedef T&          reference;
        typedef IteratorTag iterator_category;

        Iterator(T* ptrt):ptr_(ptrt){}
        Iterator& operator++(){
            ptr_ = static_cast<T*>(ptr_->next_); return *this;
        };
        Iterator operator++(int){
            Iterator tmp = *this;
            ++*this;
            return tmp;
        };
        Iterator& operator--(){
            ptr_ = static_cast<T*>(ptr_->prev_); return *this;
        };
        Iterator operator--(int){
            Iterator tmp = *this;
            --*this;
            return tmp;
        };

        T& operator*() const{return *ptr_;}
        T* operator->() const{return ptr_;}

        bool operator==(const Iterator& rhs) const{return ptr_ == rhs.ptr_;};
        bool operator!=(const Iterator& rhs) const{return ptr_ != rhs.ptr_;};
    private:
        T* ptr_;
    };

    List() = default;
    List(const List&) = delete;
    List(List&& other);

    // must unlink all elements from list
    ~List();

    List& operator=(const List&) = delete;
    List& operator=(List&& other);

    bool IsEmpty() const;
    // that method is allowed to be O(n)
    size_t Size() const;

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem);
    void PushFront(T* elem);

    T& Front(){return *static_cast<T*>(dummy_.next_);}
    const T& Front() const{return *static_cast<const T*>(dummy_.next_);}

    T& Back(){return *static_cast<T*>(dummy_.prev_);}
    const T& Back() const{return *static_cast<T*>(dummy_.prev_);};

    void PopBack(){dummy_.prev_->Unlink();}
    void PopFront(){dummy_.next_->Unlink();}

    Iterator Begin(){return Iterator(&Front());};
    Iterator End(){return Iterator(static_cast<T*>(&dummy_));};

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element){return Iterator(element);};

private:
    void SwapDummies(List& other){
        auto b1 = dummy_.next_;
        auto b2 = other.dummy_.next_;
        bool emptythis = IsEmpty();
        bool emptyother = other.IsEmpty();
        dummy_.Unlink();
        other.dummy_.Unlink();
        if(!emptyother)
        {
            dummy_.LinkBefore(b2);
        }
        if(!emptythis)
        {
            other.dummy_.LinkBefore(b1);
        }
    }
    ListHook dummy_;
};

template <typename T>
List<T>::List(List&& other):dummy_(){
   SwapDummies(other);
}
template <typename T>
List<T>::~List(){
    std::cout<<"Destructor"<<std::endl;
    while(dummy_.IsLinked()){
        dummy_.next_->Unlink();
    }
}
template <typename T>
List<T>& List<T>::operator=(List&& other){
    if(this != &other){
        SwapDummies(other);
    }
    return *this;
}
template<typename T>
bool List<T>::IsEmpty() const{
    return !dummy_.IsLinked();
}
template <typename T>
size_t List<T>::Size() const{
    ListHook* beg = dummy_.next_;
    size_t sz = 0;
    while(beg!=&dummy_){
        sz++;
        beg=beg->next_;
    }
    return sz;
}
template <typename T>
void List<T>::PushBack(T* elem){
    elem->LinkBefore(&dummy_);
}
template<typename T>
void List<T>::PushFront(T* elem){
    elem->LinkBefore(dummy_.next_);
}



template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}

#include <iostream>
#include "intrusive_list.h"
#include <algorithm>

struct Item: public ListHook{

    explicit Item(int i) : i(i), blob("abcdefgh") {
        blob += std::to_string(i);
    }

    int i;
    std::string blob;  // put nontrivial field here, hopefully incorrect code
                       // will hit double free

    bool operator==(const Item& other){return blob == other.blob;}
    Item(const Item&) = delete;
    Item(Item&&) = delete;
    Item& operator=(const Item&) = delete;
    Item& operator=(Item&&) = default;
};

int main()
{
    List<Item> lst;
    Item a(3), b(4), c(5), d(6);
    lst.PushBack(&a);
    lst.PushBack(&c);
    lst.PushBack(&d);
    lst.PushFront(&b);
    Item a1(11), b1(23), c1(34), d1(342);
    List<Item>lst1;
    lst1.PushBack(&a1);
    lst1.PushBack(&b1);
    lst1.PushBack(&c1);
    lst1.PushBack(&d1);
    lst1 = std::move(lst);
    std::cout<<"+__+++++_+"<<std::endl;
    for(auto it = lst1.Begin(); it != lst1.End(); it++)
    {
        std::cout<<(*it).blob<<std::endl;
    }
    std::cout<<"\nbackwards\n"<<std::endl;
    for(auto it = --lst1.End(); it!=--lst1.Begin(); it--)
    {
        std::cout<<(*it).blob<<std::endl;
    }
    std::cout<<std::endl;

    lst1.PopFront();
    lst1.PopBack();
    std::cout<<std::endl;
    for(auto it = lst1.Begin(); it != lst1.End(); it++)
    {
        std::cout<<(*it).blob<<std::endl;
    }
    std::cout<<std::endl;
    lst1.PushBack(&b);
    lst1.PushFront(&d);
    for(auto it = lst1.Begin(); it != lst1.End(); it++)
    {
        std::cout<<(*it).blob<<std::endl;
    }

    std::cout<<"\nbackwards\n"<<std::endl;
    for(auto it = --lst1.End(); it!=--lst1.Begin(); it--)
    {
        std::cout<<(*it).blob<<std::endl;
    }
    std::cout<<std::endl;
    
    std::cout<<"+++++++++++++++++++++++++++++++++++"<<std::endl;
    while(lst1.Size() != 0)
    {
        for(auto it = lst1.Begin(); it != lst1.End(); it++)
        {
            std::cout<<(*it).blob<< " ";
        }
        std::cout<<"\nbackwards"<<std::endl;
        for(auto it = --lst1.End(); it!=--lst1.Begin(); it--)
        {
            std::cout<<(*it).blob<<" ";
        }
        std::cout<<(*lst1.Begin()).blob<<std::endl;
        std::cout<<"\n===="<<std::endl;
        std::cout<<std::endl;
        lst1.PopFront();
        //std::cout<<lst1.Size()<<std::endl;
    }
}
#pragma once
#include<string>
#include <cstddef>
class Directory;

class Node{
    inline static int count=0;
    Directory *parent_=nullptr;
    std::string name_="";
    public:
    explicit Node(std::string name):name_(name){
        count++;
    }
    virtual ~Node(){
        count--;
    }
    //getters
    const std::string& get_name()const{
        return name_;
    }
    Directory *get_parent()const{
        return parent_;
    }
    static int get_count(){
        return count;
    }
    //setters
    void set_parent(Directory *parent){
        parent_=parent;
    }
    void set_name(std::string name){
        name_=name;
    }
    //virtual functions
    virtual std::size_t size() const=0;
    virtual bool is_directory() const=0;

};
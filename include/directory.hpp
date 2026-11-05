#pragma once
#include "node.hpp"
#include <vector>
#include <string>
class Directory:public Node{
    std::vector<Node*>children_;
    public:
    explicit Directory(std::string name):Node(name){}
    ~Directory(){
        for(std::size_t i=0;i<children_.size();i++){
            delete children_[i];
        }
    }
    Node* find_child(const std::string &name)const{
        for(std::size_t i=0;i<children_.size();i++){
            if(children_[i]->get_name()==name)return children_[i];
        }
        return nullptr;
    }
    bool add_child(Node *child){
        if(find_child(child->get_name())!=nullptr)return false;
        children_.push_back(child);
        child->set_parent(this);
        return true;
    }
    Node* remove_child(const std::string &name){
        if(find_child(name)==nullptr)return nullptr;
        Node* tmp=nullptr;
        int idx=-1;
        for(std::size_t i=0;i<children_.size();i++){
            if(children_[i]->get_name()==name){
                tmp=children_[i];
                idx=i;
            }
        }
        children_.erase(children_.begin()+idx);
        tmp->set_parent(nullptr);
        return tmp;
    }
    const std::vector<Node*>& get_children()const{
        return children_;
    }
    std::size_t size()const override{
        int cur=0;
        for(std::size_t i=0;i<children_.size();i++){
            cur+=children_[i]->size();
        }
        return cur;
    }
    bool is_directory()const override{
        return true;
    }
    Node* clone()const override{
        Directory* dir=new Directory(this->get_name());
        for(auto elem:this->children_){
            dir->add_child(elem->clone());
        }
        return dir;
    }
    Directory(const Directory&)=delete;
    Directory& operator=(const Directory&)=delete;
};
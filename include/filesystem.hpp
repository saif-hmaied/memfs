#pragma once
#include "file.hpp"
#include "directory.hpp"
#include <string>
#include <vector>
class Filesystem{
    Directory *root_=nullptr;
    Directory *cwd_=nullptr;
    public:
    Filesystem():root_(new Directory("/")),cwd_(root_){}
    ~Filesystem(){
        delete root_;
    }
    Filesystem(const Filesystem&)=delete;
    Filesystem& operator=(const Filesystem&)=delete;

    bool mkdir(const std::string& name){
        if(cwd_->find_child(name)!=nullptr)return false;
        Directory *tmp=new Directory(name);
        cwd_->add_child(tmp);
        return true;
    }
    bool touch(const std::string &name){
        if(cwd_->find_child(name)!=nullptr)return false;
        File *tmp=new File(name);
        cwd_->add_child(tmp);
        return true;
    }
    const std::vector<Node*>& ls()const{
        return cwd_->get_children();
    }
    std::string pwd()const{
        if(cwd_==root_)return "/";
        Node *tmp=cwd_;
        std::string ans="";
        while(tmp!=root_){
            ans="/"+tmp->get_name()+ans;
            tmp=tmp->get_parent();
        }
        return ans;
    }
    bool cd(const std::string &name){
        if(name==".."){
            if(cwd_==root_)return false;
            cwd_=cwd_->get_parent();
            return true;
        }
        if(name=="/"){
            cwd_=root_;
            return true;
        }
        Node *tmp=cwd_->find_child(name);
        if(tmp==nullptr)return false;
        if(!tmp->is_directory())return false;
        cwd_=static_cast<Directory*>(tmp);
        return true;
    }

};

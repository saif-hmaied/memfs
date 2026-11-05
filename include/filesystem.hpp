#pragma once
#include "file.hpp"
#include "directory.hpp"
#include <string>
#include <vector>
#include<algorithm>
class Filesystem{
    Directory *root_=nullptr;
    Directory *cwd_=nullptr;
    private:
    static std::vector<std::string> split_path(const std::string &path){
        std::vector<std::string>ans;
        int n=path.size();
        int last=-1;
        for(int i=0;i<n;i++){
            if(path[i]=='/'){
                std::string cur="";
                for(int j=last+1;j<i;j++){
                    cur+=path[j];
                }
                if(!cur.empty())ans.push_back(cur);
                last=i;
            }
        }
        std::string  cur="";
        for(int i=last+1;i<n;i++){
            cur+=path[i];
        }
        if(!cur.empty())ans.push_back(cur);
        return ans;
    }
    public:
    Filesystem():root_(new Directory("/")),cwd_(root_){}
    ~Filesystem(){
        delete root_;
    }
    Filesystem(const Filesystem&)=delete;
    Filesystem& operator=(const Filesystem&)=delete;

    bool mkdir(std::string path){
        std::string name="";
        while(!path.empty()&&path.back()!='/'){
            name+=path.back();
            path.pop_back();
        }
        if(name.empty()||name==".."||name==".")return false;
        reverse(name.begin(),name.end());
        auto cur=solve_path(path);
        if(cur==nullptr||!cur->is_directory())return false;
        Directory* dir=static_cast<Directory*>(cur);
        if(dir->find_child(name))return false;
        Directory *tmp=new Directory(name);
        dir->add_child(tmp);
        return true;
    }
    bool touch(std::string path){
        std::string name="";
        while(!path.empty()&&path.back()!='/'){
            name+=path.back();
            path.pop_back();
        }
        if(name.empty()||name==".."||name==".")return false;
        std::reverse(name.begin(),name.end());
        auto cur=solve_path(path);
        if(cur==nullptr||!cur->is_directory())return false;
        Directory* dir=static_cast<Directory*>(cur);
        if(dir->find_child(name)!=nullptr)return false;
        File *tmp=new File(name);
        dir->add_child(tmp);
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
    bool cd(const std::string &path){
        auto cur=solve_path(path);
        if(cur==nullptr||!cur->is_directory())return false;
        cwd_=static_cast<Directory*>(cur);
        return true;
    }
    Node* solve_path(const std::string &path)const{
        auto parts=split_path(path);
        if(path.empty())return cwd_;
        Node* tmp=(path[0]=='/'?root_:cwd_);
        for(std::size_t i=0;i<parts.size();i++){
            if(parts[i]==".")continue;
            if(parts[i]==".."){
                if(tmp->get_parent()==nullptr)return nullptr;
                tmp=tmp->get_parent();
                continue;
            }
            Node* nxt=static_cast<Directory*>(tmp)->find_child(parts[i]);
            if(nxt==nullptr||(!nxt->is_directory()&&i!=parts.size()-1))return nullptr;
            tmp=nxt;
        }
        return tmp;
    }
};

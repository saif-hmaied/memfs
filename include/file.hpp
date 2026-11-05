#pragma once
#include "node.hpp"

class File: public Node{
    std::string content_="";
    public:
    explicit File(std::string name):Node(name){}
    //getters
    const std::string &get_content()const {
        return content_;
    }
    //setters
    void set_content(const std::string &content){
        content_=content;
    }
    bool is_directory()const override{
        return false;
    }
    Node* clone()const override{
        File* file=new File(this->get_name());
        file->content_=this->content_;
        return file;
    }
    std::size_t size()const override{
        return content_.size();
    }

};
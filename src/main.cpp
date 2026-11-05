#include<iostream>
#include"filesystem.hpp"
#include<vector>
#include<string>
#include<iostream>

std::vector<std::string>split(const std::string &s){
    std::vector<std::string>ans;
    int last=-1;
    for(size_t i=0;i<s.size();i++){
        if(s[i]==' '){
            std::string cur="";
            for(size_t j=last+1;j<i;j++){
                cur+=s[j];
            }
            if(!cur.empty())ans.push_back(cur);
            last=i;
        }
    }
    std::string cur="";
    for(size_t i=last+1;i<s.size();i++){
        cur+=s[i];
    }
    if(!cur.empty())ans.push_back(cur);
    return ans;
}
using namespace std;
string Help=R"(Commands:
  help                     show this list
  exit                     quit
  pwd                      print the current folder
  ls [path]                list a folder (current folder if no path)
  cd <path>                change folder
  mkdir <path>             create a folder
  touch <path>             create an empty file
  write <path> <text...>   set a file's content
  cat <path>               print a file's content
  rm [-r] <path>           delete a file (-r for a folder)
  mv <src> <folder>        move into a folder
  cp [-r] <src> <folder>   copy into a folder (-r for a folder)
  stats                    number of nodes in memory)";
int main(){
    Filesystem f;
    while(true){
        cout<<"memfs:"+f.pwd()+"$ ";
        string s;
        if(!getline(cin,s))break;
        vector<string> words=split(s);
        if(words.empty())continue;
        string cmd=words[0];
        int n=words.size();
        if(cmd=="exit"){
            break;
        }else if(cmd=="help"){
            cout<<Help<<endl;
        }else if(cmd=="pwd"){
            cout<<f.pwd()<<endl;
        }else if(cmd=="ls"){
            auto v=f.ls(n>=2?words[1]:"");
            if(v==nullptr){
                cout<<"ls: cannot access "<<words[1]<<endl;
                continue;
            }
            for(auto child:*v){
                cout<<child->get_name();
                if(child->is_directory())cout<<"/";
                cout<<endl;
            }
        }else if(cmd=="cd"){
            if(n<2){cout<<"usage: cd <path>"<<endl;continue;}
            if(!f.cd(words[1]))cout<<"cd: cannot go to "<<words[1]<<endl;
        }else if(cmd=="mkdir"){
            if(n<2){cout<<"usage: mkdir <path>"<<endl;continue;}
            if(!f.mkdir(words[1]))cout<<"mkdir: cannot create "<<words[1]<<endl;
        }else if(cmd=="touch"){
            if(n<2){cout<<"usage: touch <path>"<<endl;continue;}
            if(!f.touch(words[1]))cout<<"touch: cannot create "<<words[1]<<endl;
        }else if(cmd=="write"){
            if(n<3){cout<<"usage: write <path> <text...>"<<endl;continue;}
            string text=words[2];
            for(int i=3;i<n;i++)text+=" "+words[i];
            if(!f.write(words[1],text))cout<<"write: cannot write to "<<words[1]<<endl;
        }else if(cmd=="cat"){
            if(n<2){cout<<"usage: cat <path>"<<endl;continue;}
            auto content=f.cat(words[1]);
            if(content==nullptr)cout<<"cat: cannot read "<<words[1]<<endl;
            else cout<<*content<<endl;
        }else if(cmd=="rm"){
            bool r=(n>=2&&words[1]=="-r");
            int idx=(r?2:1);
            if(n<=idx){cout<<"usage: rm [-r] <path>"<<endl;continue;}
            if(!f.rm(words[idx],r))cout<<"rm: cannot remove "<<words[idx]<<endl;
        }else if(cmd=="mv"){
            if(n<3){cout<<"usage: mv <src> <folder>"<<endl;continue;}
            if(!f.mv(words[1],words[2]))cout<<"mv: cannot move "<<words[1]<<" to "<<words[2]<<endl;
        }else if(cmd=="cp"){
            bool r=(n>=2&&words[1]=="-r");
            int idx=(r?2:1);
            if(n<=idx+1){cout<<"usage: cp [-r] <src> <folder>"<<endl;continue;}
            if(!f.cp(words[idx],words[idx+1],r))cout<<"cp: cannot copy "<<words[idx]<<" to "<<words[idx+1]<<endl;
        }else if(cmd=="stats"){
            cout<<"nodes in memory: "<<Node::get_count()<<endl;
        }else{
            cout<<"unknown command: "<<cmd<<endl;
        }
    }
    return 0;
}

#include "huffman.h"
#include "filemanager.h"
#include <algorithm>
#include <iostream>
#include <bitset>
#include <stack>
#include <fstream>

huffman::huffman(){
}

void huffman::clear(){
    for(int i=0;i<this->list.size();i++){
        delete list[i];
    }
    list.clear();
    this->frequencyMap.clear();
    this->frequencyVector.clear();
    this->info="";
    this->code.clear();
}

void huffman::load(std::string& fileName){
    clear();
    fileManager::readFile(fileName.c_str(),this->info);
    // std::cout<<this->data;
    for(int i =0;i<this->info.length();i++){
        char key = this->info.at(i);
        std::cout<<i<<"the key is: "<<key<<std::endl;
        if(frequencyMap.count( key )==0){
            this->frequencyMap[key]=1;
        }else{
            this->frequencyMap[key]+=1;
        }
    }

    std::vector<std::pair<char,int>> values(this->frequencyMap.begin(),this->frequencyMap.end());
    auto cmp = [](const std::pair<char,int>& l, std::pair<char,int>& r) { return l.second > r.second;};
    std::sort(values.begin(),values.end(),cmp);
    this->frequencyVector = values;

    for(int i =0;i<this->frequencyVector.size();i++){
        huffman::node* leafnode;
        leafnode = new huffman::node();
        leafnode->name=this->frequencyVector[i].first;
        leafnode->freq=this->frequencyVector[i].second;
        leafnode->isLeaf= true;
        this->list.push_back(leafnode);
    }
    for (auto& t : this->frequencyVector){
        std::cout << t.first << " " << t.second<<std::endl;
    }

    construct();
}

void huffman::construct(){
    while(this->list.size()>1){
        huffman::node* parentnode;
        node* leftnode = this->list.back();
        this->list.pop_back();
        node* rightnode = list.back();
        list.pop_back();
        parentnode = new huffman::node();
        parentnode->freq=leftnode->freq+rightnode->freq;
//        std::cout<<parentnode->freq<<"freq\n";
        parentnode->left=leftnode;
        parentnode->right=rightnode;
        parentnode->isLeaf = false;

        if(list.size()==0){
            list.push_back(parentnode);
            break;
        }
        for(int i =0;i<list.size();i++){
            int listFreq;
            listFreq=list[i]->freq;
            if(parentnode->freq>listFreq){
                auto temp = list[i];
                list[i] = parentnode;
                for(int j=i+1;j<list.size();j++){
                    auto temp2 = list[j];
                    list[j] = temp;
                    temp = temp2;
                }
                list.push_back(temp);
                // std::cout<<list.size()<<" ";
                break;
            }else{
                if(i==list.size()-1){
                    this->list.push_back(parentnode);
                    break;
                }
            }
        }
    }
}

void huffman::encode(std::string& fileName){
    if(this->list.size()==1){
        huffman::node* topnode = list[0];
        std::string leftBitString = "0";
        std::string rightBitString = "1";
        node* leftnode = topnode->left;
        node* rightnode = topnode ->right;
        encodeNode(leftnode,leftBitString);
        encodeNode(rightnode,rightBitString);
    }
    std::string bitString="";
    for(int i=0;i<info.length();i++){
        bitString+=code[info.at(i)];
    }

    std::string output="";
    int totalChar = bitString.length();
    while(bitString.length()>8){
        // std::cout<<bitString.length()<<" ";
        std::string window= bitString.substr(0,8);
        std::bitset<8> byte(window);
        unsigned long i = byte.to_ulong(); 
        unsigned char c = static_cast<unsigned char>( i );
        output+=c;
        bitString=bitString.substr(8);
    }
    while(bitString.length()<8){
        bitString+="0";
    }
    std::bitset<8> byte(bitString);
    unsigned long i = byte.to_ulong(); 
    unsigned char c = static_cast<unsigned char>( i );
    output+=c;
    std::cout<<output.length()<<std::endl;

    std::stack<node*> deepFirst;
    deepFirst.push(list[0]);
    list.pop_back();
    while(deepFirst.size()>0){
        node* curr = deepFirst.top();
        list.push_back(curr);
        deepFirst.pop();
        if(!curr->isLeaf){
            deepFirst.push(curr->left);
            deepFirst.push(curr->right);
        }
    }

    std::string header=std::to_string(list.size())+" "+std::to_string(totalChar)+" ";
    for(int i =list.size()-1;i>=0;i--){
        if(list[i]->isLeaf){
            header+="1";
            header+=list[i]->name;
        }else{
            header+="0";
        }
    }
    header+=output;
    this->info = header;
    fileManager::writeFile(fileName.c_str(),header);
}

void huffman::decode(std::string& fileName){
    clear();
    node* root;
    int nodeNumber, outputNumber;
    std::string output;
    std::stack<node*> leafnodes;
    std::ifstream file(fileName.c_str(), std::ios::in|std::ios::binary);
    if (file) {
        file>>nodeNumber>>outputNumber;
        char space;
        file.get(space);
        
        
        for(int i=0;i<nodeNumber;i++){
            char type;
            char name;
            file.get(type);
            if(type=='1'){
                file.get(name);
                node* leaf=new node();
                leaf->isLeaf=true;
                leaf->name=name;
                leafnodes.push(leaf);
            }else{
                node* parent=new node();
                parent->isLeaf=false;
                parent->right = leafnodes.top();
                leafnodes.pop();
                parent->left = leafnodes.top();
                leafnodes.pop();
                leafnodes.push(parent);
            }
        }
        root = leafnodes.top();
        
        char c;
        file.get(c);
        while(!file.eof()){
            output+=c;
            file.get(c);
        }
        file.close();
    }
    else {
        std::cerr << "Couldn't open " << fileName << " for reading\n";
    }

    std::ofstream myfile;
    myfile.open (fileName);

    std::cout<<output.size();
        // DECODE
    node* curr = root;
    std::string outputBit;
    for(int i=0;i<output.length();i++){
        std::bitset<8> outputByte(output.at(i));
        outputBit += outputByte.to_string();
    }
    for(int i=0;i<outputNumber;i++){
        if(outputBit.at(i)=='1'){
            curr= curr->right;
        }else{
            curr=curr->left;
        }
        if(curr->isLeaf){
            myfile<<curr->name;
            info+=curr->name;
            curr=root;
        }
    }
    
    myfile.close();
}

void huffman::encodeNode(node* curr, std::string bitString){
    if(curr->isLeaf){
        this->code[curr->name]=bitString;
    }else{
        encodeNode(curr->left,bitString+"0");
        encodeNode(curr->right,bitString+"1");
    }
}


huffman::~huffman(){
    clear();
}


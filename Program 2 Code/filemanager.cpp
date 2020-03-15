#include "filemanager.h"
#include <fstream>
#include <iostream>

fileManager::fileManager(){

}

void fileManager::readFile(const char* fileName, std::string& data){
    std::ifstream file(fileName, std::ios::in|std::ios::binary);
    if (file) {

        std::string line="";
        std::getline(file,line);
        data+=line;
        while(!file.eof()){
            data.at(data.length()-1)='\n';
            std::getline(file,line);
            data+=line;
        }
        file.close();
    }
    else {
        std::cerr << "Couldn't open " << fileName << " for reading\n";
    }
}

void fileManager::writeFile(const char* fileName, std::string& data){
    std::ofstream myfile;
    myfile.open (fileName,std::ios::out|std::ios::binary);
    myfile << data;
    myfile.close();
}

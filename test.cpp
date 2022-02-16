#include<iostream>

int main(){
    std::string str = "[hello[";

    if(str.find("[") != std::string::npos)
        std::cout <<"|"<< "here"<< "|"<<std::endl;
}
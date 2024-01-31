#include <iostream>
#include <fstream>
#include "parser.h"

// https://github.com/progsource/maddy


std::string readFile(std::string path) {
    std::ifstream fin;
    std::stringstream ss;
    fin.open(path, std::ios::in);

    if(!fin.is_open()){
        std::cerr << "cannot open the file" << std::endl;
        return "";
    }
    char buff = 0;
	while ((buff = fin.get())!=EOF){
		ss << buff;
	}
    auto str = ss.str();

    return str;
}




int main(int argc, char *argv[])    
{
    std::cout << "Hello World!" << std::endl;

    auto text = readFile("/home/wjs/Projects/test.md");
    std::stringstream markdown(text);
    auto config = std::make_shared<matrix::markdown::parser::ParserConfig>();
    auto parser = std::make_shared<matrix::markdown::parser::Parser>(config);
    const std::string output = parser->Parse(markdown);
    std::cout << output << std::endl;

   return 0;
}

#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstring>
#include <boost/algorithm/string/predicate.hpp>

#define EXTENSION ".htons"

void print_usage() {
    std::cerr << "htonsify\nUsage: htonsify <file>" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        print_usage();
        return 1;
    }
    
    std::string finame = argv[1]; //file in name
    std::string foname; //file out name
    bool encode = true;
    
    if (boost::algorithm::ends_with(finame, EXTENSION)) {
        foname = finame.substr(0, finame.length()-strlen(EXTENSION));
        encode = false;
    } else {
        foname = finame+".htons";
    }
    std::ifstream ifd(finame);
    std::ofstream ofd(foname);
    
    char buf[2];
    if (ifd.is_open() && ofd.is_open()) {
        while (!ifd.eof() && !ifd.bad()) {
            std::streamsize s = ifd.readsome(buf, 2);
            if (ofd.bad()) {
                std::cerr << "problem writing to output file" << std::endl;
                return 1;
            }
            if (s <= 0) {
                //no bytes read, exit
                //this shouldn't really happen 
                break;
            } else if (s == 1) {
                ofd << buf[0]; //leave the lonely btye alone
                break;
            } else {
                short res;
                if (encode) {
                    res = htons(*((short*)(void*)buf));
                } else /*decode*/ {
                    res = ntohs(*((short*)(void*)buf));
                }
                char* s2 = (char*)(void*)&res;
                ofd << s2[0] << s2[1];
            }
        }
    }
    ofd.close();
    ifd.close();
    return 0;
}
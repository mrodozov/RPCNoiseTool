#ifndef LBName_h
#define LBName_h

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

class LBName
{
public:
    LBName();
    LBName ( const std::string & lbtochamber,const std::string & areanoise );
    virtual ~LBName() {};
    std::string chamber ( const std::string& link );
    double areastrip ( const std::string& cha );
    double areachamber ( const std::string& cha );
    int numberstrip ( const std::string& cha );

private:
    std::map<std::string, std::string> lcmap;
    std::map<std::string, double> lcarea1;
    std::map<std::string, double> lcarea2;
    std::map<std::string, int> lcarea3;

};


#endif

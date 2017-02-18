#include "interface/LBName.h"

using namespace std;

LBName::LBName ( const string & lbtochamber,const std::string & areanoise ) {
    std::fstream fname ( lbtochamber.c_str() );
    int l=0;
    char buffer[128];
    std::string cha,link;
    int nl;
    do {
        l++;
        fname.getline ( buffer,128 );
        if ( !fname.eof() ) {
            std::stringstream is;
            is <<buffer;
            if ( is.str().find ( "LB_" ) != is.str().npos ) {
                is >> link >> nl >> cha;
                lcmap[link]=cha;

                // find the same chamber in area_noise.txt and get its surface,
                // strip surface and number of strips

                std::fstream fn ( areanoise.c_str() );
                int ll=0;
                char buff[128];
                std::string chamb;
                float areaC, areaS;
                int ns;
                do {
                    ll++;
                    fn.getline ( buff,128 );
                    if ( !fn.eof() ) {
                        std::stringstream iss;
                        iss <<buff;
                        iss >> chamb >> areaC >> areaS;
                        ns = int ( areaC/areaS );                            // check nearest int

                        size_t pos;
                        pos = chamb.find ( "RE" );
                        if ( pos!=std::string::npos ) { //found RE = endcap chamber}
                            chamb.replace ( 9,10,"" );
                        }

                        if ( chamb.compare ( cha ) ==0 ) {
                            //	      std::cout <<"CHA "<<cha<<" CHAMB "<<chamb<<" AREA =  "<<areaS<<" ns = "<<ns<<std::endl;
                            lcarea1[cha]=areaS;
                            lcarea2[cha]=areaC;
                            lcarea3[cha]=ns;
                            //	      std::cout <<"CHA "<<cha<<" CHAMB "<<chamb<<" AREA =  "<<lcarea[cha]<<" ns = "<<ns<<std::endl;
                        }

                    }
                } while ( !fn.eof() );


                //lb-chamber
            }
        }
    }

    while ( !fname.eof() );
}


std::string LBName::chamber ( const std::string& link ) {
    std::string cha;
    if ( lcmap.find ( link ) != lcmap.end() )
        cha=lcmap[link];
    return cha;
}

double LBName::areastrip ( const std::string& cha ) {
    double areaS =-2;
    if ( lcarea1.find ( cha ) != lcarea1.end() )
        areaS=lcarea1[cha];
    return areaS;
}

double LBName::areachamber ( const std::string& cha ) {
    double areaC =-1;
    if ( lcarea2.find ( cha ) != lcarea2.end() )
        areaC=lcarea2[cha];
    return areaC;
}

int LBName::numberstrip ( const std::string& cha ) {
    int NS =-3;
    if ( lcarea3.find ( cha ) != lcarea3.end() )
        NS=lcarea3[cha];
    return NS;
}



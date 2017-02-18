#ifndef LBNoise_h
#define LBNoise_h

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCut.h"
#include "TSystem.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TText.h"
#include "LBName.h"

struct LBNoise {

    ULong64_t timer;
    UInt_t  start_;
    UInt_t  stop_;
    UInt_t  binsF[96], binsW[96];
};


void LBNoiseF ( int _argc, char * _argv[] );

#endif

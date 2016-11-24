#include "interface/LBNoise.h"
#include <assert.h>
#include <cstdlib>

using std::size_t;
using std::vector;
using namespace std;

void LBNoiseF(int _argc, char * _argv[]){
  
  cout << " izstartd" << endl;
  
  float rateThr = 30000.;  // 30 kHz threshold for dangerous noisy strips
  //  float rateThr = 3000.;  // 3 kHz threshold for noisy strips
  bool debug = false;
  bool strips = false;
  unsigned stop_after_N_secs = 0;
  
  if (_argc > 6)  debug  = atoi(_argv[6]);
  if (_argc > 5) strips = atoi(_argv[5]);    // set "true" to fill in histos at the level of single strips 
  if (_argc > 7) stop_after_N_secs = atoi(_argv[7]);
  
  
  LBName lbnames(_argv[2],_argv[3]);
  std::string resultsDirectory = _argv[4];
  
  if (_argc == 1) {
  
    std::cout <<"  "<<std::endl;
    std::cout <<"    Please enter a root file.  Example: "<<std::endl;
    std::cout <<"    ./LBnoise /castor/cern.ch/user/c/ccmuon/RPC/LBMonitor/Histos_RB+1_far_2009_5_28__15_28_54_th230_hv9200.root"<<std::endl;
    std::cout <<"    End "<<std::endl;
    std::cout <<"  "<<std::endl;
    //return 0;
  }
    
  cout << " file check passed " << endl;
  
  std::string filein;
  filein=std::string(_argv[1]);

  TFile *hfile = TFile::Open(filein.c_str());
  if(!hfile->IsOpen()){
    assert(0);
    exit(-1);
  }
  
  if (hfile->IsZombie()){
    cout << " File is zombie ! " << endl;
    exit(-1);
  }
  
  std::string fileout=filein;
  std::string date =" ";
  std::string run =" ";
  size_t pos, pos1, pos2, pos3,pos4;
  int year = 2013;
  
  size_t yearStringPos = filein.find("_",0);
  if (yearStringPos == string::npos) exit(-1); //
  int positionsFound = 0;
  while (yearStringPos != string::npos && positionsFound <= 4){
    yearStringPos = (filein.find("_",yearStringPos+1));
    //cout << yearStringPos << endl;
    positionsFound ++;
  }
  string yearString = filein.substr(yearStringPos+1,4);
  year = atoi(yearString.c_str());
  
  pos = filein.find("Histos_");
  fileout.replace(0,pos+7,"Noise_");
  pos1 = filein.find(yearString);
 
 cout <<  "filein ok" << endl;
  
 date = filein.substr (pos1,10);   
 std::string month = filein.substr(pos1+5,2);
 cout << "check-1" << endl;
 pos2 = pos1 + 8;
 cout << "check 0" << endl;
 if ((month.find("_")!=std::string::npos)) { //found underscore
   month = filein.substr(pos1+5,1);
   pos2 = pos1 + 7;
   //   std::cout<<"month =  "<<month <<std::endl;
 }
 std::string day = filein.substr(pos2,2);
 // pos1 = pos1 + 7;
 if ((day.find("_")!=std::string::npos)) { 
   day = filein.substr(pos2,1);
   //   pos1 = pos1 + 6;
   //  std::cout<<"day =  "<<day<<std::endl;
 }
 
 cout << " check 1" << endl;
 struct tm timeinfo;

 int mm = atoi(month.c_str());
 int dd = atoi (day.c_str());
 // std::cout<<"mm, dd =  "<<mm<<" "<<dd<<std::endl;

  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = mm - 1;
  timeinfo.tm_mday = dd;
  timeinfo.tm_hour = 1;
  timeinfo.tm_min =  1;
  timeinfo.tm_sec = 1;

  /* call mktime: timeinfo->tm_wday will be set */
  time_t sec = mktime(&timeinfo);
  //  std::cout<<"sec =  "<<sec<<std::endl;
  
  pos2 = filein.find("run");
  if(pos2!=std::string::npos) { //found run
   run = filein.substr((pos2+4),6);
   //   std::cout<<"run =" <<run<<std::endl;
  }

  cout << " check 2" << endl;
  
 std::string filetxt1=filein;
 std::string filetxt2=filein;
 std::string filetxt3=filein;
 std::string filetxt4=filein;
 std::string filetxt5=filein;
 std::string filetxt6=filein;
 std::string filetxt7=filein;

 filetxt1.replace(0,pos+7,"ToMask_");   
 filetxt2.replace(0,pos+7,"ToUnmask_");   
 filetxt3.replace(0,pos+7,"Summary_");   
 filetxt4.replace(0,pos+7,"LowRate_");   
 filetxt5.replace(0,pos+7,"Database_");   
 filetxt6.replace(0,pos+7,"Masked_");   
 filetxt7.replace(0,pos+7,"DeadChannels_");   

 pos3 = fileout.find(".root");
 filetxt1.replace(pos3,pos3+5,".txt");
 filetxt2.replace(pos3,pos3+5,".txt");
 filetxt3.replace(pos3,pos3+5,".txt");
 filetxt4.replace(pos3,pos3+5,".txt");
 filetxt5.replace(pos3,pos3+5,".txt");
 filetxt6.replace(pos3,pos3+5,".txt");
 filetxt7.replace(pos3,pos3+5,".txt");

 ofstream myfile1;
 ofstream myfile2;
 ofstream myfile3;
 ofstream myfile4;
 ofstream myfile5;
 ofstream myfile6;
 ofstream myfile7;
 
 TTree* tree;
 
 hfile->GetObject("tree",tree);
 if (tree) { 
   cout << " File is with complete structure, continue " << endl;
 }

  else {
    cout << " file is missing the tree, abort" << endl;
    exit(-1);
  }
 
 myfile1.open((resultsDirectory+"/"+filetxt1).c_str());
 myfile2.open((resultsDirectory+"/"+filetxt2).c_str());
 myfile3.open((resultsDirectory+"/"+filetxt3).c_str());
 myfile4.open((resultsDirectory+"/"+filetxt4).c_str());
 myfile5.open((resultsDirectory+"/"+filetxt5).c_str());
 myfile6.open((resultsDirectory+"/"+filetxt6).c_str());
 myfile7.open((resultsDirectory+"/"+filetxt7).c_str());
 
 // = dynamic_cast<TTree *>(hfile->Get("tree"));
 
 Int_t nevent = tree->GetEntries();
 std::cout <<"Number of Events "<<nevent<<std::endl;
 TObjArray* oA = tree->GetListOfBranches();
 Int_t nbrs = oA->GetEntries();
 std::cout <<"Number of Branches " <<nbrs<<std::endl;
 
 std::vector<LBNoise> cs;
 for (Int_t b=0;b<nbrs;b++){
   LBNoise c;
   cs.push_back(c);
 }
 
 TFile ana((resultsDirectory+"/"+fileout).c_str(),"RECREATE","Noise Results");
 std::vector<TH1F> hstrips;
 std::vector<TH1F> hstripsNM;
 std::vector<TH1F> hiss;
 std::vector<TH1F> hissNM;
 
 
 std::string summary_title="Sum of strip mean rates (Hz) for all LBs   "+date+run;
 std::string average_title="Strip mean rates (Hz) for all LBs   "+date+run;
 TH1F hsummary1("Summary with sum",summary_title.c_str(),nbrs,0.5,nbrs+0.5);
 TH1F hsummary2("Summary with average",average_title.c_str() ,nbrs,0.5,nbrs+0.5);
 TH1F hsummary11("Summary with sum, ordered",summary_title.c_str(),nbrs,0.5,nbrs+0.5);
 TH1F hsummary12("Summary with sum, WIN",summary_title.c_str(),nbrs,0.5,nbrs+0.5);
 TH1F hsummary21("Summary with average, ordered",average_title.c_str() ,nbrs,0.5,nbrs+0.5);  
 
 TH1F hnoisy01(" Delta_t - FULL", "Distrib. noisy strips",1000,0.,10000);    //in Hz/cm2
 TH1F hnoisy02(" Delta_t - WIN ", "Distrib. noisy strips",1000,0.,10000);    //in Hz/cm2
 TH1F hnoisy11(" Strips - FULL", "Distrib. noisy strips",1000,0.,1000);      //in Hz/cm2 
 TH1F hnoisy12(" Strips - WIN ", "Distrib. noisy strips",1000,0.,1000);      //in Hz/cm2 
 TH1F hnoisy21(" Rolls - FULL", "Distrib. noisy rolls",1000,0.,1000);
 TH1F hnoisy22(" Rolls - WIN ", "Distrib. noisy rolls",1000,0.,1000);

 /* 
TH2F hnoisy3("Rate noisy strips vs noise percentage", "Summary of # noisy strips",100,0.5,100+0.5,300,0.,300000);
 */

 std::vector<std::string> lbs; 
 std::vector<bool> isNo;
 std::vector<bool> isNoisy;
 std::vector<bool> isNoisyAve;
 std::vector<bool> isMasked;
 std::vector<bool> isDisc;   

 
 
 for (Int_t brs=0;brs<nbrs;brs++){
   TBranch* _brs = dynamic_cast<TBranch *>(oA->At(brs));
   std::string branch(_brs->GetName());
   std::string cha(lbnames.chamber(branch));
  
   //qui
   std::cout <<"BranchName "<<branch<<" Chamber "<<cha<<std::endl;

   lbs.push_back(branch);                      // togliere?
   _brs->SetAddress(&(cs[brs]));
   std::string header = cha+"    "+date+run;

   TH1F h(cha.c_str(),header.c_str(),96,0.5,96+0.5);
   hiss.push_back(h);
   hissNM.push_back(h);


   char strip [50];
   for (Int_t l=0;l<96;l++){
     isNo.push_back(false);
     isNoisy.push_back(false);
     isNoisyAve.push_back(false);
     isMasked.push_back(false);
     isDisc.push_back(false);
     if (strips) {
     int ll = sprintf (strip," Strip n. %d  ",l+1); //ll string length
     std::string title = cha+strip+date+run;
     std::string titleshort = cha+strip;

     if(debug) std::cout <<title<<" ll = "<<ll<<std::endl;
     TH1F h2(titleshort.c_str(),title.c_str(),nevent,0.5,nevent+0.5);
     hstrips.push_back(h2);
     hstripsNM.push_back(h2);
     }
   }
 }




 //1st loop
 // read out single pins and subtract previous count of previous event 
 Double_t  n_noisy_tot   = 0;
 Double_t  n_noisy_tot2  = 0;
 Double_t  n_noisy_tot3  = 0;
 Double_t  n_noisy_totNM = 0;
 Double_t  n_noisy10_tot = 0;
 Double_t  n_noisy15_tot = 0;
 Double_t  n_noisy15_totNM = 0;
 Double_t  n_noisy90_tot   = 0;
 Double_t  n_noisyAve_tot  = 0;
 Double_t  rateAve_tot   = 0;
 Double_t  rateAve_totcm = 0;
 Double_t  rateAve_totNM   = 0;
 Double_t  rateAve_totNMcm = 0;
 UInt_t nDead = 0;
 UInt_t nMasked = 0;
 UInt_t nMaskedBut = 0;
 UInt_t nToMask = 0;
 float timeTot = 0;


 std::vector<const char*> label;


 myfile5<<run<<"  "<<sec<<"\n";
 // myfile5<<" run# = "<<run<<" time = "<<sec<<"\n";
 // myfile5<<" Roll "<<" N_masked_strips"<<" N_ToUnmask "<<" N_ToMask "<<" Rate_Hz "<<" Rate_Hz_cm2 "<<"\n";

 //for (Int_t brs=30;brs<31;brs++){
 for (Int_t brs=0;brs<nbrs;brs++){
   TBranch* _brs = dynamic_cast<TBranch *>(oA->At(brs));
   const char* branch(_brs->GetName());
   std::string cha(lbnames.chamber(branch));
   label.push_back(cha.c_str());


   float rateb          = 0;
   float ratebcm        = 0;
   UInt_t goodstrips    = 0;     
   float ratebNM        = 0;
   float ratebNMcm      = 0;
   UInt_t goodstripsNM  = 0;     
   Double_t  n_noisyb   = 0;
   Double_t  n_noisybNM = 0;
   Double_t  n_noisy10b = 0;
   Double_t  n_noisy15b = 0;
   Double_t  n_noisy15bNM = 0;
   Double_t  n_noisy90b = 0;
   Double_t  n_noisyAveb = 0;
   UInt_t percentage10   = 0;
   UInt_t percentage30   = 0;

   UInt_t nDeadb = 0;
   UInt_t nMaskedb = 0;
   UInt_t nMaskedButb = 0;
   UInt_t nToMaskb = 0;
   
   if (debug) std::cout <<"  " <<std::endl;
   if (!debug) std::cout <<"Analyzing now branch n. "<<brs+1<<" out of "<<nbrs<<" Chamber "<<cha<<std::endl; //qui
   
   double chamberarea =lbnames.areachamber(cha);
   double striparea   =lbnames.areastrip(cha);
   int stripnumber    =lbnames.numberstrip(cha);

   double chamberareaA =-1;
   double chamberareaB =-1;
   double chamberareaC =-1;


   if (debug) std::cout <<"Branch "<<brs+1<<" area chamber "<<chamberarea<<" area strip "<<striparea<<
     " number of strips "<<stripnumber<<" Chamber "<<cha<<std::endl; 


   for (int l=0;l<96;l++){      

     Double_t ncount    = 0;
     Double_t nprev     = 0;
     Double_t diff      = 0;
     float rate         = 0;
     float ratecm       = 0;
     float rates        = 0;
     float ratescm      = 0;

     Double_t ncountNM  = 0; // for non-masked strips
     Double_t nprevNM   = 0;
     Double_t diffNM    = 0;
     float rateNM       = 0;
     float rateNMcm     = 0;
     float ratesNM      = 0;
     float ratesNMcm    = 0;

     float ratemax       = 0;
     float percentage    = 0;   
     Double_t  t_noisy   = 0;
     Double_t  t_noisyNM = 0;
     Double_t  n_noisy   = 0;
     Double_t  n_noisyNM = 0;
     Double_t  n_noisy10 = 0;
     Double_t  n_noisy15 = 0;
     Double_t  n_noisy15NM = 0;
     Double_t  n_noisy90 = 0;
     Double_t  n_noisyAve = 0;
     Double_t timeNoisy = 0.0;
     Double_t timeNoisyNM = 0.0;




     size_t ps;
     ps = cha.find("RE");       
     if(ps!=std::string::npos) { //found RE = endcap chamber, assign partitions A, B, C

 //RE+1, RE-1
       if (cha.substr(3,4)=="1_2_") {
	 chamberareaA = 4072.47; 
	 chamberareaB = 3282.02; 
	 chamberareaC = 4412.66; 
       }
       if ((cha.substr(3,4)=="1_2_")&(l<32)) {
	 striparea   = 127.265; }
       if ((cha.substr(3,4)=="1_2_")&(l>31)&(l<64)) {
	 striparea   = 102.563; }
       if ((cha.substr(3,4)=="1_2_")&(l>63)) {
	 striparea   = 137.895; }

       if ((cha.substr(3,4)=="1_3_")&(l<32)) {
	 chamberareaA = 4663.52;
	 chamberareaB = 7998.19;
	 chamberareaC = 5245.13;
       }
       if ((cha.substr(3,4)=="1_3_")&(l<32)) {
	 striparea   = 145.735; }
       if ((cha.substr(3,4)=="1_3_")&(l>31)&(l<64)) {
	 striparea   = 249.943; }
       if ((cha.substr(3,4)=="1_3_")&(l>63)) {
	 striparea   = 163.91; }
 //RE+1_1, RE-1_1
       if (cha.substr(3,4)=="1_1_") {
	 chamberareaA = 1; // two partitions for this chamber, which area is the sum of A+B (B) and C+D (C). 
	 chamberareaB = 1112.8; // A+B
	 chamberareaC = 582.3; // C+D
	 if (l<32) striparea   = 1; // not connected up to 32
	 if (l>31 || l<48) striparea = 44.2236;
	 if (l>47 || l<64) striparea   = 25.326;
	 if (l>63 || l<80) striparea = 18.5906;
	 if (l>79 || l<96) striparea = 17.8028;
       }
       
 //RE+2, RE-2
       if ((cha.substr(3,4)=="2_2_")&(l<32)) {
	 chamberareaA = 5036.82;
	 chamberareaB = 3845.5;
	 chamberareaC = 2982.4;
       }
       if ((cha.substr(3,4)=="2_2_")&(l<32)) {
	 striparea   = 157.401; }
       if ((cha.substr(3,4)=="2_2_")&(l>31)&(l<64)) {
	 striparea   = 120.172; }
       if ((cha.substr(3,4)=="2_2_")&(l>63)) {
	 striparea   = 93.1999; }


       if ((cha.substr(3,4)=="2_3_")&(l<32)) {
	 chamberareaA = 6063.01;
	 chamberareaB = 8004.87;
	 chamberareaC = 5639.31;
       }
       if ((cha.substr(3,4)=="2_3_")&(l<32)) {
	 striparea   = 189.469;}
       if ((cha.substr(3,4)=="2_3_")&(l>31)&(l<64)) {
	 striparea   = 250.152;}
       if ((cha.substr(3,4)=="2_3_")&(l>63)) {
	 striparea   = 176.228;}

 //RE+3, RE-3
       if ((cha.substr(3,4)=="3_2_")&(l<32)) {
	 chamberareaA = 5036.82;
	 chamberareaB = 3845.5;
	 chamberareaC = 2982.4;
       }
       if ((cha.substr(3,4)=="3_2_")&(l<32)) {
	 striparea   = 157.401;}
       if ((cha.substr(3,4)=="3_2_")&(l>31)&(l<64)) {
	 striparea   = 120.172;}
       if ((cha.substr(3,4)=="3_2_")&(l>63)) {
	 striparea   = 93.1999;}


       if ((cha.substr(3,4)=="3_3_")&(l<32)) {
	 chamberareaA = 6063.01;
	 chamberareaB = 8004.87;
	 chamberareaC = 5639.31;
       }
       if ((cha.substr(3,4)=="3_3_")&(l<32)) {
	 striparea   = 189.469;}
       if ((cha.substr(3,4)=="3_3_")&(l>31)&(l<64)) {
	 striparea   = 250.152;}
       if ((cha.substr(3,4)=="3_3_")&(l>63)) {
	 striparea   = 176.228;}
	 
// RE+4, RE-4
	if ((cha.substr(3,4)=="4_2_")&(l<32)) {
	 chamberareaA = 5036.82;
	 chamberareaB = 3845.5;
	 chamberareaC = 2982.4;
       }
       if ((cha.substr(3,4)=="4_2_")&(l<32)) {
	 striparea   = 157.401;}
       if ((cha.substr(3,4)=="4_2_")&(l>31)&(l<64)) {
	 striparea   = 120.172;}
       if ((cha.substr(3,4)=="4_2_")&(l>63)) {
	 striparea   = 93.1999;}


       if ((cha.substr(3,4)=="4_3_")&(l<32)) {
	 chamberareaA = 6063.01;
	 chamberareaB = 8004.87;
	 chamberareaC = 5639.31;
       }
       if ((cha.substr(3,4)=="4_3_")&(l<32)) {
	 striparea   = 189.469;}
       if ((cha.substr(3,4)=="4_3_")&(l>31)&(l<64)) {
	 striparea   = 250.152;}
       if ((cha.substr(3,4)=="4_3_")&(l>63)) {
	 striparea   = 176.228;}
	 
	 

       chamberarea = chamberareaA+chamberareaB+chamberareaC;
       

       if (debug) std::cout <<"Branch "<<brs+1<<" strip "<<l+1<<" area chamber "<<chamberarea<<" area strip "<<striparea<<
	 //     " number of strips "<<stripnumber<<
	 " Chamber "<<cha<<std::endl; 

     }


     //check disconnected barrel strips
     size_t ps0,ps1,ps2;

     if (cha.find("RE+1_1") != string::npos){
       if (l < 32) isDisc[brs*96+l] = true;
       if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
    }
     
     ps1 = cha.find("RB1in");       
     if(ps1!=std::string::npos) { //found 
       if ((l==0)||(l==16)||(l==32)||(l==48)||(l==64)||(l==80)) { 
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }

     ps1 = cha.find("RB1out");       
     if(ps1!=std::string::npos) { //found 
       if ((l==0)||(l==8)||(l==16)||(l==24)||(l==32)||(l==40)||(l==48)||(l==56)||(l==64)||(l==72)
	   ||(l==80)||(l==88)) { 
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }

     ps1 = cha.find("RB2in");       
     if(ps1!=std::string::npos) { //found 
       if ((l==0)||(l==16)||(l==32)||(l==48)||(l==64)||(l==80)) { 
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }
 
     ps1 = cha.find("RB2out");       
     if(ps1!=std::string::npos) { //found 
       if ((l==0)||(l==1)||(l==16)||(l==17)||(l==32)||(l==33)||(l==48)||(l==49)||(l==64)||(l==65)
	   ||(l==80)||(l==81)) { 
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }

     ps1 = cha.find("RB3");       
     if(ps1!=std::string::npos) { //found 
       if ((l==0)||(l==15)||(l==16)||(l==31)||(l==32)||(l==47)||(l==48)||(l==63)||(l==64)||(l==79)
	   ||(l==80)||(l==95)) { 
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }

     //RB4 Sectors 1,2,3,5,6,7: two chambers per station, type RB4-2000. 
     //                         48 strips per roll, 96 strips per trigger layer/LB. All channels are connected.
 


     //RB4 Sectors 8 and 12:    two chambers per station, type RB4-2000 + RB4-1500: 48+36=84 strips per LB.
     //                         Non-connected channels in S8 and S12 for W+1 and W+2 are swapped w.r.t. the other wheels.


     ps0 = cha.find("W+");       
     if(ps0!=std::string::npos) { 
       //Sector 8:                The first 48 channels are connected. 
       //                         Non-connected: 49,50,63,64,65,66,79,80,81,82,95,96.

       ps1 = cha.find("RB4_8");       
       if(ps1!=std::string::npos) { 
	 if ((l==48)||(l==49)||(l==62)||(l==63)||(l==64)||(l==65)||(l==78)||(l==79)||(l==80)||(l==81)||(l==94)||(l==95)) { 
	   isDisc[brs*96+l] = true;
	   if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	 }
       }

       //Sector 12:               The last 48 channels are connected. 
       //                         Non-connected: 49,50,63,64,65,66,79,80,81,82,95,96.

       ps2 = cha.find("RB4_12");       
       if(ps2!=std::string::npos) { 
	 if ((l==0)||(l==1)||(l==14)||(l==15)||(l==16)||(l==17)||(l==30)||(l==31)||(l==32)||(l==33)||(l==46)||(l==47)) {
	   isDisc[brs*96+l] = true;
	   if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	 }
       }
     } else {  //W-2, W-1, W0

       //Sector 12:                The first 48 channels are connected. 
       //                          Non-connected: 49,50,63,64,65,66,79,80,81,82,95,96.

       ps1 = cha.find("RB4_12");       
       if(ps1!=std::string::npos) { 
	 if ((l==48)||(l==49)||(l==62)||(l==63)||(l==64)||(l==65)||(l==78)||(l==79)||(l==80)||(l==81)||(l==94)||(l==95)) { 
	   isDisc[brs*96+l] = true;
	   if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	 }
       }

       //Sector 8:                The last 48 channels are connected. 
       //                         Non-connected: 49,50,63,64,65,66,79,80,81,82,95,96.

       ps2 = cha.find("RB4_8");       
       if(ps2!=std::string::npos) { 
	 if ((l==0)||(l==1)||(l==14)||(l==15)||(l==16)||(l==17)||(l==30)||(l==31)||(l==32)||(l==33)||(l==46)||(l==47)) {
	   isDisc[brs*96+l] = true;
	   if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	 }
       }

     }
     
     //RB4 Sectors 9 and 11:    only one chambers per station, type RB4-2000: 48 strips per LB.
     //                         Only the first 48 channels are connected. 
     //                         Non-connected: from 49 to 96.
    
     ps1 = cha.find("RB4_9");       
     ps2 = cha.find("RB4_11");       
     if((ps1!=std::string::npos)||(ps2!=std::string::npos)) { 
       if (l>47) {
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }
    
     //RB4 Sector 4:  four chambers of type RB4-1500. 36 strips per roll (F/B), 72 strips per LB
     //               Non-connected: 1,2,15,16, 17,18,31,32, 33,34,47,48, 49,50,63,64, 65,66,79,80, 81,82,95,96
    
     
     ps1 = cha.find("RB4_4");       
     if(ps1!=std::string::npos) { //found 
       if ((l==0)||(l==1)||(l==14)||(l==15)||(l==16)||(l==17)||(l==30)||(l==31)||(l==32)||(l==33)
	   ||(l==46)||(l==47)||(l==48)||(l==49)||(l==62)||(l==63)||(l==64)||(l==65)||(l==78)||(l==79)
	   ||(l==80)||(l==81)||(l==94)||(l==95)) { 
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }
    
    //RB4 Sector 10:  two chambers of type RB4-2500. 60 strips per roll (F/B), 60 strips per LB
     //               Non-connected: 1,2,15,16,17,18,31,32,33,34,47,48,49,50,63,64,65,66 and from 79 on
    
     
     ps1 = cha.find("RB4_10");       
     if(ps1!=std::string::npos) { //found 
       if ((l==0)||(l==1)||(l==14)||(l==15)||(l==16)||(l==17)||(l==30)||(l==31)||(l==32)||(l==33)
	   ||(l==46)||(l==47)||(l==48)||(l==49)||(l==62)||(l==63)||(l==64)||(l==65)||(l>77)) {
	     isDisc[brs*96+l] = true;
	     if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" is not connected " <<std::endl;
	   }
     }
     
     int total_time_single_strip_counter = 0;
     
     for (int i=0;i<nevent;i++) {
       TBranch *bn = tree->GetBranch(branch);
       bn->GetEntry(i);
       //UInt_t deltat=cs[brs].stop_-cs[brs].start_;
       ULong64_t timervalue = cs[brs].timer;
       Double_t deltat = timervalue / 40000078;
       
       TObjArray *leaves  = bn->GetListOfLeaves();
       TLeaf *leaf3 = (TLeaf*)leaves->UncheckedAt(3); // gets leaf binsFull_  (all counts)
       TLeaf *leaf4 = (TLeaf*)leaves->UncheckedAt(4); // gets leaf binsWin_   (only non-masked strips appear here)
       Double_t ncount   = leaf3->GetValue(l);
       Double_t ncountNM = leaf4->GetValue(l);
       
       timeTot += deltat;
       total_time_single_strip_counter += deltat;
       
       //if ( i+1 == nevent ) stop_after_N_secs = total_time_single_strip_counter;
       
       if (total_time_single_strip_counter < stop_after_N_secs ) continue;
       
       // get previous event (i.e. previous time interval)
       bn->GetEntry(i-1);
       if (i > 0) nprev   = leaf3->GetValue(l);
       if (i > 0) nprevNM = leaf4->GetValue(l);
       
       diff  = ncount-nprev;  //single strip, single time interval
       
       if (diff < 0 ) {      //then there was an overflow, have to sum 2^(32)-1
	 
	 if (debug) std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" pin = "<<l+1<<" evt "<<i+1<<" delta t ="<<deltat<<" diff 1 = "<<diff<<std::endl;
	 cout << " overflow appear here " << endl;
	 diff = (pow(2,32)-1)-nprev+ncount;
	 ncount = ncount + (pow(2,32)-1);   // need ncount later for masked
       }
       
       //std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" LBchannel = "<<l+1<<" evt "<<i+1<<" delta t ="<<deltat<<" diff  = "<<diff<<std::endl;
       
       
       // x-check if fixed:
       if (diff < 0 ) {      
	 std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" pin = "<<l+1<<" evt "<<i+1<<" diff 2 = "<<diff<<std::endl;
       }
       
       float fl_diff = diff;
       float fl_deltat = deltat;
       
       rate =  fl_diff / fl_deltat;
       //cout << "Chamber= "<<cha<<" brs "<<brs+1<<" LBChannel = "<<l+1<<" evt "<<i+1<<" diff 2 = " << fl_diff<< " deltat " << fl_deltat <<  std::endl;
       ratecm = rate/striparea;
       rates += rate; 
       ratescm = rates/striparea;

       //       if (rate > ratemax) ratemax = rate; // was in Hz

       if (ratecm > ratemax) ratemax = ratecm;  //changed ratemax: now it is in Hz/cm2

       if (rate <0 ) 
	 std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" pin = "<<l+1<<" evt "<<i+1<<" rates = "<<rates<<" rate = "<<rate<<std::endl;
       
       //same thing with WIN counts
       
       diffNM  = ncountNM-nprevNM;  
       if (diffNM < 0 ) {      //then there was an overflow, have to sum 2^(32)-1 also for NM (in case 
	                       //they have not been masked... it may happen...
	 diffNM = (pow(2,32)-1)-nprevNM+ncountNM;
	 ncountNM = ncountNM + (pow(2,32)-1);   
       }
       
       if (diffNM < 0 ) {      
	 std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" pin = "<<l+1<<" evt "<<i+1<<" diffNM 2 = "<<diffNM<<std::endl;
	 // diffNM = (pow(2,32)-1)-nprevNM+ncountNM;
	 //ncountNM = ncountNM + (pow(2,32)-1);   
       }
       
       rateNM =  (float)((float)diffNM/(float)deltat);
       rateNMcm = rateNM/striparea;
       ratesNM += rateNM; 
       ratesNMcm = ratesNM/striparea;
       
       if (rateNM < 0 ) std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" pin = "<<l+1<<" evt "<<i+1<<" ratesNM = "<<ratesNM<<" rateNM = "<<rateNM<<std::endl;


       if(debug)       std::cout<<"Chamber "<<cha<<" Strip = "<<l+1<<" event = "<<i+1<<" rate = "<<rate 
		<<" rateNM = "<<rateNM<<" deltat = "<<deltat<<std::endl;


       if ((debug)&(i==0)) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" ratesNM " <<ratesNM<<" area "<<striparea<<" ratesNMcm "<<ratesNMcm<<std::endl;

       //
       //identify masked strips as the ones with ncount>0 before masking and ncountNM=0 after masking
       //
       //it has to be done after checking for the overflows
       //

            if (isDisc[brs*96+l] ==0) {   // only if the strip is not disconnected       
	      if ((ncount>5)&(ncountNM==0)){           //ncount > 5 to avoid fluctuations, etc.
	   isMasked[brs*96+l] = true;
	   if (i==0) nMaskedb ++ ;                 
	   if (i==0) nMasked ++ ;
	   if ((debug)&(i==5)) 
	   std::cout << "LB_channel n. = "<<l+1<<" of chamber "<<cha <<" masked. FULL "<<ncount<<" WIN "<<ncountNM <<std::endl;

	   if (i==0) myfile6 << "LB_Channel n. = "<<l+1<<" of chamber "<<cha <<" masked \n";
 
	 }
	        }



       if (rate > rateThr) {    // single strip rate > 3kHz ==> noisy strip
	 t_noisy ++ ; 
	 timeNoisy +=deltat ;
       }
       if (rateNM > rateThr) {    // single strip rate > 3kHz ==> noisy strip
	 t_noisyNM ++ ; 
	 timeNoisyNM +=deltat ;
       }

       hnoisy01.Fill(ratecm);
       hnoisy01.SetFillColor(2);
       hnoisy01.SetYTitle("Total number of time intervals");     
       hnoisy01.SetXTitle("Strip rate in the single time interval [Hz/cm2] ");     
       

       hnoisy02.Fill(rateNMcm);
       hnoisy02.SetFillColor(2);
       hnoisy02.SetYTitle("Total number of time intervals");     
       hnoisy02.SetXTitle("Strip rate in the single time interval [Hz/cm2]");     

       if (strips) {
	 if (debug) std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" pin = "<<l+1<<" evt "<<i+1<<" delta t ="<<deltat<<" rate = "<<rate <<" diff 1xx = "<<diff<<std::endl;

       hstrips[brs*96+l].SetFillColor(8);  // 2 red 4 blue 8 green
       //       hstrips[brs*96+l].SetBinContent( i+1, rate);
       hstrips[brs*96+l].SetBinContent( i+1, rate);
       hstrips[brs*96+l].SetXTitle("Delta_t (time interval)");
       hstrips[brs*96+l].SetYTitle("Single strip rate (strip counts / s) (Hz)");

       hstripsNM[brs*96+l].SetFillColor(9);  // 2 red 4 blue 8 green
       //       hstripsNM[brs*96+l].SetBinContent( i+1, rateNM);
       hstripsNM[brs*96+l].SetBinContent( i+1, rateNM);
       hstripsNM[brs*96+l].SetXTitle("Delta_t (time interval)");
       hstripsNM[brs*96+l].SetYTitle("Single strip rate (strip counts / s) (Hz)");
       }

//     cout << "current counter " << total_time_single_strip_counter << endl;
       
       if ( stop_after_N_secs &&  total_time_single_strip_counter >= stop_after_N_secs ) {
	 cout << "time is: " << total_time_single_strip_counter << " one delta is: " << deltat << endl; break ; } // break the loop
     } // nevent
     
     
     if(debug)       std::cout<<"Strip = "<<l+1<<" timeTot(s) = "<<timeTot<<" timeTot(m) = "<<timeTot/60.<<std::endl;

     if (float(timeNoisy/60.)>15.) {
       n_noisy15++;
     }
     if (float(timeNoisyNM/60.)>15.) {
       n_noisy15NM++;
     }


     if (t_noisy > 0 ) {
       n_noisy++;  
       isNo[brs*96+l] = true;
       percentage = (float)(t_noisy*100./nevent);
       if (percentage < 10) n_noisy10++;
       if (percentage > 10) percentage10++;
       if (percentage > 30) percentage30++;

       if (percentage > 90) { 
	 n_noisy90++;
	 isNoisy[brs*96+l] = true;
	 //qui
	 if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" noisy for " <<t_noisy<<" intervals out of "<<nevent
			      <<" i.e. "<< percentage <<" %  with max rate "<<ratemax <<" Hz/cm2  and isNoisy = "<<isNoisy[brs*96+l]<<std::endl;
       }}



     ///////
     if (isDisc[brs*96+l] ==0) {   // only if the strip is not disconnected


       rates = rates/((float)nevent);  //average rate of single strip over all time intervals
       ratescm = ratescm/((float)nevent);  

       if (rates > 1) goodstrips++;
       if (rates > rateThr) {
	 isNoisyAve[brs*96+l] = true;   // average strip rate > 3 kHz
	 n_noisyAve++;
       }


       if (t_noisyNM > 0 ) {
	 n_noisyNM++;  
       }
       n_noisybNM   +=n_noisyNM;

       rateb += rates; 
       ratebcm = rateb/chamberarea;         //norm. to chamber area 

	 if (debug) std::cout << " Chamber "<<cha <<" area " << chamberarea <<std::endl;

       n_noisyb   +=n_noisy;
       n_noisy10b +=n_noisy10;
       n_noisy15b +=n_noisy15;
       n_noisy15bNM +=n_noisy15NM;
       n_noisy90b +=n_noisy90;
       n_noisyAveb +=n_noisyAve;







       //       if (rateb <0 ) 
       //       std::cout << "Chamber= "<<cha<<" brs "<<brs+1<<" pin = "<<l+1<<" rateb = "<<rateb<<" rates = "<<rates<<std::endl;




       //qui
       //noisy for more than 15 minutes but not masked
       if ((t_noisy >0)&(isMasked[brs*96+l] ==0)&(float(timeNoisy/60.)>15.) ) {
	 if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" noisy for " << float(timeNoisy/60.) << " minutes, with max rate "<<float(ratemax) <<" Hz/cm2 "<<std::endl;
	 nToMask ++ ;
	 nToMaskb ++ ;
	 myfile1 << "LB_Channel n. = "<<l+1<<" of chamber "<<cha <<" noisy for " << float(timeNoisy/60.) << " minutes, with max rate "<<float(ratemax) <<" Hz/cm2 \n";
       }


       //check if there are non-noisy masked strips
       if (( isNo[brs*96+l] ==0 )&(isMasked[brs*96+l] ==1)&(ratemax<1)) { //in Hz/cm2
	 nMaskedBut ++ ;
	 nMaskedButb ++ ;
	 //qui

	 //if (debug) 
	   std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" masked! But not noisy with max rate "<<ratemax<<" Hz/cm2 " <<std::endl;
	 myfile2 << "LB_Channel n. = "<<l+1<<" of chamber "<<cha <<" is masked but not noisy with max rate "<<ratemax<<" Hz/cm2 \n" ;
       }


       //dead strips (non-disconnected and non-masked!)

       //             if (( isNo[brs*96+l] ==0 )&(isMasked[brs*96+l]==0)&(ratemax==0)) {         //max rate
       if (( isNo[brs*96+l] ==0 )&(isMasked[brs*96+l]==0)&(ratescm<0.005)) {                     // avg. rate


       //       if (( isNo[brs*96+l] ==0 )&(isMasked[brs*96+l]==0)&(ratemax>0)&(ratemax<1)) {
	 if (debug) std::cout << "Strip n. = "<<l+1<<" of chamber "<<cha <<" dead with max rate "<<ratemax
			      <<" Hz/cm2 " <<std::endl;
	 myfile7 << "LB_Channel n. = "<<l+1<<" of chamber "<<cha <<" (non masked) dead with max rate "<<ratemax<<" Hz/cm2 \n";
	 
	 nDeadb ++ ;                 
	 nDead ++ ;
	      }



     


       //hiss[brs].SetFillColor(2);  
       //hiss[brs].SetBinContent( l+1, ratemax);
       hiss[brs].SetFillColor(4);  
       hiss[brs].SetBinContent( l+1, rates);
       hiss[brs].Draw("histsame");
       hiss[brs].SetXTitle("LBchannel number");
       hiss[brs].SetYTitle("Single strip rate (strip counts / s) (Hz)");


       //repeat for bins_win
       ratesNM   = ratesNM/(float)1;//((float)nevent); 
       ratesNMcm = ratesNMcm/(float)1;//((float)nevent); 
       ratebNM   += ratesNM; 
       //     ratebNMcm += ratesNMcm;
       ratebNMcm = ratebNM/chamberarea;         //norm. to chamber area 
 

       if (ratesNM > 1) goodstripsNM++;
       hissNM[brs].SetFillColor(9);  
       hissNM[brs].SetBinContent( l+1, ratesNM);
       hissNM[brs].Draw("histsame");
       hissNM[brs].SetXTitle("LBchannel number");
       hissNM[brs].SetYTitle("Single strip rate Win (strip counts / s) (Hz)");
            
       hnoisy11.Fill(ratescm);
       hnoisy11.SetFillColor(3);
       hnoisy11.SetYTitle("Total number of strips");     
       hnoisy11.SetXTitle("Strip rate [Hz/cm2]");     

       hnoisy12.Fill(ratesNMcm);
       hnoisy12.SetFillColor(3);
       hnoisy12.SetYTitle("Total number of strips");     
       hnoisy12.SetXTitle("Strip rate [Hz/cm2]");     

       /*
	 hnoisy3.Fill(rateb,percentage);
	 hnoisy3.SetMarkerStyle(20);
	 hnoisy3.SetMarkerColor(3);
	 hnoisy3.SetXTitle("Time percentage");     
	 hnoisy3.SetYTitle("Rate of noisy strips");     
	 hnoisy3.Draw();
       */

     }  //isDisc false

   } //npin

     //     rateb = rateb/goodstrips;  //average rate over all the time intervals and all the good strips
 
    if (debug) std::cout << "brs="<<brs<<" rateb = "<<rateb<<" goodstrips = "<<goodstrips<<std::endl;
    if (debug) std::cout <<"Branch "<<brs+1<<" area chamber "<<chamberarea<<" area strip "<<striparea<<
      " number of strips "<<stripnumber<<" Chamber "<<cha<<std::endl; 

    // loop again over the strips and identify which ones...

    //////////////////////////////////////////////////////
    //
    //choose one:
    //
    if ((n_noisyb > 2 )&&(percentage10 > 2)) { // at least 3 noisy strips,  noisy > 10%  //loose cut
    //if ((n_noisyb > 9 )&&(percentage10 > 9)) { // at least 10 noisy strips,  noisy > 10%  //medium cut
    //if ((n_noisyb > 9 )&&(percentage30 > 9)) { // at least 10 noisy strips,  noisy > 30%      //tight cut

      /* 

     for (int l=0;l<96;l++){      

       Double_t ncount    = 0;
       Double_t nprev     = 0;
       Double_t diff      = 0;
       float rate         = 0;
       float rates        = 0;
       float ratemax      = 0;
       float percentage   = 0;   
       Double_t  t_noisy  = 0;
       Double_t  n_noisy  = 0;
       Double_t  n_noisy10 = 0;
       Double_t  n_noisy90 = 0;
       Double_t  n_noisyAve = 0;


 
       for (int i=0;i<nevent;i++) {
	 TBranch *bn = tree->GetBranch(branch);
	 if (debug) std::cout <<"BranchName "<<branch<<"  Event "<<i <<std::endl;
	 bn->GetEntry(i);
	 UInt_t deltat=cs[brs].stop_-cs[brs].start_;
	 if (debug) std::cout <<"Event n. "<<i <<" branch n."<<brs<<" BranchName "<<branch <<" deltat = "<<deltat<< std::endl;

	  
	 TObjArray *leaves  = bn->GetListOfLeaves();
	 TLeaf *leaf3 = (TLeaf*)leaves->UncheckedAt(3); // gets leaf binsFull_

	 if (debug) {std::cout << "i=" << i << " brs = "<<brs<< " l = "<< l<< " leaf=" << leaf3->GetName() <<
		     " len=" << leaf3->GetLen() << " value=" << leaf3->GetValue(l) << std::endl;}
       
	 Double_t ncount = leaf3->GetValue(l);

	 bn->GetEntry(i-1);

	 if (debug) {std::cout << "i=" << i -1 << " brs = "<<brs<< " l = "<< l<< " leaf=" << leaf3->GetName() <<
		     " len=" << leaf3->GetLen() << " value=" << leaf3->GetValue(l) << std::endl;}
	

	 if (i > 0) nprev = leaf3->GetValue(l);


	 diff  = ncount-nprev;  //single strip, single time interval
	 rate =  (float)((float)diff/(float)deltat);
	 rates += rate; 
	 if (rate > ratemax) ratemax = rate;

	 if (rate > rateThr) {    // single strip rate > 3kHz ==> noisy strip
	   t_noisy ++ ; 
	   if (debug) std::cout << " rate = "<<rate<<" t_noisy = "<<t_noisy <<std::endl;
	   if (debug) std::cout << "brs="<<brs<<" pin = "<<l<<" i = "<<i<<" ncount ="<<ncount<<" nprev ="<<nprev<<" diff="<<diff
				<< " rate = "<<rate<<" rates = "<<rates<<" goodstrips = "
				<<goodstrips<<std::endl;
       }



	 //fill in strip histos for noisy strips!

	 if (strips) {
	   hstrips[brs*96+l].SetFillColor(8);  
	   hstrips[brs*96+l].SetBinContent( i+1, rate);
	   hstrips[brs*96+l].SetXTitle("Delta_t (time interval)");
	   hstrips[brs*96+l].SetYTitle("Single strip rate (strip counts / s) (Hz)");
	 }

       } // nevent

       if (t_noisy > 0 ) {
	 n_noisy++;  
	 percentage = (float)(t_noisy*100./nevent);
	 if (percentage < 10) n_noisy10++;
	 if (percentage > 10) percentage10++;

       if (percentage > 90) { 
	 n_noisy90++;
	 isNoisy[brs*96+l] = true;
       }

       //qui
       //	 std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" noisy for " <<t_noisy<<" intervals out of "<<nevent
       //	      <<" i.e. "<< percentage <<" %  with max rate "<<ratemax <<" Hz  "<<std::endl;
       }




       if (debug) std::cout << "brs="<<brs<<" pin = "<<l<<" ncount ="<<ncount<<" nprev ="<<nprev<<" diff="<<diff
			     << " rate = "<<rate<<" rates = "<<rates<<" goodstrips = "<<goodstrips<<std::endl;

       // cambiare nome?

     // //hiss[brs].SetFillColor(2);  
     // //hiss[brs].SetBinContent( l+1, ratemax);
     // hiss[brs].SetFillColor(4);  
     // hiss[brs].SetBinContent( l+1, rates);
     // hiss[brs].Draw("histsame");
     // hiss[brs].SetXTitle("Strip number");
     // hiss[brs].SetYTitle("Single strip rate (strip counts / s) (Hz)");
     

   } //npin



     //qui
     //        std::cout << "   Chamber = "<<cha<<" has "<<n_noisyb <<" noisy strips --- " <<n_noisy10b<<" of which are noisy <10% of the time and "
     //   <<n_noisy90b <<" are noisy > 90% of the time "<<std::endl;
     n_noisy_tot2   +=n_noisyb;
    
     */

    }



    /////////////////////////////////////////////////////
    // loop again over the strips and identify which ones...
    //////////////////////////////////////////////////////
    //
    //choose one:
    //
    //if ((n_noisyb > 2 )&&(percentage10 > 2)) { // at least 3 noisy strips,  noisy > 10%  //loose cut
    if ((n_noisyb > 9 )&&(percentage10 > 9)) { // at least 10 noisy strips,  noisy > 10%  //medium cut
      /*

    //if ((n_noisyb > 9 )&&(percentage30 > 9)) { // at least 10 noisy strips,  noisy > 30%      //tight cut



     for (int l=0;l<96;l++){      

       Double_t ncount    = 0;
       Double_t nprev     = 0;
       Double_t diff      = 0;
       float rate         = 0;
       float rates        = 0;
       float ratemax      = 0;
       float percentage   = 0;   
       Double_t  t_noisy  = 0;
       Double_t  n_noisy  = 0;
       Double_t  n_noisy10 = 0;
       Double_t  n_noisy90 = 0;
       Double_t  n_noisyAve = 0;

task monitoring
 
       for (int i=0;i<nevent;i++) {
	 TBranch *bn = tree->GetBranch(branch);
	 if (debug) std::cout <<"BranchName "<<branch<<"  Event "<<i <<std::endl;
	 bn->GetEntry(i);
	 UInt_t deltat=cs[brs].stop_-cs[brs].start_;
	 if (debug) std::cout <<"Event n. "<<i <<" branch n."<<brs<<" BranchName "<<branch <<" deltat = "<<deltat<< std::endl;

	  
	 TObjArray *leaves  = bn->GetListOfLeaves();
	 TLeaf *leaf3 = (TLeaf*)leaves->UncheckedAt(3); // gets leaf binsFull_

	 if (debug) {std::cout << "i=" << i << " brs = "<<brs<< " l = "<< l<< " leaf=" << leaf3->GetName() <<
		     " len=" << leaf3->GetLen() << " value=" << leaf3->GetValue(l) << std::endl;}
       
	 Double_t ncount = leaf3->GetValue(l);

	 bn->GetEntry(i-1);

	 if (debug) {std::cout << "i=" << i -1 << " brs = "<<brs<< " l = "<< l<< " leaf=" << leaf3->GetName() <<
		     " len=" << leaf3->GetLen() << " value=" << leaf3->GetValue(l) << std::endl;}
	

	 if (i > 0) nprev = leaf3->GetValue(l);


	 diff  = ncount-nprev;  //single strip, single time interval
	 rate =  (float)((float)diff/(float)deltat);
	 rates += rate; 
	 if (rate > ratemax) ratemax = rate;

	 if (rate > rateThr) {    // single strip rate > 3kHz ==> noisy strip
	   t_noisy ++ ; 
	   if (debug) std::cout << " rate = "<<rate<<" t_noisy = "<<t_noisy <<std::endl;
	   if (debug) std::cout << "brs="<<brs<<" pin = "<<l<<" i = "<<i<<" ncount ="<<ncount<<" nprev ="<<nprev<<" diff="<<diff
				<< " rate = "<<rate<<" rates = "<<rates<<" goodstrips = "
				<<goodstrips<<std::endl;
       }



	 //fill in strip histos for noisy strips!

	 if (strips) {
	   hstrips[brs*96+l].SetFillColor(8);  
	   hstrips[brs*96+l].SetBinContent( i+1, rate);
	   hstrips[brs*96+l].SetXTitle("Delta_t (time interval)");
	   hstrips[brs*96+l].SetYTitle("Single strip rate (strip counts / s) (Hz)");
	 }

       } // nevent

       if (t_noisy > 0 ) {
	 n_noisy++;  
	 percentage = (float)(t_noisy*100./nevent);
	 if (percentage < 10) n_noisy10++;
	 if (percentage > 10) percentage10++;

       if (percentage > 90) { 
	 n_noisy90++;
	 isNoisy[brs*96+l] = true;
       }

       //qui
	 if (debug) std::cout << " Strip n. = "<<l+1<<" of chamber "<<cha <<" noisy for " <<t_noisy<<" intervals out of "<<nevent
			      <<" i.e. "<< percentage <<" %  with max rate "<<ratemax <<" Hz  and isNoisy = "<<isNoisy[brs*96+l]<<std::endl;
       }




       if (debug) std::cout << "brs="<<brs<<" pin = "<<l<<" ncount ="<<ncount<<" nprev ="<<nprev<<" diff="<<diff
			     << " rate = "<<rate<<" rates = "<<rates<<" goodstrips = "<<goodstrips<<std::endl;

       // cambiare nome?

     // //hiss[brs].SetFillColor(2);  
     // //hiss[brs].SetBinContent( l+1, ratemax);
     //hiss[brs].SetFillColor(4);  
     //hiss[brs].SetBinContent( l+1, rates);
     //hiss[brs].Draw("histsame");
     //hiss[brs].SetXTitle("Strip number");
     //hiss[brs].SetYTitle("Single strip rate (strip counts / s) (Hz)");
     

   } //npin



     //qui
     //        std::cout << "   Chamber = "<<cha<<" has "<<n_noisyb <<" noisy strips --- " <<n_noisy10b<<" of which are noisy <10% of the time and "
     //          <<n_noisy90b <<" are noisy > 90% of the time "<<std::endl;
   n_noisy_tot3   +=n_noisyb;
   */
    }

    
     /////////////////////////////////////////////////////

 n_noisy_tot    +=n_noisyb;
 n_noisy_totNM  +=n_noisybNM;
 n_noisy10_tot  +=n_noisy10b;
 n_noisy15_tot  +=n_noisy15b;
 n_noisy15_totNM  +=n_noisy15bNM;
 n_noisy90_tot  +=n_noisy90b;
 n_noisyAve_tot +=n_noisyAveb;

 rateAve_totNM    +=ratebNM;
 rateAve_totNMcm  +=ratebNMcm;


 hnoisy21.Fill(ratebcm);
 hnoisy21.SetMarkerStyle(3);
 hnoisy21.SetFillColor(9);
 hnoisy21.SetYTitle("Total number of rolls");     
 hnoisy21.SetXTitle("Roll rate [Hz/cm2]");     

 hnoisy22.Fill(ratebNMcm);
 hnoisy22.SetMarkerStyle(3);
 hnoisy22.SetFillColor(9);
 hnoisy22.SetYTitle("Total number of rolls");     
 hnoisy22.SetXTitle("Roll rate [Hz/cm2]");     




 hsummary1.SetFillColor(8);         // 2 red 4 blue 8 green
 hsummary1.SetBinContent( brs+1, rateb);
 // hsummary1.SetLabelOffset(-0.04);
 hsummary1.SetLabelSize(0.03);     //default 0.04 
 hsummary1.GetXaxis()->SetBinLabel(brs+1,cha.c_str());
 hsummary1.GetXaxis()->LabelsOption("");    
 hsummary1.SetYTitle("Rate (counts / s) (Hz)");


 hsummary11.SetFillColor(8);  
 hsummary11.SetBinContent( brs+1, rateb);
 // hsummary11.SetLabelOffset(-0.03);
 hsummary11.SetLabelSize(0.03); 
 hsummary11.GetXaxis()->SetBinLabel(brs+1,cha.c_str());
 hsummary11.GetXaxis()->LabelsOption("a"); 
 hsummary11.SetYTitle("Full Rate (Counts / s) (Hz)");

 //qui
 // std::cout<<"Chamber "<<brs+1<<" rate after masking "<<ratebNM<<std::endl;

 if (rateb < 5) {   // very low rate of the whole chamber before masking
   std::cout<<"Attention!! Chamber "<<cha<<" rate unmasked "<<rateb<< " Hz      rate masked "<<ratebNM<< " Hz"<<std::endl;
 myfile4<<"Chamber "<<cha<<" rate unmasked "<<rateb<< " Hz      rate masked "<<ratebNM<< " Hz \n";
 }

 hsummary12.SetFillColor(9);  
 hsummary12.SetBinContent( brs+1, ratebNM);
 // hsummary12.SetLabelOffset(-0.03);
 hsummary12.SetLabelSize(0.03); 
 hsummary12.GetXaxis()->SetBinLabel(brs+1,cha.c_str());
 hsummary12.GetXaxis()->LabelsOption("a"); 
 hsummary12.SetYTitle("WIN Rate (counts / s) (Hz)");
 

 hsummary2.SetFillColor(4);  
 if (goodstrips == 0) {
   hsummary2.SetBinContent( brs+1, 0);
 } else {
   hsummary2.SetBinContent( brs+1, rateb/goodstrips);
 }
 // hsummary2.SetLabelOffset(-0.04);
 hsummary2.SetLabelSize(0.03);     
 hsummary2.GetXaxis()->SetBinLabel(brs+1,cha.c_str());
 hsummary2.GetXaxis()->LabelsOption("");  
 hsummary2.SetYTitle("Rate (counts / s) (Hz)");


 hsummary21.SetFillColor(4);  
 if (goodstrips == 0) {
   hsummary21.SetBinContent( brs+1, 0);
 } else {
   hsummary21.SetBinContent( brs+1, rateb/goodstrips);
 }
 // hsummary21.SetLabelOffset(-0.04);
 hsummary21.SetLabelSize(0.03);    
 hsummary21.GetXaxis()->SetBinLabel(brs+1,cha.c_str());
 hsummary21.GetXaxis()->LabelsOption("a");  
 hsummary21.SetYTitle("Rate (counts / s) (Hz)");


 rateAve_totNMcm = float(rateAve_totNM/chamberarea);  //rate in Hz/cm2, norm. to chamber area
 

 myfile5<<cha<<" "<<nDeadb<<" "<<nMaskedb<<" "<<nMaskedButb<<" "<<nToMaskb<<" "<<ratebNM<<" "<<ratebNMcm<<"\n";




 } //nbrs




 rateAve_totNM   = float(rateAve_totNM/nbrs);
 // rateAve_totNMcm = float(rateAve_totNMcm/nbrs);  // no, cosi' bisognerebbe dividere per il n,
                                                    // di strip connesse.... meglio come sopra






 timeTot = float(timeTot/(nbrs*96));  // mettere il n. di strips connesse







  
 
   //qui
   
 //   std::cout << "   File = "<<filein<<" has "<<n_noisy_tot <<" noisy strips --- " <<n_noisy10_tot<<" of which are noisy <10% of the time, "
 //	     <<n_noisy90_tot <<" are noisy > 90% of the time and "<<n_noisy15_tot<<" noisy for more than 15 min "<<std::endl;
 //  std::cout << "   File = "<<filein<<" has "<<n_noisy_tot2 <<" noisy strips (loose analysis cuts) and "<<n_noisy_tot3<<" noisy strips (medium cuts)" <<std::endl;
   
 //   std::cout << "   "<<std::endl;
   //   std::cout <<filein<<" " <<n_noisy_tot<< " "<<n_noisy_totNM<<" "<<(n_noisy_tot-n_noisy_totNM)<<" "<<nMasked<<" "<<n_noisy_tot2<< " "<<n_noisy_tot3<< " "<<std::endl;

 // std::cout <<filein<<" " <<n_noisy_tot<< " "<<n_noisy15_tot<<" "<<n_noisy_totNM<<" "<<n_noisy15_totNM<<" "<<nMasked<<" "<<nMaskedBut<<" "<<n_noisy_tot2<< " "<<n_noisy_tot3<< " "<<rateAve_totNM<<" "<<float(timeTot/60.)<<std::endl;

 std::cout <<filein<<" " <<n_noisy_tot<< " "<<n_noisy15_tot<<" "<<n_noisy_totNM<<" "<<n_noisy15_totNM<<" "<<nMasked<<" "<<nMaskedBut<< " "<<rateAve_totNM<<" "<<float(timeTot/60.)<<std::endl;
   //std::cout << "   "<<std::endl;


 // std::cout <<"End of "<<filein<<" " <<std::endl;

 myfile3 <<"Summary per tower - Input file "<< filein<< "\n";
 myfile3 <<"Total number of masked strips:             " <<nMasked<<"\n";
 myfile3 <<"Total number of masked strips to unmask:   " <<nMaskedBut<<"\n";
 myfile3 <<"Total number of strips to mask:            " <<nToMask<<"\n";

 // myfile3 <<"#  of noisy and dangerous strips:          " <<n_noisy_tot2<< "\n"
 myfile3 <<"Average rate per roll in WIN data:         "<<rateAve_totNM<<" Hz \n";
 myfile3 <<"Total number of noisy strips in FULL data: " <<n_noisy_tot<< "\n";
 myfile3 <<"Total number of noisy strips in WIN  data: " <<n_noisy_totNM<<"\n";
 myfile3 <<"Noisy for more than 15' min in WIN data:   " <<n_noisy15_totNM<<"\n";
 myfile3 <<" "<< "\n";

 std::cout <<"  " <<std::endl;
 
 myfile1.close();
 myfile2.close();
 myfile3.close();
 myfile4.close();
 myfile5.close();
 myfile6.close();
 myfile7.close();


 // ---------------------------------------------------------------------------------------------


 ana.Write();
 //ana.Close();  //gives invalid pointer for some Histos files
 // tree->Scan();

 delete hfile;
 
 std::cout <<"Last line of the Noise Tool - End of "<<filein<<" " <<std::endl;
 std::cout <<"  " <<std::endl;
 
 //return 0;

 
 }

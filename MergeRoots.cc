#include "interface/ROOT.h"
#include <string>
#include <vector>
#include <sstream>
#include <utility>

using namespace std;



vector<string> explode(string del, string str)
{
int slen = str.length();
int dlen = del.length();
int found_a;
string before;
vector<string> final;
vector<string> fail (0);
if(dlen == 0) return fail;
if(slen == 0) return fail;
for(int counter = 0; counter < slen; counter ++)
{
    found_a = str.find(del);
    if(found_a != string::npos)
{
before = str.substr(0, found_a + dlen);
final.push_back(before);
str.replace(0, found_a + dlen, "");
}
else
{
if(str.length() > 0)
{
    final.push_back(str);
    str.replace(0, str.length(), "");
}
else
{
    break;
}
    }
}
return final;
}


void MergeRateHistograms(string folder,string fileList){
  
  TFile * total = new TFile((folder+"total.root").c_str(),"RECREATE");
  
  vector<string> v = explode(" ",fileList.c_str());
  for (int i = 0 ; i < v.size() ; i++){
    string rfile = folder + v.at(i);
    TFile * rootFile = new TFile(rfile.c_str(), "READ","in");
    TH1F * h1;
    TIter nextkey( rootFile->GetListOfKeys() );
    TKey *key;
    TObject * obj;
    TDirectory * dir = gDirectory;
    map <string, string> saved;
    
    while (key = (TKey*)nextkey()) {
	
	obj = (TH1F*)key->ReadObj();
	// populate the rolls here
	string histoCurrentName = obj->GetName();
	if ( (histoCurrentName.substr(0,1) == "W" || histoCurrentName.substr(0,2) == "RE") && saved.find(histoCurrentName) == saved.end()) {
	  //h1 = (TH1F*)rootFile->Get((histoCurrentName).c_str());
	  h1 = dynamic_cast<TH1F*>(obj);
	  cout << h1->GetName() << endl;
	  total->cd();
	  h1->Write();
	  saved[histoCurrentName] = histoCurrentName;
	}
    }
    rootFile->Close("R");
  }
  //total->Write();
  total->Close("R");
  
}

int main(int argc,char * argv[]){
  
  MergeRateHistograms(argv[1],argv[2]);
  
  return 0;
}
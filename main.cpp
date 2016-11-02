#include "interface/LBNoise.h"

using std::size_t;
using std::vector;
using namespace std;

int main (int argc,char * argv[]){
  
  //LBNoiseF(argc,argv);
  
  string inputFile=argv[1] , lb_to_chamber_map = argv[2], strips_area=argv[3], output_file = argv[4];
  
  TFile * inputLBfile = new TFile(inputFile.c_str(),"READ");  
  TTree * inputLBtree;  inputLBfile->GetObject("tree",inputLBtree);  
  TObjArray* listOfObjects = inputLBtree->GetListOfBranches();
  LBName  lb_name(lb_to_chamber_map,strips_area);   
  vector<string> branchNames;
  vector<string> chambersNames;
  Double_t norm_factor = 40000078;
  
  for (int entry = 0 ; entry < listOfObjects->GetEntries() ; entry++){
    //cout << listOfObjects->At(entry)->GetName() << " " << lb_name.chamber(listOfObjects->At(entry)->GetName()) << endl;
    string lb_string = listOfObjects->At(entry)->GetName();
    branchNames.push_back(lb_string);    
    chambersNames.push_back(lb_string);
  }  
  
  // try to write a tree
  TFile * bonsaiche = new TFile(output_file.c_str(),"UPDATE");
  TTree * myTree = new TTree("detailed_rate_tree", "tree title");
  
  int nEvents = inputLBtree->GetEntries();
  //cout << nEvents << endl;
  int nChannels = 96;
  int numberOfChambers = branchNames.size();
  
  Double_t  chnls[numberOfChambers][96];
  Double_t prev_counts[numberOfChambers][96];
  for (int i = 0 ; i < numberOfChambers ; ++i){
    myTree->Branch(chambersNames.at(i).c_str(),chnls[i]);
  }
  
  //cout << numberOfChambers << endl;
  
  for (int i = 0 ; i < nEvents ; i ++){
    inputLBtree->GetEntry(i);
    
    //cout << "event " << i << endl;
    
    for (int k = 0 ; k < numberOfChambers ; k++){
      string nr_name = chambersNames.at(k);
      TBranch * lb_branch = inputLBtree->GetBranch(branchNames.at(k).c_str());
      
      TLeaf * timer = (TLeaf*)lb_branch->FindLeaf("timerValue"); //        
      TLeaf * interval_counts_leaf = (TLeaf*)lb_branch->FindLeaf("binsFull_");
      ULong64_t duration = timer->GetValue();
      Double_t duration_normalized = duration / norm_factor;
      
      //cout << "chamber " << nr_name << " duration " << duration_normalized << endl;
      
      for (int j = 0 ; j < 96 ; j ++){
	
	
	
	Int_t interval_counts = interval_counts_leaf->GetValue(j);
	
	Int_t counts = (i > 0) ? (interval_counts-prev_counts[k][j]) : (interval_counts);
	Double_t rate = counts / duration_normalized;
	//cout << " channel " << j+1 << " rate " << rate << " counts " << counts << " duration " << duration_normalized << endl;
	chnls[k][j] = rate;
	prev_counts[k][j] = counts;
	
	//cout << k << " " << chnls[k-1][j] << endl;
      }
      //cout << i << " " << k << endl;
    }
    //cout << endl;
    bonsaiche->cd();
    myTree->Fill();
  }
  myTree->Write();
  bonsaiche->Close();
  inputLBfile->Close();
  
  /*
  cout << " out of writing " << endl;
  
  
  // read the tree
  
  TFile * bonsaiche_read = new TFile("bonsai.root","READ");
  TTree * tree; bonsaiche_read->GetObject("bonsai",tree);
  
    
  for (int i = 0 ; i < numberOfChambers; i++){
    TBranch * aBranch =  tree->GetBranch(branchNames.at(i).c_str());
    Double_t chnnls[96];
    tree->SetBranchAddress(aBranch->GetName(),&chnnls,&aBranch);
    
    //cout << aBranch->GetName() << endl;
    
    for(int j = 0 ; j < tree->GetEntries(); j++){
      aBranch->GetEntry(j+1);
      //cout << "entry " << j+1 << " channels " ;
      for (int ch = 0 ; ch < 96 ; ch ++){
	//cout << chnnls[ch] << " ";
      }
      //cout << endl;
    }
    
    //cout << endl;
    
    //cout << tree->GetEntries() << endl;
  }
  */
  
  
  
  return 0;
  
}

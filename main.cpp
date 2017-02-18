#include "interface/LBNoise.h"

using std::size_t;
using std::vector;
using namespace std;

int main ( int argc,char * argv[] ) {

    //LBNoiseF(argc,argv);


    string inputFile=argv[1] , lb_to_chamber_map = argv[2], strips_area=argv[3], output_file = argv[4], output_folder = argv[5],  towerStartTime = argv[6];


    TFile * inputLBfile = new TFile ( inputFile.c_str(),"READ" );
    //TTree * inputLBtree;
    TTree * inputLBtree;
    inputLBfile->GetObject ( "tree",inputLBtree );
    TObjArray* listOfObjects = inputLBtree->GetListOfBranches();

    LBName  lb_name ( lb_to_chamber_map,strips_area );
    string aline;

//     while ( getline ( IFS, aline ) ) {

//         cout  << aline << " " << lb_name.chamber ( aline ) << endl;

//     }
    //cout << lb_name.chamber(inputFile) << endl;

    vector<string> branchNames;
    vector<string> chambersNames;
    Double_t norm_factor = 40000078;
    map<string, vector <double> > chamberName_deltaT_vector;

    size_t firstUnderscore = inputFile.find ( "_" );
    string towerName = inputFile.substr ( firstUnderscore+1,inputFile.find ( "_run" )-firstUnderscore-1 );
    for ( int entry = 0 ; entry < listOfObjects->GetEntries() ; entry++ ) {
        //cout << listOfObjects->At(entry)->GetName() << " " << lb_name.chamber(listOfObjects->At(entry)->GetName()) << endl;
        string lb_string = listOfObjects->At ( entry )->GetName();
        branchNames.push_back ( lb_string );
        chambersNames.push_back ( lb_string );
        vector<double> time_durations_per_chamber;
        chamberName_deltaT_vector[lb_name.chamber ( lb_string )] = time_durations_per_chamber;
    }

    // put the .json time files to come from here



    // try to write a tree
    TFile * bonsaiche = new TFile ( output_file.c_str(),"UPDATE" );
    TTree * myTree = new TTree ( towerName.c_str(), "tree title" ); 
    if ( bonsaiche ) 

    int nEvents = 0;
    
    //nEvents = inputLBtree->GetEntries();
    
    //cout << nEvents << endl;
    int nChannels = 96;
    int numberOfChambers = branchNames.size();

    Double_t prev_counts[numberOfChambers][96];
    Double_t  chnls[numberOfChambers][96];
    for ( int i = 0 ; i < numberOfChambers ; ++i ) {
        myTree->Branch ( chambersNames.at ( i ).c_str(),chnls[i], ( chambersNames.at ( i ) +"[96]/D" ).c_str() );
    }

    //cout << numberOfChambers << endl;

    for ( int i = 0 ; i < inputLBtree->GetEntries() ; i ++ ) {
        inputLBtree->GetEntry ( i );

        //cout << "event " << i << endl;


        for ( int k = 0 ; k < numberOfChambers ; k++ ) {
            string nr_name = lb_name.chamber ( chambersNames.at ( k ) );
            TBranch * lb_branch = inputLBtree->GetBranch ( branchNames.at ( k ).c_str() );

            TLeaf * timer = ( TLeaf* ) lb_branch->FindLeaf ( "timerValue" ); //
            TLeaf * interval_counts_leaf = ( TLeaf* ) lb_branch->FindLeaf ( "binsFull_" );
            ULong64_t duration = timer->GetValue();
            Double_t duration_normalized = duration / norm_factor;

            chamberName_deltaT_vector.at ( nr_name ).push_back ( duration_normalized );

            //cout << "chamber " << nr_name << " duration " << duration_normalized << endl;

            for ( int j = 0 ; j < 96 ; j ++ ) {

                Int_t interval_counts = interval_counts_leaf->GetValue ( j );
		lb_branch->GetEntry(i-1);
		Int_t prevc = interval_counts_leaf->GetValue ( j );
		lb_branch->GetEntry(i);
		if (i == 0) prevc = 0;

                Int_t counts = interval_counts - prevc;
                Double_t rate = counts / duration_normalized;
		string lbbr = lb_branch->GetName();
// 		if (lbbr == "LB_REp4_S12_EP42_CH1"){
// 		  cout << " channel " << j+1 << " rate " << rate << " counts " << counts << " duration " << duration_normalized << endl;
// 		}
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
    
    ofstream OFS ( ( output_folder+towerName ).c_str() );
    OFS << "{" << "\n";



    unsigned mapcount = 0;

    for ( map<string, vector<double> >::iterator itr = chamberName_deltaT_vector.begin() ; itr != chamberName_deltaT_vector.end() ; itr++ ) {
        OFS << "\"" + itr->first + "\"" << ": {" << "\n" << "\"times\": [" ;
        for ( unsigned i = 0 ; i < itr->second.size() ; i++ ) {

            unsigned Nseconds = itr->second.at ( i );
            OFS << Nseconds ;
            if ( i+1 != itr->second.size() ) OFS << ",";

        }
        OFS << "]," << "\n"<< "\"startTime\": " << "\"" + towerStartTime << "\"" << " }";
        if ( mapcount+1 != chamberName_deltaT_vector.size() ) OFS << ",";
        OFS << "\n";
        OFS.clear();
        mapcount++;
    }

    OFS << "}";
    OFS.close();
    
    */
    
    /*

    cout << " out of writing " << endl;

    
    // read the tree
    
    TFile * bonsaiche_read = new TFile(output_file.c_str(),"READ");
    TTree * tree; bonsaiche_read->GetObject("YEP3_far",tree);
    //int numberOfChambers = tree->GetListOfBranches()->GetEntries();



    for (int i = 0 ; i < tree->GetListOfBranches()->GetEntries(); i++){
      TBranch * aBranch =  tree->GetBranch(tree->GetListOfBranches()->At(i)->GetName());
      Double_t chnnls[96];
      tree->SetBranchAddress(aBranch->GetName(),&chnnls,&aBranch);
      
      string lbbrr = aBranch->GetName();

      if (lbbrr == "LB_REp4_S12_EP42_CH1"){
      
      cout << aBranch->GetName() << endl;

      for(int j = 0 ; j < tree->GetEntries(); j++){
        aBranch->GetEntry(j+1);
        cout << "entry " << j+1 << " channels " ;
        for (int ch = 0 ; ch < 96 ; ch ++){
	cout << chnnls[ch] << " ";
        }
        cout << endl;
      }
      
      }
      
      //cout << endl;

      //cout << tree->GetEntries() << endl;
    }
    

    */

    return 0;

}

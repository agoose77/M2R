char data[16384]
Int_t buffer[16384]
ifstream file("R1_0c" )
for(Int_t i=0;i<16385;i++) file >> data[i];
for(Int_t i=0;i<16385;i++) file>> Int_t buffer[16384]


for (size_t t = 0; t < 60; t++) {
 cout<< unsignedchar(data[t]) << "\n";
 data->Branch("Event",&Event,"Event[%t]/I");
}

//medio funcionando


TBrowser n


///prueba buenaza
rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
//buenazo mi carnal
for (size_t j = 0; j < 20; j++) {
  Int_t TString::Format("DET%i", j); 
}


for (int j=0;j<200;j++){

data->Branch(TString::Format("DET%i", j), TString::Format("&DET%i", j), TString::Format("DET%i/I", j));
}



data.Branch("DET[0]", &DET[0], "DET[0]/I");
data.Branch("DET[1]", &DET[1], "DET[1]/I");
data.Branch("DET[2]", &DET[1], "DET[2]/I");
Int_t DET[200]

TBrowser n


rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
Int_t AA[200];
data->Branch("AA",AA,"AA[200]/F");



//////////test////////
.q
rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite)

//std::vector<int> vpx[200];
//data->Branch("vpx",&vpx);

std::vector<int> *myp = new std::vector<int>();
data->Branch("mybranch","vector<float>",&myp);

f.Write("",TObject::kOverwrite);
//std::vector<ROOT::Math::XYZTVector>  tracks;
//std::vector<ROOT::Math::XYZTVector> * pTracks = &tracks;
//data.Branch("tracks","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double>>>",&pTracks);

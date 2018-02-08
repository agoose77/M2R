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

TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
//buenazo mi carnal
char tree_[20];//
for (int j=0;j<200;j++){
//sprintf(tree_,"DET%i[%i]",j);
data.Branch(TString::Format("DET%i",j), &tree_, TString::Format("DET%i[%i]/I", j)); //bueno
}
TBrowser n


///prueba buenaza
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
//buenazo mi carnal
Int_t DET[200]
for (int j=0;j<200;j++){
data.Branch(TString::Format("DET[%i]", j), &DET, TString::Format("DET[%i]/I", j)); //bueno
}
TBrowser n

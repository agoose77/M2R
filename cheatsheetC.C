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
.q
rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
//buenazo mi carnal
int det0;
data->Branch("det0", &det0, "det0/I")


for (int j=0;j<5;j++){
data->Branch(TString::Format("det%i", j), &TString::Format("det%i", j), TString::Format("det%i/I",j));
}



;


data.Branch("DET[1]", &DET[1], "DET[1]/I");
data.Branch("DET[2]", &DET[1], "DET[2]/I");


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

//VECTORS///

std::vector<int> first(50);                             // empty vector of ints
std::vector<int> second (4,100);                       // four ints with value 100
std::vector<int> third (second.begin(),second.end());  // iterating through second
std::vector<int> fourth (third);                       // a copy of third
std::vector<int> *v = new std::vector<int>(10);


.q
rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite)

std::vector<int> first(4);
data->Branch("vector", &first,first[3]);
for (size_t i = 0; i < 4; i++) {
  first[i]=2*(i+4);
}
data->Fill();
for (size_t i = 0; i < 4; i++) {
  first[i]=3*(i+4);
}
data->Fill();
f.Write("",TObject::kOverwrite);

//// ARRAYS IN NTUPLES
rm adc.root
root -l

   Int_t i,j,event;
   Float_t adc[16][16];
   TFile *file = new TFile("adc.root","RECREATE");
   TTree *tree = new TTree("tree","Testing root trees");
   tree->Branch("adc",&adc,"adc[nadc]/F");

   for (event=0;event<100;event++) {
      for (j=0;j<16;j++) {
         for (i=0;i<16;i++) {
            adc[i][j] = 100*i +j;
         }
      }
      tree->Fill();
   }
   tree->Print();
   tree->Write();
   file->Close();
 ////////////
.q
rm adc.root
root -l

Int_t  adc[200];
Int_t i,j;
TFile *file=new TFile("adc.root","RECREATE");
TTree *tree=new TTree("tree","Testing still");
tree->Branch("adc",&adc,"adc[200]/I");
for(j=0;j<100;j++) {
for(i=0;i<200;i++) {adc[i]=i*5};
  tree->Fill();
}
for(j=0;j<100;j++) {
for(i=0;i<200;i++) {adc[i]=i*3;}
  tree->Fill();
}
tree->Write();
tree->Print();
file->Close();

 memset(event_vector, 0, sizeof(event_vector));
 event_vector[adc_num]=ampl;

ROOT resourcers

"https://root.cern.ch/root/htmldoc/guides/primer/ROOTPrimerLetter.pdf"
"http://caeinfo.in2p3.fr/root/Formation/en/"
"http://www-root.fnal.gov/root/CPlusPlus/index.html"

///// branches of arrays WORKING multiplicity

.q
rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
Int_t Mult;
Float_t Z[200];
data->Branch("M_part", &Mult, "Mult/I");
data->Branch("Z_part", Z, "Z[Mult]/F");
//Float_t Z1[200]={0};
//0
data->Write()

memset(Z1, 0, sizeof(Z1));
data->Draw("Z_part[9]>>(100,0,150)","Z_part[9]!=0","")

for (size_t i = 0; i < 200; i++) {
  Z[i]=i;
}




////////////////////
char data[16384]
Int_t buffer[16384]
ifstream file("R1_0c" )
for(Int_t i=0;i<16385;i++) file >> data[i];
for(Int_t i=0;i<16385;i++) file>> Int_t buffer[16384]


for (size_t t = 0; t < 5; t++) {
TString::Format("data->Draw(\"Z_event[%i]>>(4000,0,4000)\",\"Z_event[%i]!=0\",\"\")",t);
}

//medio funcionando


TBrowser n


///good test my dude
.q
rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
//very good mi carnal
//data->Branch("det0", &det0, "det0/I")
for (int t=0;t<5;t++){
TString::Format("data->Draw(\"Z_event[%i]>>(4000,0,4000)\",\"Z_event[%i]!=0\",\"\")",t,t);
}

data->Branch(TString::Format("det%i", j), &TString::Format("det%i", j), TString::Format("det%i/I",j));


data.Branch("DET[1]", &DET[1], "DET[1]/I");
data.Branch("DET[2]", &DET[1], "DET[2]/I");


TBrowser n


rm data.root
root -l
TFile f("data.root","UPDATE"); //ROOT file
TTree *data = new TTree("data","Tree data"); //Tree to store the Data
data->Write("data.root",TObject::kOverwrite); // Making the root file
Double_t AA[200];
data->Branch("AA",AA,"AA[200]/D");
for (int i = 0; i < 200; i++) {
  AA[i]=i*2;
}
data->Fill()
f.Write("",TObject::kOverwrite);



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

//// ARRAYS IN NTUPLES #not working
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



//////////////////////

Float_t Z[200];
for (size_t i = 0; i < 200; i++) {
  Z[i]=i;
}
memset(Z1, 0, sizeof(Z1))

//////////////////////////

for (int t=0;t<5;t++){
TString::Format("data->Draw("Z_event[%i]>>(4000,0,4000)","Z_event[%i]!=0","")");
}


//////

data->Draw("Z_event[1]>>(4000,0,4000)","Z_event[1]!=0","")
///How to divide CANVAS///

TCanvas *c1 = new TCanvas()
 c1->Divide(1,2) ///horizontal split
 c1->cd(1)
 h1->Draw()
 c1->cd(2)
 h2->Draw()

/////////////////
	for(int i=0;i<200;i++) {
		ostringstream name;
		name<<i;
    data->Draw("Z_event[name.str().c_str()]>>(4000,0,4000)","Z_event[name.str().c_str()]!=0","");
	}
hadc[i] = new TH1F(name.str().c_str(),name.str().c_str(),4096,0,4096.);

 TH1F *myhist[1000];
 char *histname = new char[10];
sprintf(histname, "h_x_%d",xx);
myhist[xx]=new TH1F(histname,"",100,-0.25,0.25);
for (Int_t iii=0;iii<nentries; iii++) {
Double_t binx=h_x->GetBinContent(iii);
myhist[xx]->SetBinContent(iii,binx);
}

string mystring

std::string firstlevel ("com");
  std::string secondlevel ("cplusplus");
  std::string scheme ("http://");
  std::string hostname;
  std::string url;

  hostname = "www." + secondlevel + '.' + firstlevel;
  url = scheme + hostname;

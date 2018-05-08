#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <fstream>
#include <TString.h>
#include <sstream>
#include <TH1.h>
#include <TH2.h>
#include "TCanvas.h"
#include <vector>

void PF_gammas() {

string mystring;
  cout << "Enter the name of the ROOT run" << "\n";
//  cin >> mystring;
  mystring="R175_0"; //178 alphas, 177 gammas, member to change the channels and energies!
  string rootext(".root");
  string txtext(".txt");
  string RF = mystring+rootext;
  string TT = mystring+txtext;
  TFile *f = TFile::Open(RF.c_str(),"UPDATE");
  ofstream myfiletxt;
  myfiletxt.open (TT.c_str(),ios::out);
  f->Print();
  Int_t ich;
  Int_t fch;

  ich=64;
  fch=73;
  Int_t peakstofind=4;
  Double_t energies[4];

  energies[0]=0.511;
  energies[1]=3.416;
  energies[2]=3.927;
  energies[3]=4.438;
  TH1I *histopeaks[200];
  TH1I *histopeakscalib[200];
  TH1I *histopeaks_background[200];

  for(int i=ich;i<=fch;i++){
      Double_t par0[3]={0};
      Double_t par1[3]={0};
      Double_t par2[3]={0};
      Double_t par3[3]={0};
      Double_t parl1[2]={0};
      Double_t centroids[4]={0};

      ostringstream name;
      name<<"hadc"<<i<< mystring;
      f->GetObject(name.str().c_str(),histopeaks[i]);
      histopeaks[i]->Draw();
      histopeaks[i]->ShowBackground();
      TH1I h = *histopeaks[i] - *histopeaks_background[i];
      TH1I *rest = &h;
      rest->GetXaxis()->SetRangeUser(180,4096);
      rest->Smooth();
      TSpectrum *s = new TSpectrum(peakstofind);
    //  const int npeaks = s->Search(histopeaks[i],2,"",0.10 ); // for alphas dssd
      const int npeaks = s->Search(rest,5,"",0.05);// for gammas labr3
      float *xpeaks = s->GetPositionX();

      cout << "channel " << i <<": " <<xpeaks[0]<<"," << xpeaks[1]<<"," << xpeaks[2] << "," << xpeaks[3] << endl;
      TF1 *g0    = new TF1("g1","gaus",xpeaks[0]-50,xpeaks[0]+50); // 200 for alphas, 20 for gammas
      TF1 *g1    = new TF1("g2","gaus",xpeaks[1]-50,xpeaks[1]+50);
      TF1 *g2    = new TF1("g3","gaus",xpeaks[2]-50,xpeaks[2]+50);
      TF1 *g3    = new TF1("g3","gaus",xpeaks[3]-50,xpeaks[3]+50);
    //https://root.cern.ch/root/html534/guides/users-guide/FittingHistograms.html
      histopeaks[i]->Fit(g0,"QR+");
      histopeaks[i]->Fit(g1,"QR");
      histopeaks[i]->Fit(g2,"QR+");
      histopeaks[i]->Fit(g3,"QR+");

      g0->GetParameters(&par0[0]);
      g1->GetParameters(&par1[0]);
      g2->GetParameters(&par2[0]);
      g3->GetParameters(&par3[0]);

      centroids[0]=par0[1];
      centroids[1]=par1[1];
      centroids[2]=par2[1];
      centroids[3]=par3[1];

      TGraph* gr = new TGraph(4,centroids,energies);
      //gr->Draw("AC*");
      TF1 *l1    = new TF1("l1","pol1",centroids[0]-10,centroids[2]+10);
      gr->Fit("l1","");
      l1->GetParameters(&parl1[0]);
      cout<< i << "\t" <<parl1[0] << "\t" << parl1[1] << endl;
      //myfiletxt<< i << "\t" <<parl1[1] << "\t" << parl1[0] << endl;
      //https://root.cern.ch/doc/master/classTH1.html#aff6520fdae026334bf34fa1800946790

       cout << "check " << i << endl;
    }
 delete s;
 //delete rest;
 delete[] histopeaks;

}

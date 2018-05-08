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


void PF() {
  string mystring;
  cout << "Enter the name of the ROOT run" << "\n";
//  cin >> mystring;
  mystring="R178_0"; //178 alphas, 177 gammas, member to change the channels and energies!
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

  ich=0;
  fch=63;
  Int_t peakstofind=3;
  Double_t energies[3];

 //alphas
  energies[0]=5.1566;
  energies[1]=5.4856;
  energies[2]=5.8048;

  TH1I *histopeaks[200];
  TH1I *histopeakscalib[200];
    for(int i=ich;i<=fch;i++){
      Double_t par1[3]={0};
      Double_t par2[3]={0};
      Double_t par3[3]={0};
      Double_t parl1[2]={0};
      Double_t centroids[3]={0};
      ostringstream name;
      name<<"hadc"<<i<< mystring;
      f->GetObject(name.str().c_str(),histopeaks[i]);
      histopeaks[i]->Draw();
      TSpectrum *s = new TSpectrum(peakstofind);
      histopeaks[i]->GetXaxis()->SetRangeUser(250,4096);
      histopeaks[i]->Smooth(80);
    //  const int npeaks = s->Search(histopeaks[i],2,"",0.10 ); // for alphas dssd
      const int npeaks = s->Search(histopeaks[i],2,"",0.003 );// for gammas labr3
      float *xpeaks = s->GetPositionX();
      cout << "channel " << i <<": " <<xpeaks[0]<<"," << xpeaks[1]<<"," << xpeaks[2] << endl;
      TF1 *g1    = new TF1("g1","gaus",xpeaks[0]-10,xpeaks[0]+10); // 200 for alphas, 20 for gammas
      TF1 *g2    = new TF1("g2","gaus",xpeaks[1]-10,xpeaks[1]+10);
      TF1 *g3    = new TF1("g3","gaus",xpeaks[2]-10,xpeaks[2]+10);
    //https://root.cern.ch/root/html534/guides/users-guide/FittingHistograms.html
      histopeaks[i]->Fit(g1,"QR");
      histopeaks[i]->Fit(g2,"QR+");
      histopeaks[i]->Fit(g3,"QR+");

      g1->GetParameters(&par1[0]);
      g2->GetParameters(&par2[0]);
      g3->GetParameters(&par3[0]);

      centroids[0]=par1[1];
      centroids[1]=par2[1];
      centroids[2]=par3[1];
      TGraph* gr = new TGraph(3,centroids,energies);
      //gr->Draw("AC*");
      TF1 *l1    = new TF1("l1","pol1",centroids[0]-10,centroids[2]+10);
      gr->Fit("l1","");
      l1->GetParameters(&parl1[0]);
      myfiletxt<< i << "\t" <<parl1[1] << "\t" << parl1[0] << endl;
      //https://root.cern.ch/doc/master/classTH1.html#aff6520fdae026334bf34fa1800946790
  //    cout << par1[1] << ", "<< par2[1]<< ", " << par3[1] << endl;
    }


 delete s;
 delete[] histopeaks;
  }


  /* how to look for peaks
  hadc70R178_0->Draw()
  hadc70R178_0->ShowBackground()
  TH1I h = *hadc70R178_0 - *hadc70R178_0_background
  TH1I *rest = &h
  rest->GetXaxis()->SetRangeUser(150,1800)
  rest->GetYaxis()->SetRangeUser(0,1000)
  TSpectrum *s = new TSpectrum(4)
  Int_t npeaks = s->Search(rest,2,"",0.05)
  */

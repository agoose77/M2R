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
/*
TFile *_file0 = TFile::Open("R162_0.root")
TH1 *hadc64R162_0b = hadc64R162_0->ShowBackground()
TH1 *hadc64R162_0co = hadc64R162_0->Clone();
hadc64R162_0co->Add(hadc64R162_0b,-1)
hadc64R162_0co->GetXaxis()->SetRangeUser(170,2000)
hadc64R162_0co->Draw()
hadc64R162_0co->Smooth(100)
hadc64R162_0co->ShowPeaks(0.5)
*/

// TFile *_file0 = TFile::Open("tchain.root")
// TH1 *back = Ex_C12->ShowBackground()
// TH1 *Ex_C12co = Ex_C12->Clone();
// Ex_C12co->Add(back,-1)
// Ex_C12co->Draw()

void PF_gammas() {

string mystring;
  cout << "Enter the name of the ROOT run" << "\n";
//  cin >> mystring;
  mystring="tchaing";
  string rootext(".root");
  string txtext(".txt");
  string RF = mystring+rootext;
  string TT = mystring+txtext;
  TFile *f = TFile::Open(RF.c_str(),"UPDATE");
  ofstream myfiletxt;
  myfiletxt.open (TT.c_str(),ios::out);
  f->Print();


  const int peakstofind=5;
  Double_t energies[5];

  energies[0]=0.511;
  energies[1]=2.2;
  energies[2]=3.416;
  energies[3]=3.927;
  energies[4]=4.438;
  TH1 *histopeaks[10];
  TH1 *histopeaksoriginal[10];
  TH1 *histopeaks_background[10];
  for(int i=0;i<10;i++){

      Double_t par0[3]={0};
      Double_t par1[3]={0};
      Double_t par2[3]={0};
      Double_t par3[3]={0};
      Double_t par4[3]={0};

      Double_t parl1[5]={0};
      Double_t centroids[5]={0};

      Int_t s_centroids[5];
      Double_t sorted_centroids[5];

      ostringstream name;
      name << "labr"<<i<<  "_0";

      f->GetObject(name.str().c_str(),histopeaks[i]);
      f->GetObject(name.str().c_str(),histopeaksoriginal[i]);
      histopeaks_background[i] = histopeaks[i]->ShowBackground(5);
      histopeaks[i]->Add(histopeaks_background[i],-1);
      histopeaks[i]->GetYaxis()->SetRangeUser(0,50000);
      histopeaks[i]->GetXaxis()->SetRangeUser(160,3000);
      histopeaks[i]->Smooth(20);
      TSpectrum *s = new TSpectrum(peakstofind);
      const int npeaks = s->Search(histopeaks[i],3,"",0.07);// for gammas labr3
      Float_t *xpeaks = s->GetPositionX();
      cout << "Channel: " << i << endl;

      //cout << "channel " << i <<": " <<xpeaks[0]<<"," << xpeaks[1]<<"," << xpeaks[2] << "," << xpeaks[3] << endl;

      TF1 *g0    = new TF1("g0","gaus",xpeaks[0]-25,xpeaks[0]+25); // 200 for alphas, 20 for gammas
      TF1 *g1    = new TF1("g1","gaus",xpeaks[1]-25,xpeaks[1]+25);
      TF1 *g2    = new TF1("g2","gaus",xpeaks[2]-25,xpeaks[2]+25);
      TF1 *g3    = new TF1("g3","gaus",xpeaks[3]-25,xpeaks[3]+25);
      TF1 *g4    = new TF1("g4","gaus",xpeaks[4]-25,xpeaks[4]+25);

      histopeaks[i]->Fit(g0,"QR+");
      histopeaks[i]->Fit(g1,"QR+");
      histopeaks[i]->Fit(g2,"QR+");
      histopeaks[i]->Fit(g3,"QR+");
      histopeaks[i]->Fit(g4,"QR+");

      g0->GetParameters(&par0[0]);
      g1->GetParameters(&par1[0]);
      g2->GetParameters(&par2[0]);
      g3->GetParameters(&par3[0]);
      g4->GetParameters(&par4[0]);

      centroids[0]=par0[1];
      centroids[1]=par1[1];
      centroids[2]=par2[1];
      centroids[3]=par3[1];
      centroids[4]=par4[1];

      TMath::Sort(5,centroids,s_centroids,kFALSE);

      for (size_t r = 0; r < 5; r++) {
        sorted_centroids[r]=centroids[s_centroids[r]];
      }


      TGraph* gr = new TGraph(5,sorted_centroids,energies);
      gr->Draw("AC*");

      TF1 *l1    = new TF1("l1","pol3",centroids[0]-10,centroids[3]+10);
      gr->Fit("l1","Q");
      // char c;
      // printf("Press enter");
      // c = getchar();
      l1->GetParameters(&parl1[0]);
      for (size_t u = 0; u < 5; u++) {
        cout << "peak number "<< u <<  " in: " << sorted_centroids[u]<< " energy of: " <<
        +parl1[0]
        +(pow(sorted_centroids[u],1)*parl1[1])
        +(pow(sorted_centroids[u],2)*parl1[2])
        +(pow(sorted_centroids[u],3)*parl1[3])
        +(pow(sorted_centroids[u],4)*parl1[4])
         << endl;

      }

      cout << i+64 << "\t" <<parl1[1] << "\t" << parl1[0] <<"\t" << parl1[2] <<"\t" << parl1[3] <<endl;
      myfiletxt<< i+64 << "\t" <<parl1[1] << "\t" << parl1[0] <<"\t" << parl1[2] <<"\t" << parl1[3] <<endl;
      //https://root.cern.ch/doc/master/classTH1.html#aff6520fdae026334bf34fa1800946790

    }

/*
root tchain.root
TCanvas * c2 = new TCanvas("c","c",1920,1080);
labr0_0->GetXaxis()->SetRangeUser(150,2500);
labr0_0->GetXaxis()->SetTitle("Channel");
labr0_0->GetYaxis()->SetTitle("Counts");
labr0_0->GetYaxis()->SetTitleOffset(1);
labr0_0->SetStats(0);
labr0_0->Draw();
labr0_0->SetTitle("Raw data: Dect 0");
c2->SaveAs("raw.pdf");

root PF_gammas.C

TCanvas * c3 = new TCanvas("c","c",1920,1080);
labr0_0->GetXaxis()->SetRangeUser(150,2500);
labr0_0->GetYaxis()->SetRangeUser(0,5000);
labr0_0->GetXaxis()->SetTitle("Channel");
labr0_0->GetYaxis()->SetTitle("Counts");
labr0_0->SetStats(0);
labr0_0->Draw();
labr0_0->SetTitle("4 peaks position, no-background: Dect 0");
c3->SaveAs("peaksfinal.pdf");

TCanvas * c1 = new TCanvas("c","c",1920,1080);
labr0_0_background->GetXaxis()->SetRangeUser(150,2500);
labr0_0_background->GetYaxis()->SetRangeUser(0,15000);
labr0_0_background->GetXaxis()->SetTitle("Channel");
labr0_0_background->GetYaxis()->SetTitle("Counts");
labr0_0_background->SetStats(0);
labr0_0_background->SetTitle("Calculated background: Dect 0");
labr0_0_background->GetXaxis()->CenterTitle();
labr0_0_background->Draw();
c1->SaveAs("background.pdf");


*/


 delete s;
 //delete rest;
 delete[] histopeaks;
}

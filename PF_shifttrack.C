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


void PF_shifttrack() {

string mystring;
  cout << "Enter the name of the ROOT run" << "\n";
//  cin >> mystring;
  mystring="tchain";
  string rootext(".root");
  string txtext("shift_table_noncorrected.txt");
  //string txtext("shift_table_corrected.txt");

  string RF = mystring+rootext;
  string TT = mystring+txtext;
  TFile *f = TFile::Open(RF.c_str(),"UPDATE");
  ofstream myfiletxt;
  myfiletxt.open (TT.c_str(),ios::out);
  f->Print();

  Double_t energies[6];

  energies[0]=0.511;
  energies[1]=2.2;
  energies[2]=3.416;
  energies[3]=3.927;
  energies[4]=4.438;
  energies[5]=6.129;

  TH1 *histopeaks[10][10];

  Double_t data511[10][10];
  Double_t data22[10][10];
  Double_t data34[10][10];
  Double_t data39[10][10];
  Double_t data44[10][10];
  Double_t data61[10][10];

  Double_t data511_er[10][10];
  Double_t data22_er[10][10];
  Double_t data34_er[10][10];
  Double_t data39_er[10][10];
  Double_t data44_er[10][10];
  Double_t data61_er[10][10];


for (Int_t k = 0; k < 10; k++) { //k is the detector index
  for(Int_t i=0;i<10; i++){ // i is the run index

      Double_t par0[3]={0};
      Double_t par1[3]={0};
      Double_t par2[3]={0};
      Double_t par3[3]={0};
      Double_t par4[3]={0};
      Double_t par5[3]={0};

      Double_t calibrationt[10]={0}; // this is for the energy fit for each run for each detector

      Double_t centroids[6]={0};
      Double_t centroids_er[6]={0};

      Int_t s_centroids[6];
      Int_t s_centroids_er[6];

      Double_t sorted_centroids[6];
      Double_t sorted_centroids_er[6];

      Double_t  datatime_er[10];
      Double_t  datatime[10];
      Double_t  grapharray511[10];
      Double_t  grapharray511_er[10];
      Double_t  grapharray22[10];
      Double_t  grapharray22_er[10];
      Double_t  grapharray44[10];
      Double_t  grapharray44_er[10];
      Double_t  grapharray61[10];
      Double_t  grapharray61_er[10];

      ostringstream name;
      name << "labrc"<< k <<  "_" << i; // corrected
      //name << "labrcpre"<< k <<  "_" << i; // NO corrected

      f->GetObject(name.str().c_str(),histopeaks[i][k]);
      // Look for the .511 peak
      histopeaks[i][k]->GetXaxis()->SetRangeUser(0.4,0.8);
      TSpectrum *w = new TSpectrum(1);
      const int npeaks = w->Search(histopeaks[i][k],1,"",0.3);// for gammas labr3
      Float_t *xpeaks511 = w->GetPositionX();
      TF1 *g511    = new TF1("g511","gaus",xpeaks511[0]-0.119,xpeaks511[0]+0.119);
      histopeaks[i][k]->Fit(g511,"QR+");
      g511->GetParameters(&par0[0]);

      //look for the 2.2 peak
      histopeaks[i][k]->GetXaxis()->SetRangeUser(2.0,2.4);
      TSpectrum *h = new TSpectrum(1);
      const int npeaks = h->Search(histopeaks[i][k],1,"",0.3);// for gammas labr3
      Float_t *xpeaks22 = h->GetPositionX();
      TF1 *g22    = new TF1("g22","gaus",xpeaks22[0]-0.09,xpeaks22[0]+0.09);
      histopeaks[i][k]->Fit(g22,"QR+");
      g22->GetParameters(&par1[0]);

      // look for the 4.4 and escape peaks
      histopeaks[i][k]->GetXaxis()->SetRangeUser(3.2,4.6);
      TSpectrum *s = new TSpectrum(3);
      const int npeaks = s->Search(histopeaks[i][k],2,"",0.07);// for gammas labr3
      Float_t *xpeaksC12 = s->GetPositionX();

      TF1 *g0    = new TF1("g0","gaus",xpeaksC12[0]-0.09,xpeaksC12[0]+0.09);
      TF1 *g1    = new TF1("g1","gaus",xpeaksC12[1]-0.09,xpeaksC12[1]+0.09);
      TF1 *g2    = new TF1("g2","gaus",xpeaksC12[2]-0.09,xpeaksC12[2]+0.09);

      histopeaks[i][k]->Fit(g0,"QR+");
      histopeaks[i][k]->Fit(g1,"QR+");
      histopeaks[i][k]->Fit(g2,"QR+");
      g0->GetParameters(&par2[0]);
      g1->GetParameters(&par3[0]);
      g2->GetParameters(&par4[0]);
    //look for the 6.129 peak
      histopeaks[i][k]->GetXaxis()->SetRangeUser(5.7,6.2);
      TSpectrum *q = new TSpectrum(1);
      const int npeaks = q->Search(histopeaks[i][k],1,"",0.3);// for gammas labr3
      Float_t *xpeaks61 = q->GetPositionX();
      TF1 *g61    = new TF1("g61","gaus",xpeaks61[0]-0.09,xpeaks61[0]+0.09);
      histopeaks[i][k]->Fit(g61,"QR+");
      g61->GetParameters(&par5[0]);

      centroids[0]=par0[1];
      centroids[1]=par1[1];
      centroids[2]=par2[1];
      centroids[3]=par3[1];
      centroids[4]=par4[1];
      centroids[5]=par5[1];

      centroids_er[0]=par0[2];
      centroids_er[1]=par1[2];
      centroids_er[2]=par2[2];
      centroids_er[3]=par3[2];
      centroids_er[4]=par4[2];
      centroids_er[5]=par5[2];

      TMath::Sort(6,centroids,s_centroids,kFALSE);
      //k is the detector index //i is the run
      for (size_t r = 0; r < 6; r++) {
        sorted_centroids[r]=centroids[s_centroids[r]];
        sorted_centroids_er[r]=centroids_er[s_centroids[r]];
      }
      histopeaks[i][k]->GetXaxis()->SetRangeUser(0,7);

      data511[k][i]=sorted_centroids[0];
      data22[k][i]=sorted_centroids[1];
      data34[k][i]=sorted_centroids[2];
      data39[k][i]=sorted_centroids[3];
      data44[k][i]=sorted_centroids[4];
      data61[k][i]=sorted_centroids[5];

      data511_er[k][i]=2.355*sorted_centroids_er[0]/2;
      data22_er[k][i]=2.355*sorted_centroids_er[1]/2;
      data34_er[k][i]=2.355*sorted_centroids_er[2]/2;
      data39_er[k][i]=2.355*sorted_centroids_er[3]/2;
      data44_er[k][i]=2.355*sorted_centroids_er[4]/2;
      data61_er[k][i]=2.355*sorted_centroids_er[5]/2;

      TGraph* gr = new TGraph(6,sorted_centroids,energies);
      //gr->Draw("AC*");
      //cout<< k <<","<< i << endl;
      TF1 *l1  = new TF1("l1","pol3",sorted_centroids[0]-1,sorted_centroids[3]+1);
      gr->Fit("l1","Q+");
      l1->GetParameters(&calibrationt[0]);
      // for (Int_t j = 0; j < 4; j++) {
      //   myfiletxt << k << "\t" << i << "\t"<< j <<"\t"<< calibrationt[j] << endl;
      //   cout << k << "\t" << i << "\t"<< j <<"\t"<< calibrationt[j] << endl;
      //
      // }

      //myfiletxt << k << "\t" << i << "\t"<< data511[k][i] <<"\t"<< data22[k][i] <<"\t" << data44[k][i] <<"\t" << data61[k][i]<< endl;

    }
}




// FOR DETECTOR 2
const int detector=2;
//K is the detector index //I is the run data44[k][i]
for (Int_t c = 0; c < 10; c++) {
datatime_er[c]=0;
datatime[c]=c;
grapharray511[c]=data511[detector][c];
grapharray511_er[c]=data511_er[detector][c];

grapharray22[c]=data22[detector][c];
grapharray22_er[c]=data22_er[detector][c];

grapharray44[c]=data44[detector][c];
grapharray44_er[c]=data44_er[detector][c];

grapharray61[c]=data61[detector][c];
grapharray61_er[c]=data61_er[detector][c];
}


TCanvas * c0 = new TCanvas("c","c",1920,1080);
TGraphErrors* g511c_er = new TGraphErrors(10, datatime, grapharray511, datatime_er, grapharray511_er);
//g511c_er->SetTitle("Gain shift in detector 2 for the 0.511 MeV peak");
g511c_er->SetTitle("Corrected gain shift in detector 2 for the 0.511 MeV peak");
g511c_er->GetXaxis()->SetTitle("Time arbitrary units");
g511c_er->GetYaxis()->SetTitle("Energy MeV");
g511c_er->GetXaxis()->SetRangeUser(-3,11);
g511c_er->SetMarkerColor(4);
g511c_er->SetMarkerSize(3);
g511c_er->SetMarkerStyle(20);
gPad->SetGrid(1, 1); gPad->Update();
g511c_er->Draw("ACP");
//c0->SaveAs("511shift.pdf");
c0->SaveAs("511shiftcorrected.pdf");

TCanvas * c1 = new TCanvas("c","c",1920,1080);
TGraphErrors* g22c_er = new TGraphErrors(10, datatime, grapharray22, datatime_er, grapharray22_er);
//g22c_er->SetTitle("Gain shift in detector 2 for the 2.2 MeV peak");
g22c_er->SetTitle("Corrected gain shift in detector 2 for the 2.2 MeV peak");
g22c_er->GetXaxis()->SetTitle("Time arbitrary units");
g22c_er->GetYaxis()->SetTitle("Energy MeV");
g22c_er->GetXaxis()->SetRangeUser(-3,11);
g22c_er->SetMarkerColor(4);
g22c_er->SetMarkerSize(3);
g22c_er->SetMarkerStyle(21);
gPad->SetGrid(1, 1); gPad->Update();
g22c_er->Draw("ACP");
//c1->SaveAs("22shift.pdf");
c1->SaveAs("22shiftcorrected.pdf");

TCanvas * c2 = new TCanvas("c","c",1920,1080);
TGraphErrors* g44c_er = new TGraphErrors(10, datatime, grapharray44, datatime_er, grapharray44_er);
//g44c_er->SetTitle("Gain shift in detector 2 for the 4.4 MeV peak");
g44c_er->SetTitle("Corrected gain shift in detector 2 for the 4.4 MeV peak");
g44c_er->GetXaxis()->SetTitle("Time arbitrary units");
g44c_er->GetYaxis()->SetTitle("Energy MeV");
g44c_er->GetXaxis()->SetRangeUser(-3,11);
g44c_er->SetMarkerColor(4);
g44c_er->SetMarkerSize(3);
g44c_er->SetMarkerStyle(34);
gPad->SetGrid(1, 1); gPad->Update();
g44c_er->Draw("ACP");
//c2->SaveAs("44shift.pdf");
c2->SaveAs("44shiftcorrected.pdf");

TCanvas * c3 = new TCanvas("c","c",1920,1080);
TGraphErrors* g61c_er = new TGraphErrors(10, datatime, grapharray61, datatime_er, grapharray61_er);
//g61c_er->SetTitle("Gain shift in detector 2 for the 6.1 MeV peak");
g61c_er->SetTitle("Corrected gain shift in detector 2 for the 6.1 MeV peak");
g61c_er->GetXaxis()->SetTitle("Time arbitrary units");
g61c_er->GetYaxis()->SetTitle("Energy MeV");
g61c_er->GetXaxis()->SetRangeUser(-3,11);
g61c_er->SetMarkerColor(4);
g61c_er->SetMarkerSize(3);
g61c_er->SetMarkerStyle(29);
gPad->SetGrid(1, 1); gPad->Update();
g61c_er->Draw("ACP");
//c3->SaveAs("61shift.pdf");
c3->SaveAs("61shiftcorrected.pdf");


 delete s;
 delete[] histopeaks;
}

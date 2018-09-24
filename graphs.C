
  mystring="tchain";
  string rootext(".root");
  TFile *f = TFile::Open(RF.c_str(),"UPDATE");
  f->Print();

  TCanvas * c2 = new TCanvas("c","c",1920,1080);
  labr0_0->GetXaxis()->SetRangeUser(150,2500);
  labr0_0->GetXaxis()->SetTitle("Channel");
  labr0_0->GetYaxis()->SetTitle("Counts");
  labr0_0->GetYaxis()->SetTitleOffset(1.2);
  labr0_0->CenterTitle();
  labr0_0->SetStats(0);
  labr0_0->Draw();
  labr0_0->SetTitle("Raw data: Detector 0");
  c2->SaveAs("raw.pdf");

  // after running PF_gammas.C
  TCanvas * c3 = new TCanvas("c","c",1920,1080);
  labr0_0->GetXaxis()->SetRangeUser(150,2500);
  labr0_0->GetYaxis()->SetRangeUser(0,11000);
  labr0_0->GetXaxis()->SetTitle("Channel");
  labr0_0->GetYaxis()->SetTitleOffset(1.2);
  labr0_0->GetYaxis()->SetTitle("Counts");
  labr0_0->SetStats(0);
  labr0_0->Draw();
  labr0_0->SetTitle("5 peaks position, no-background: Detector 0");
  c3->SaveAs("peaksfinal.pdf");

  TCanvas * c1 = new TCanvas("c","c",1920,1080);
  labr0_0_background->GetXaxis()->SetRangeUser(150,2500);
  labr0_0_background->GetYaxis()->SetRangeUser(0,15000);
  labr0_0_background->GetXaxis()->SetTitle("Channel");
  labr0_0_background->GetYaxis()->SetTitleOffset(1.2);
  labr0_0_background->GetYaxis()->SetTitle("Counts");
  labr0_0_background->SetStats(0);
  labr0_0_background->SetTitle("Calculated background: Dect 0");
  labr0_0_background->GetXaxis()->CenterTitle();
  labr0_0_background->Draw();
  c1->SaveAs("background.pdf");

  /// second calibrationt

  TCanvas * c = new TCanvas("c","c",1920,1080);
  labrcpre9_9->GetXaxis()->SetRangeUser(0.2,6.5);
  labrcpre9_9->GetYaxis()->SetRangeUser(100,1e5);
  labrcpre9_9->GetXaxis()->SetTitle("Energy: MeV");
  labrcpre9_9->GetYaxis()->SetTitleOffset(1.2);
  labrcpre9_9->GetYaxis()->SetTitle("Counts");
  labrcpre9_9->SetStats(0);
  labrcpre9_9->Draw();
  labrcpre9_0->Draw("same");
  labrcpre9_0->SetLineColor(2);
  labrcpre9_9->SetLineColor(4);
  labrcpre9_9->SetTitle("Shift Gain in detector 9");
  c->SetLogy();
  c->SaveAs("timeshift.pdf");

  TCanvas * c = new TCanvas("c","c",1920,1080);
  labrc9_9->GetXaxis()->SetRangeUser(0.2,6.5);
  labrc9_9->GetYaxis()->SetRangeUser(100,1e5);
  labrc9_9->GetXaxis()->SetTitle("Energy: MeV");
  labrc9_9->GetYaxis()->SetTitleOffset(1.2);
  labrc9_9->GetYaxis()->SetTitle("Counts");
  labrc9_9->SetStats(0);
  labrc9_9->Draw();
  labrc9_0->Draw("same");
  labrc9_0->SetLineColor(2);
  labrc9_9->SetLineColor(4);
  labrc9_9->SetTitle("Shift Gain in detector 9 after correction");
  c->SetLogy();
  c->SaveAs("timeshiftcorrected.pdf");

////  emergy dssd///////


  TCanvas * c2 = new TCanvas("c","c",1920,1080);
  alphaenergylGATE->GetXaxis()->SetRangeUser(0,8);
  alphaenergylGATE->GetXaxis()->SetTitle("Energy 0.08 MeV/bin");
  alphaenergylGATE->GetYaxis()->SetTitle("Counts");
  alphaenergylGATE->GetYaxis()->SetTitleOffset(1.2);
  alphaenergylGATE->CenterTitle();
  alphaenergylGATE->SetStats(0);
  alphaenergylGATE->Draw();
  alphaenergylGATE->SetTitle("Gated energy distribution in left DSSD");
  c2->SaveAs("leftdssdenergygated.pdf");


  TCanvas * c3 = new TCanvas("c","c",1920,1080);
  alphaenergyl->GetXaxis()->SetRangeUser(0,8);
  alphaenergyl->GetXaxis()->SetTitle("Energy 0.08 MeV/bin");
  alphaenergyl->GetYaxis()->SetTitle("Counts");
  alphaenergyl->GetYaxis()->SetTitleOffset(1.2);
  alphaenergyl->CenterTitle();
  alphaenergyl->SetStats(0);
  alphaenergyl->Draw();
  alphaenergyl->SetTitle("Energy distribution in left DSSD");
  c3->SaveAs("leftdssdenergy.pdf");

//// labr3///////////////buenazoa
TCanvas * c4 = new TCanvas("c","c",1920,1080);
labrsingleGATE->GetXaxis()->SetRangeUser(0,8);
labrsingleGATE->GetXaxis()->SetTitle("Energy 0.04 MeV/bin");
labrsingleGATE->GetYaxis()->SetTitle("Counts");
labrsingleGATE->GetYaxis()->SetTitleOffset(1.2);
labrsingleGATE->CenterTitle();
labrsingleGATE->SetStats(0);
labrsingleGATE->Draw();
labrsingleGATE->SetTitle("Gated energy distribution the LaBr3 array");
c4->SaveAs("labr3energygated.pdf");

TCanvas * c5 = new TCanvas("c","c",1920,1080);
labrsingle->GetXaxis()->SetRangeUser(0,8);
labrsingle->GetXaxis()->SetTitle("Energy 0.04 MeV/bin");
labrsingle->GetYaxis()->SetTitle("Counts");
labrsingle->GetYaxis()->SetTitleOffset(1.2);
labrsingle->CenterTitle();
labrsingle->SetStats(0);
labrsingle->Draw();
labrsingle->SetTitle("Energy distribution the LaBr3 array");
c5->SaveAs("labr3energy.pdf");

// total
TCanvas * c6 = new TCanvas("c","c",1920,1080);
labrt->GetXaxis()->SetRangeUser(0,8);
labrt->GetXaxis()->SetTitle("Energy 0.04 MeV/bin");
labrt->GetYaxis()->SetTitle("Counts");
labrt->GetYaxis()->SetTitleOffset(1.2);
labrt->CenterTitle();
labrt->SetStats(0);
labrt->Draw();
labrsingle->SetLineColor(2);
labrsingle->Draw("SAME");
labrt->SetTitle("Energy distribution the LaBr3 array");
c6->SaveAs("labr3energytotal.pdf");
//// labr3 ZOOM

TCanvas * c4 = new TCanvas("c","c",1920,1080);
labrsingleGATE->GetXaxis()->SetRangeUser(4.58,6.5);
labrsingleGATE->GetXaxis()->SetTitle("Energy 0.04 MeV/bin");
labrsingleGATE->GetYaxis()->SetTitle("Counts");
labrsingleGATE->GetYaxis()->SetTitleOffset(1.2);
labrsingleGATE->CenterTitle();
labrsingleGATE->SetStats(0);
labrsingleGATE->Draw();
labrsingleGATE->SetTitle("Gated energy distribution the LaBr3 array zoomed");
TH1 *filledgate = (TH1*)labrsingleGATE->Clone("filledgate");
filledgate->GetXaxis()->SetRangeUser(5.139,5.699);
filledgate->SetFillColor(2);
filledgate->Draw("same");
c4->SaveAs("labr3energygatedZOOM.pdf");

TCanvas * c5 = new TCanvas("c","c",1920,1080);
labrsingle->GetXaxis()->SetRangeUser(5.2,6.5);
labrsingle->GetXaxis()->SetTitle("Energy 0.04 MeV/bin");
labrsingle->GetYaxis()->SetTitle("Counts");
labrsingle->GetYaxis()->SetTitleOffset(1.2);
labrsingle->CenterTitle();
labrsingle->SetStats(0);
labrsingle->Draw();
labrsingle->SetTitle("Energy distribution the LaBr3 array zoomed");
TH1 *filled = (TH1*)labrsingle->Clone("filledgate");
filled->GetXaxis()->SetRangeUser(5.139,5.699);
filled->SetFillColor(2);
filled->Draw("same");
c5->SaveAs("labr3energyZOOM.pdf");



/// graphs for TAC and DSSD maps

TCanvas *c1 = new TCanvas("c1","multipads",1920,1080);
   gStyle->SetOptStat(0);
   c1->Divide(2,1);
   c1->cd(1);
   mapleft->SetTitle("DSSD 1");
   mapleft->GetXaxis()->SetRangeUser(1.8,2.9);
   mapleft->GetXaxis()->SetTitle("Angle X(Rad)");
   mapleft->GetYaxis()->SetTitle("Angle Y(Rad)");
   mapleft->GetYaxis()->SetTitleOffset(1.2);
   mapleft->Draw("colz");


   c1->cd(2);
   mapright->SetTitle("DSSD 2");
   mapright->GetXaxis()->SetRangeUser(1.8,2.9);
   mapright->GetXaxis()->SetTitle("Angle X (Rad)");
   mapright->GetYaxis()->SetTitleOffset(1.2);
   mapright->GetYaxis()->SetTitle("Angle Y (Rad)");
   mapright->Draw("colz");

   tac->SetTitle("TAC spectra");
   tac->GetYaxis()->SetTitle("Counts");
   tac->GetXaxis()->SetTitle("Channels");
   tac->SetStats(0);
   tac->GetXaxis()->SetRangeUser(0,4000);
   tac->Draw();
   TH1F *tacc = (TH1F*)tac->Clone();
   tacc->SetFillColor(46);
   tacc->GetXaxis()->SetRange(630,750);
   TCanvas *c1 = new TCanvas("c1","multipads",1920,1080);
   tac->Draw();
   tacc->Draw("same");
   c1->SaveAs("tac.pdf");

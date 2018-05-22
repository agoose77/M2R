
mapright->Draw("colz")
mapleft->Draw("colz")
E1vsE2->Draw("colz")
Be8->Draw()
alphatheta->Draw("colz")

Labr3_9final->Draw()
Labr3_9final->ShowBackground()
TH1F h = *Labr3_9final - *Labr3_9final_background
TH1F *rest = &h
rest->GetXaxis()->SetRangeUser(150,2500)
rest->GetYaxis()->SetRangeUser(0,10000)
TSpectrum *s = new TSpectrum(4)
Int_t npeaks = s->Search(rest,2,"",0.05)


  #include <stdlib.h>     // srand, rand
  Float_t div = RAND_MAX
  Int_t xl= 8
  Int_t a = rand() % 2
  Float_t xli = (3*(xl+((-1**a)*rand()/div)))-24.75
  Float_t x = xl+(  (-1**a)*rand()/div)

void histo2ascciinb (TH1* h)
{
   Int_t n = h->GetNbinsX();
   for (Int_t i=1; i<=n; i++) {
      printf("%g\n",h->GetBinContent(i));
   }
}

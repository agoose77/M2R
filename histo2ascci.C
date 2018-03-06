void histo2ascci (TH1* h)
{
   Int_t n = h->GetNbinsX();
   for (Int_t i=1; i<=n; i++) {
      printf("%g, %g\n",
             int(h->GetBinLowEdge(i)+h->GetBinWidth(i)/2),
             h->GetBinContent(i));
   }
}

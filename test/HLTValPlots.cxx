run(string fname1,string fname2,string v1,string v2,bool hasL3)
{
  L2Val(fname1,fname2,v1,v2,"DQMData/L2");
  L25Val(fname1,fname2,v1,v2,"DQMData/L25");
  if(hasL3)
  L3Val(fname1,fname2,v1,v2,"DQMData/L3");



}


L2Val(string fname1,string fname2,string v1,string v2,string folder)
{

  GetHistos(fname1,fname2,v1,v2,folder,"L2tauCandEt","L2 jet E_{t}","n. #tau cands."," L2 jet E_{t}",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2tauCandEta","L2 jet #eta","n. #tau cands."," L2 jet #eta",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2ecalIsolEt","L2 ECAL Isol. E_{t}","n. #tau cands.","ECAL #Sigma E_{t} in annulus",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2towerIsolEt","L2 Tower isol E_{t}","n. #tau cands.","Tower #Sigma E_{t} in annulus",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2nClusters","L2 Number of  Clusters","n. #tau cands.","Number of clusters",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2clusterEtaRMS","L2 Cluster #eta RMS","n. #tau cands.","Cluster #eta RMS",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2clusterPhiRMS","L2 Cluster #phi RMS","n. #tau cands.","Cluster #phi RMS",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2clusterDeltaRRMS","L2 Cluster #DeltaR RMS","n. #tau cands.","Cluster #DeltaR RMS",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"MET","MET [GeV]","n. events","Missing E_{t}",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L2EtEff","L2 Reco Jet E_{t} [GeV]","Efficiency","L2 Efficiency vs E_{t}",0,true );




}

L25Val(string fname1,string fname2,string v1,string v2,string folder)
{

  GetHistos(fname1,fname2,v1,v2,folder,"L25jetEt","L25 Jet E_{t} [GeV]","n. #tau cands.","L25 Jet E_{t} [GeV]",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L25jetEta","L25 Jet #eta","n. #tau cands.","L25 Jet #eta",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L25EtEff","L25 Reco Jet E_{t} [GeV]","Efficiency","L25 Efficiency vs E_{t}",0,true );
  GetHistos(fname1,fname2,v1,v2,folder,"L25EtaEff","L25 Efficiency vs #eta","n. #tau cands.","Reco Jet #eta",0,true );
  GetHistos(fname1,fname2,v1,v2,folder,"L25nPxlTrksInJet","L25 Number of Pixel Tracks","n. #tau cands.","# Pixel Tracks in L25 Jet",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L25nQPxlTrksInJet","L25 Number of Q Pixel Tracks","n. #tau cands.","# Quality Pixel Tracks in L25 Jet",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L25signalLeadTrkPt","L25 Lead Track P_{t}[GeV]","n. #tau cands.","Lead Track P_{t} [GeV]",0,false );
  

}

L3Val(string fname1,string fname2,string v1,string v2,string folder)
{
  GetHistos(fname1,fname2,v1,v2,folder,"L3jetEt","L3 Jet E_{t} [GeV]","n. #tau cands.","L3 Jet E_{t} [GeV]",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L3jetEta","L3 Jet #eta","n. #tau cands.","L3 Jet #eta",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L3EtEff","L3 Jet E_{t} [GeV]","Efficiency","L3 Efficiency vs E_{t}",0,true );
  GetHistos(fname1,fname2,v1,v2,folder,"L3EtaEff","L3 Efficiency vs #eta","n. #tau cands.","Reco Jet #eta",0,true );
  GetHistos(fname1,fname2,v1,v2,folder,"L3nPxlTrksInJet","L3 Number of  Silicon Tracks","n. #tau cands.","# Si Tracks in L3 Jet",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L3nQPxlTrksInJet","L3 Number of Quality Si Tracks","n. #tau cands.","# Quality Si. Tracks in L3 Jet",0,false );
  GetHistos(fname1,fname2,v1,v2,folder,"L3signalLeadTrkPt","L3 Lead Track P_{t}[GeV]","n. #tau cands.","Lead Track P_{t} [GeV]",0,false );
 
}


GetHistos(string f1,string f2,string v1,string v2,string modName,string histo,char* xlabel = "",char *ylabel = "",char *title, double scale = 1,bool eff)
{
  TCanvas *c = new TCanvas;
  c->cd();
  TFile *f = new TFile(f1.c_str());
  TFile *ff = new TFile(f2.c_str());
  
  TH1F *h = ((TH1F*)f->Get((modName+"/"+histo).c_str()))->Clone();
  TH1F *hh = ((TH1F*)ff->Get((modName+"/"+histo).c_str()))->Clone();

  if(!eff)
    h->Sumw2();

  h->GetXaxis()->SetLabelSize(0.06);
  h->GetXaxis()->SetNdivisions(509);
  h->GetYaxis()->SetNdivisions(509);
  h->GetYaxis()->SetLabelSize(0.06);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleOffset(1.25);

   h->SetMarkerColor(kYellow);
  h->SetMarkerStyle(20);
  h->SetFillColor(kYellow);
  h->GetXaxis()->SetTitle(xlabel);
  h->GetYaxis()->SetTitle(ylabel);
  h->SetTitle(title);
  if(!eff)
    {
      if(scale==0) h->Scale(1/h->Integral());
      else
	h->Scale(scale);
      hh->Sumw2();
    }
  hh->GetXaxis()->SetLabelSize(0.06);
  hh->GetYaxis()->SetLabelSize(0.06);
  hh->GetXaxis()->SetTitleSize(0.06);
  hh->GetYaxis()->SetTitleSize(0.06);
  hh->SetLineColor(kRed);
  hh->SetMarkerColor(kRed);
  hh->SetMarkerStyle(20);
  hh->GetXaxis()->SetTitle(xlabel);
  hh->GetYaxis()->SetTitle(ylabel);
  hh->SetTitle(title);
  if(!eff)
    {
      if(scale==0) hh->Scale(1/hh->Integral());
      else
	hh->Scale(scale);
    }

  h->Draw("HIST");
  hh->Draw("SAME");
  TLegend *l = new TLegend(0.7,0.5,0.9,0.7);
  l->AddEntry(h,v1.c_str());
  l->AddEntry(hh,v2.c_str());
  l->Draw();

  c->SaveAs((histo+".gif").c_str());
  delete c;
   f->Close();
   ff->Close();

}





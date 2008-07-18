run(string fname,string ref = "MC")
{
  TCanvas *l2 = new TCanvas("l2","L@ Validation");
  L2Val(fname,"DQMData/L2ValidationRefTo"+ref,kBlue,"HIST",l2);
  TCanvas *l25 = new TCanvas("l25","l25 Validation");
  L25Val(fname,"DQMData/L25ValidationRefTo"+ref,kBlue,"HIST",l25);


}
overlay(string fname,string ref = "MC",Color_t col)
{
  L2Val(fname,"DQMData/L2ValidationRefTo"+ref,col,"SAME",l2);
  L25Val(fname,"DQMData/L25ValidationRefTo"+ref,col,"SAME",l25);
}



L2Val(string fname,string folder,Color_t color,char* opt = "",TCanvas *c)
{
  if(opt ==""||opt =="HIST")
    c->Divide(3,3);

  c->cd(1);
  GetHisto(fname, folder,"ecalIsolEt",color,opt,"ECAL Isol E_{t}");
  c->cd(2);
  GetHisto(fname, folder,"nClusters",color,opt,"N Clusters");
  c->cd(3);
  GetHisto(fname, folder,"clusterEtaRMS",color,opt,"Cluster #eta RMS");
  c->cd(4);
  GetHisto(fname, folder,"clusterPhiRMS",color,opt,"Cluster #phi RMS");
  c->cd(5);
  GetHisto(fname, folder,"clusterDeltaRRMS",color,opt,"Cluster #DeltaR RMS");
  c->cd(6);
  GetEffHisto(fname, folder,"EtEffNum","EtEffDenom",color,opt,"L2 Jet E_{t}","Efficiency");
  c->cd(7);
  GetHisto(fname, folder,"MET",color,opt,"Missing E_{t} [GeV]");

  //  c->cd(8);
  // GetHisto(fname, folder,"towerIsolEt",color,opt,"Tower Isol E_{t}");
  //c->cd(9);
  //GetHisto(fname, folder,"seedTowerEt",color,opt,"Seed Tower  E_{t}");
 


}

L25Val(string fname,string folder,Color_t color,char* opt = "",TCanvas *c)
{
  if(opt ==""||opt=="HIST")
    c->Divide(2,3);
  c->cd(1);
  GetHisto(fname,folder,"nPxlTrksInL25Jet",color,opt,"Number of Pixel Tracks");
  c->cd(2);
  GetHisto(fname,folder,"nQPxlTrksInL25Jet",color,opt,"Number of Quality Pixel Tracks");
  c->cd(3);
  GetHisto(fname,folder,"signalLeadTrkPt",color,opt,"Lead Track P_{t}");
  c->cd(4);
  GetEffHisto(fname, folder,"l25IsoJetEt","jetEt",color,opt,"L2 Jet E_{t}","Efficiency");
  c->cd(5);
  GetEffHisto(fname, folder,"l25IsoJetEta","jetEta",color,opt,"L25 Jet #eta","Efficiency");




}







GetHisto(string fName,string modName,string histo,Color_t c = kBlue,char* opt ="",char* xlabel = "",char *ylabel = "", double scale = 0)
{
  TFile *f = new TFile(fName.c_str());
  
  TH1F *h = ((TH1F*)f->Get((modName+"/"+histo).c_str()))->Clone();

  h->Sumw2();
  h->SetLineWidth(2);
  h->GetXaxis()->SetLabelSize(0.06);
  h->GetYaxis()->SetLabelSize(0.06);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);

  h->SetLineColor(c);
  h->SetMarkerColor(c);
  h->SetMarkerStyle(20);

  h->SetFillStyle(3001);
  h->SetFillColor(c);
  h->GetXaxis()->SetTitle(xlabel);
  h->GetYaxis()->SetTitle(ylabel);
  if(scale==0) h->Scale(1/h->Integral());
  else
  h->Scale(scale);
  
  h->Draw(opt);
}




GetEffHisto(string fName,string modName,string histo1,string histo2,Color_t c = kBlue,char* opt ="",char* xlabel = "",char *ylabel = "")
{
  TFile *f = new TFile(fName.c_str());
  
  TH1F *h1 = (TH1F*)f->Get((modName+"/"+histo1).c_str());
  TH1F *h2 = (TH1F*)f->Get((modName+"/"+histo2).c_str());

  TH1F *h =h1->Clone();
  TH1F *h11 = h1->Clone();
  TH1F *h21 = h2->Clone();

  h11->Sumw2();
  h21->Sumw2();


  h->Divide(h11,h21,1.,1.,"b");


  h->GetXaxis()->SetLabelSize(0.06);
  h->GetYaxis()->SetLabelSize(0.06);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  
  h->SetLineColor(kBlack);
  h->SetLineWidth(2);
  h->SetMarkerColor(c);
  h->SetMarkerStyle(20);

    //  h->SetFillStyle(3001);
    // h->SetFillColor(c);
  h->GetXaxis()->SetTitle(xlabel);
  h->GetYaxis()->SetTitle(ylabel);


  if(opt == "HIST") opt ="";
  h->Draw(opt);
}


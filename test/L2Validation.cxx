class L2Validation
{
public:
  L2Validation(std::string);

  ~L2Validation();

void  makePlots(); 
void  weighbyCS(double,int);
void Write(std::string);
double Efficiency(double, double,double,int,double,double,double,double);//Optimize Trigger Cuts



  
private:
  TFile *f;
  TTree *t;
  float jetet,et,eta,drRMS,etaRMS,phiRMS,ecalEt,towerEt,seedEt;
 int matched,l1matched,nclusters;
TH1F* HistDivide(TH1F*,TH1F*);//Define the Binomial Error in the efficiency 
//Reminder: The binomial error is correct only when we have a lot of events...
//In low stat  samples you need a different way 

//Declare Histograms



  TH1F *ecal_isol_et,*tower_isol_et,*cluster_n,*cluster_eta_rms,*cluster_phi_rms,*cluster_dr_rms,*seed_tower_et,*jet_et,*et_eff;
 

};

////////////////////////////////////////////////////////////////////////


L2Validation::L2Validation(std::string fileName)
{
  //Initialize
  f = new TFile(fileName.c_str());
  t = (TTree*)f->Get("l2tree");

  et=0.;
  eta=0.;
  jetet=0.;
  drRMS=0.;
  etaRMS=0.;
  phiRMS=0.;
  ecalEt=0.;
  towerEt=0.;
  seedEt=0.;
  matched=0;
  l1matched=0;
  nclusters=0;
 
  //Declare Branches

  t->SetBranchAddress("MCet",&et);
  t->SetBranchAddress("MCeta",&eta);
  t->SetBranchAddress("Jet_Et",&jetet);
  t->SetBranchAddress("MCMatched",&matched);
  t->SetBranchAddress("L1Matched",&l1matched);
  t->SetBranchAddress("cl_Nclusters",&nclusters);
  t->SetBranchAddress("cl_drRMS",&drRMS);
  t->SetBranchAddress("cl_etaRMS",&etaRMS);
  t->SetBranchAddress("cl_phiRMS",&phiRMS);
  t->SetBranchAddress("ecalIsol_Et",&ecalEt);
  t->SetBranchAddress("towerIsol_Et",&towerEt);
  t->SetBranchAddress("seedTower_Et",&seedEt);

  //Setup Histograms
  
  ecal_isol_et    = new TH1F("ecal_isol_et","ECAL Isolation E_{t}",20,0.,20.);
  ecal_isol_et->GetYaxis()->SetTitle("norm. L2 tau cands / bin");
  ecal_isol_et->GetXaxis()->SetTitle("Isolation E_{t} [GeV]");

  tower_isol_et   = new TH1F("tower_isol_et","CaloTower Isolation E_{t}",20,0.,20.);
  tower_isol_et->GetYaxis()->SetTitle("norm. L2 tau cands / bin");
  tower_isol_et->GetXaxis()->SetTitle("Isolation E_{t} [GeV]");

  cluster_n       = new TH1F("cluster_n","Clustering-Number of Clusters",15,0,15);
  cluster_n->GetYaxis()->SetTitle("norm. L2 tau cands / bin");
  cluster_n->GetXaxis()->SetTitle("# of Clusters");

  cluster_eta_rms = new TH1F("cluster_eta_rms","Clustering #eta RMS",25,0,0.5);
  cluster_eta_rms->GetXaxis()->SetTitle("Cluster #eta RMS");
  cluster_eta_rms->GetYaxis()->SetTitle("norm. L2 tau cands / bin");

  cluster_phi_rms = new TH1F("cluster_phi_rms","Clustering #phi RMS",25,0,0.5);
  cluster_phi_rms->GetYaxis()->SetTitle("norm. L2 tau cands / bin");
  cluster_phi_rms->GetXaxis()->SetTitle("Cluster #phi RMS");
 
  cluster_dr_rms  = new TH1F("cluster_dr_rms","Clustering #delta R RMS",25,0,0.5);
  cluster_dr_rms->GetYaxis()->SetTitle("norm. L2 tau cands / bin");
  cluster_dr_rms->GetXaxis()->SetTitle("Cluster #delta R RMS");

  seed_tower_et  = new TH1F("seed_tower_et","Seed Tower E_{t}",25,0,100);
  seed_tower_et->GetYaxis()->SetTitle("norm. L2 tau cands / bin");
  seed_tower_et->GetXaxis()->SetTitle("Seed Tower E_{t} [GeV]");
 
  jet_et  = new TH1F("jet_et","#tau Candidate E_{t}",25,0,100);
  jet_et->GetYaxis()->SetTitle("norm. L2 tau cands / bin");
  jet_et->GetXaxis()->SetTitle("E_{t} [GeV]");

  et_eff  = new TH1F("et_eff","Efficiency vs E_{t}",50,0,100);
  


}

L2Validation::~L2Validation()
{
  f->Close();
}


void 
L2Validation::makePlots()
{
  TCanvas *c = new TCanvas("valplots","L2 Validation Plots");
  c->Divide(3,3);
  c->cd(1);
  t->Draw("ecalIsol_Et >>ecal_isol_et","MCMatched==1"); 
 c->cd(2);
 t->Draw("towerIsol_Et >>tower_isol_et","MCMatched==1"); 
 c->cd(3);
 t->Draw("cl_Nclusters >>cluster_n","MCMatched==1"); 
 c->cd(4);
 t->Draw("cl_etaRMS >>cluster_eta_rms","MCMatched==1"); 
 c->cd(5);
 t->Draw("cl_phiRMS >>cluster_phi_rms","MCMatched==1"); 
 c->cd(6);
 t->Draw("cl_drRMS  >>cluster_dr_rms","MCMatched==1");
 c->cd(7);
 t->Draw("seedTower_Et >>seed_tower_et","MCMatched==1");
 c->cd(8);
 t->Draw("Jet_Et >>jet_et","MCMatched==1");
  
}


void 
L2Validation::Write(std::string fName)
{
  TFile *f2 = new TFile(fName.c_str(),"recreate");
  ecal_isol_et->Write();
  tower_isol_et->Write();
  cluster_n->Write();
  cluster_eta_rms->Write();
  cluster_phi_rms->Write();
  cluster_dr_rms->Write();
  seed_tower_et->Write();
  jet_et->Write();
  et_eff->Write();
  f2->Close();

} 




TH1F* L2Validation::HistDivide(TH1F* hist1,TH1F* hist2)
{
  hist1->Sumw2();
  hist2->Sumw2();

  TH1F *out = hist1->Clone();

  float e1,e2,eff,e;

  for(int i =0;i<hist1->GetNbinsX();++i)
    {
     
      e1=hist1->GetBinContent(i);
      e2=hist2->GetBinContent(i);
      if(e2>0)
	{
	  eff=e1/e2;
	  e=sqrt(eff*(1-eff)/e2);
	}
      else
	{
	  eff=0;
	}
      if(eff>1)
	{
	  eff=0;
	  e=0;
	}  
      out->SetBinContent(i,eff);
      out->SetBinError(i,e);

    }

  out->SetLineWidth(1);
  out->SetMarkerStyle(20);
  out->GetYaxis()->SetRangeUser(0,1.05);

  return out;

}


double 
L2Validation::Efficiency(double JET_ET, double ECAL_ET,double TOWER_ET,int NCLUSTERS ,double CLUSTER_ETA_RMS,double CLUSTER_PHI_RMS,double CLUSTER_DR_RMS ,double SEED_TOWER_ET)
{
   
  et_eff->GetXaxis()->SetTitle("E_{t} [GeV]");

  TH1F * tmpet =new TH1F("tmpet","",50,0,100);

  int matched_only=0;
  int matched_passed=0;
  int l1reco_matched=0;

  for(int i =0;i< t->GetEntries();++i)
    {
      t->GetEntry(i);
       if(matched==1)
	{
	  matched_only++;
	  tmpet->Fill(et);
	 	  
	  if(l1matched==1) 
	    l1reco_matched++;

	  if(et>JET_ET)  
	    if(ecalEt<=ECAL_ET)
	      if(towerEt<=TOWER_ET)
		if(nclusters<=NCLUSTERS)
		  if(etaRMS<=CLUSTER_ETA_RMS)
		    if(phiRMS<=CLUSTER_PHI_RMS)
		      if(drRMS<=CLUSTER_DR_RMS)
			if(seedEt>SEED_TOWER_ET)
			  {
			    et_eff->Fill(et);
			    matched_passed++;
			  }
	    
	}
    }

 
  TCanvas *opt = new TCanvas("opt","Optimization");
   et_eff = HistDivide(et_eff,tmpet);
   et_eff->Draw();
 
   double eff=0;
   double l2reff=0;
   
  if(matched_only>0)
    {
      eff= ((double)(matched_passed))/((double)matched_only);
      l2reff= ((double)(l1reco_matched))/((double)matched_only);
 
    }
  

  

  printf ("L2 Jets Reconstruction Efficiency with ref to L1,Match %f\n",l2reff);
  printf ("L2 Trigger Efficiency w/ref to L1  = %f \n",eff);

  return eff;

}


void 
L2Validation::weighbyCS(double cs,int nevents)
{
  
  ecal_isol_et->Sumw2();
  ecal_isol_et->Scale(cs/((double)nevents));
  tower_isol_et->Sumw2();
  tower_isol_et->Scale(cs/((double)nevents));
  cluster_n->Sumw2();
  cluster_n->Scale(cs/((double)nevents));
  cluster_eta_rms->Sumw2();
  cluster_eta_rms->Scale(cs/((double)nevents));
  cluster_phi_rms->Sumw2();
  cluster_phi_rms->Scale(cs/((double)nevents));
  cluster_dr_rms->Sumw2();
  cluster_dr_rms->Scale(cs/((double)nevents));
  seed_tower_et->Sumw2();
  seed_tower_et->Scale(cs/((double)nevents));
  jet_et->Sumw2();
  jet_et->Scale(cs/((double)nevents));
  

}



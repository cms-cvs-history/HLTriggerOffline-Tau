class L2Validation
{
public:
  L2Validation(std::string);

  ~L2Validation();

void  makePlots(); 
void Write(std::string);
double optimize(double,double,int,double,double,double);//Optimize Trigger Cuts

TH1F* HistDivide(TH1F*,TH1F*);//Define the Binomial Error in the efficiency 
//Be very carefull: The binomial error is correct only when we have a lot of events...
//In small samples you need a confidence belt...

//Declare Histograms
  
private:
  TFile *f;
  TTree *t;
  float et,eta,phi,drRMS,etaRMS,phiRMS,ecalEt,towerEt;
 int matched,nclusters;
  TH1F *ecal_isol_et,*tower_isol_et,*cluster_n,*cluster_eta_rms,*cluster_phi_rms,*cluster_dr_rms;
 

};

////////////////////////////////////////////////////////////////////////


L2Validation::L2Validation(std::string fileName)
{
  //Initialize
  f = new TFile(fileName.c_str());
  t = (TTree*)f->Get("l2tree");

  et=0.;
  eta=0.;
  phi=0.;
  drRMS=0.;
  etaRMS=0.;
  phiRMS=0.;
  ecalEt=0.;
  towerEt=0.;

  matched=0;
  nclusters=0;
 
  //Declare Branches

  t->SetBranchAddress("MCet",&et);
  t->SetBranchAddress("MCeta",&eta);
  t->SetBranchAddress("MCphi",&phi);
  t->SetBranchAddress("Matched",&matched);
  t->SetBranchAddress("cl_Nclusters",&nclusters);
  t->SetBranchAddress("cl_drRMS",&drRMS);
  t->SetBranchAddress("cl_etaRMS",&etaRMS);
  t->SetBranchAddress("cl_phiRMS",&phiRMS);
  t->SetBranchAddress("ecalIsol_Et",&ecalEt);
  t->SetBranchAddress("towerIsol_Et",&towerEt);

  //Setup Histograms
  
  ecal_isol_et    = new TH1F("ecal_isol_et","ECAL Isolation E_{t}",10,0.,10.);
  ecal_isol_et->GetXaxis()->SetTitle("Isolation E_{t} [GeV]");

  tower_isol_et   = new TH1F("tower_isol_et","CaloTower Isolation E_{t}",10,0.,10.);
  tower_isol_et->GetXaxis()->SetTitle("Isolation E_{t} [GeV]");

  cluster_n       = new TH1F("cluster_n","Clustering-Number of Clusters",15,0,15);
  cluster_n->GetXaxis()->SetTitle("# of Clusters");

  cluster_eta_rms = new TH1F("cluster_eta_rms","Clustering #eta RMS",50,0,0.5);
  cluster_eta_rms->GetXaxis()->SetTitle("Cluster #eta RMS");

  cluster_phi_rms = new TH1F("cluster_phi_rms","Clustering #phi RMS",50,0,0.5);
  cluster_phi_rms->GetXaxis()->SetTitle("Cluster #phi RMS");
 
  cluster_dr_rms  = new TH1F("cluster_dr_rms","Clustering #delta R RMS",50,0,0.5);
  cluster_dr_rms->GetXaxis()->SetTitle("Cluster #delta R RMS");
 




}

L2Validation::~L2Validation()
{
  f->Close();
}


void L2Validation::makePlots()
{
  TCanvas *c = new TCanvas("valplots","L2 Validation Plots");
  c->Divide(3,2);
  c->cd(1);
  t->Draw("ecalIsol_Et >>ecal_isol_et","Matched==1"); 
 c->cd(2);
  t->Draw("towerIsol_Et >>tower_isol_et","Matched==1"); 
 c->cd(3);
  t->Draw("cl_Nclusters >>cluster_n","Matched==1"); 
 c->cd(4);
  t->Draw("cl_etaRMS >>cluster_eta_rms","Matched==1"); 
 c->cd(5);
  t->Draw("cl_phiRMS >>cluster_phi_rms","Matched==1"); 
 c->cd(6);
  t->Draw("cl_drRMS >>cluster_dr_rms","Matched==1"); 
}


void L2Validation::Write(std::string fName)
{
  TFile *f2 = new TFile(fName.c_str(),"recreate");
  (ecal_isol_et)->Write();
  (tower_isol_et)->Write();
  (cluster_n)->Write();
  (cluster_eta_rms)->Write();
  (cluster_phi_rms)->Write();
  (cluster_dr_rms)->Write();
  f2->Close();

} 




TH1F* L2Validation::HistDivide(TH1F* hist1,TH1F* hist2)
{
  hist1->Sumw2();
  hist2->Sumw2();

  TH1F *out = hist1->Clone();
  out->Sumw2();
  int binNum,binDeNum;
  double eff,err;

  for(int i =0;i<hist1->GetNbinsX();++i)
    {
     
      binNum=hist1->GetBinContent(i);
      binDeNum=hist2->GetBinContent(i);
      if(binDeNum==0)
	{
	  eff=0.;
	  err=0.;
	}
      else
	{
	  eff = (double)(binNum/binDeNum);
          err = sqrt(eff*(1-eff)/binDeNum);
	}
      out->SetBinContent(i,eff);
      out->SetBinError(i,err);


    }

  out->SetLineWidth(1);
  out->SetMarkerStyle(20);
  out->GetYaxis()->SetRangeUser(0,1.05);

  return out;

}


double 
L2Validation::optimize(double ee ,double te ,int nc  ,double ce ,double cp ,double dd )
{
   
  TH1F*  eta_eff         = new TH1F("eta_eff","Efficiency vs #eta",48,-2.4,2.4);
  eta_eff->GetXaxis()->SetTitle("#eta");

  TH1F*  phi_eff         = new TH1F("phi_eff","Efficiency vs #phi",62,-3.1,3.1);
  phi_eff->GetXaxis()->SetTitle("#phi");
  
  TH1F*  et_eff          = new TH1F("et_eff","Efficiency vs E_{t}",50,0,100);
  et_eff->GetXaxis()->SetTitle("E_{t} [GeV]");


  TH1F * tmpet =new TH1F("tmpet","",50,0,100);
  TH1F * tmpeta =new TH1F("tmpeta","",48,-2.4,2.4);
  TH1F * tmpphi =new TH1F("tmpphi","",62,-3.1,3.1);

  int matched_only=0;
  int matched_passed=0;

  for(int i =0;i< t->GetEntries();++i)
    {
      t->GetEntry(i);
      if(matched)
	{
	  matched_only++;
	  tmpet->Fill(et);
	  tmpeta->Fill(eta);
	  tmpphi->Fill(phi);

	if(ecalEt<ee)
	  if(towerEt<te)
	    if(nclusters<nc)
	      if(etaRMS<ce)
		if(phiRMS<cp)
		  if(drRMS<dd)
		    {
		      et_eff->Fill(et);
		      eta_eff->Fill(eta);
		      phi_eff->Fill(phi);

		    matched_passed++;
		    }
	}
    }

  ////////////////////////////
  TCanvas *opt = new TCanvas("opt","Optimization");
  opt->Divide(2,2);
  opt->cd(1);
   et_eff = HistDivide(et_eff,tmpet);
   et_eff->Draw();
  opt->cd(2);
    eta_eff = HistDivide(eta_eff,tmpeta);
    eta_eff->Draw();
  opt->cd(3);
    phi_eff = HistDivide(phi_eff,tmpphi);
    phi_eff->Draw();

  /////////////////////////////

  if(matched_only>0)
  double eff= ((double)(matched_passed))/((double)matched_only);
  else
    eff=0;


  printf ("Efficiency = %f \n",eff);

  return eff;

}

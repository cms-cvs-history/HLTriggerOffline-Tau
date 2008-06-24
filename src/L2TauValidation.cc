#include "HLTriggerOffline/Tau/interface/L2TauValidation.h"
#include "Math/GenVector/VectorUtil.h"
#include <iostream>
#include <iomanip>
#include <fstream>

L2TauValidation::L2TauValidation(const edm::ParameterSet& iConfig):
 l2TauInfoAssoc_(iConfig.getParameter<edm::InputTag>("L2InfoAssociationInput")),
 mcColl_(iConfig.getParameter<edm::InputTag>("MatchedCollection")),
 met_(iConfig.getParameter<edm::InputTag >("MET")),
 matchDeltaRMC_(iConfig.getParameter<double>("MatchDeltaRMC")),
 triggerTag_((iConfig.getParameter<std::string>("TriggerTag"))),
 l2Isolated_(iConfig.getParameter<edm::InputTag>("L2IsolatedJets")),

 outFile_(iConfig.getParameter<std::string>("OutputFileName")),
 EtMin_(iConfig.getParameter<double>("EtMin")),
 EtMax_(iConfig.getParameter<double>("EtMax")),
 NBins_(iConfig.getParameter<int>("NBins"))


{

  DQMStore* store = &*edm::Service<DQMStore>();
  
  if(store)
    {
      //Create the histograms
      store->setCurrentFolder(triggerTag_);
      jetEt= store->book1D("L2tauCandEt","tauCandEt",NBins_,EtMin_,EtMax_);
      jetEta= store->book1D("L2tauCandEta","tauCandEta",50,-2.5,2.5);
      jetPhi= store->book1D("L2tauCandPhi","tauCandPhi",63,-3.14,3.14);
      ecalIsolEt=store->book1D("L2ecalIsolEt","ecalIsolEt",40,0,20);
      towerIsolEt=store->book1D("L2towerIsolEt","towerIsolEt",40,0,20);
      seedTowerEt=store->book1D("L2seedTowerEt","seedTowerEt",40,0,80);
      nClusters=store->book1D("L2nClusters","nClusters",20,0,20);
      clusterEtaRMS=store->book1D("L2clusterEtaRMS","clusterEtaRMS",25,0,0.5);
      clusterPhiRMS=store->book1D("L2clusterPhiRMS","clusterPhiRMS",25,0,0.5);
      clusterDeltaRRMS=store->book1D("L2clusterDeltaRRMS","clusterDeltaRRMS",25,0,0.5);
      EtEffNum=store->book1D("L2EtEffNum","Efficiency vs E_{t}(Numerator)",NBins_,EtMin_,EtMax_);
      EtEffDenom=store->book1D("L2EtEffDenom","Efficiency vs E_{t}(Denominator)",NBins_,EtMin_,EtMax_);
      EtEff=store->book1D("L2EtEff","Efficiency vs E_{t}",NBins_,EtMin_,EtMax_);
      MET=store->book1D("MET","Missing E_{t}",NBins_,EtMin_,EtMax_);
      
    }
  

 
}


L2TauValidation::~L2TauValidation()
{
}



void
L2TauValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

   Handle<L2TauInfoAssociation> l2TauInfoAssoc; //Handle to the input (L2 Tau Info Association)
   Handle<LVColl> McInfo; //Handle To The Truth!!!!
   Handle<reco::CaloJetCollection> l2Isolated;


   std::vector<l1extra::L1JetParticleRef> tauCandRefVec;

   if(iEvent.getByLabel(l2TauInfoAssoc_,l2TauInfoAssoc))//get the Association class
     {

       //Lets see if we have MC w matching or real data
       if(iEvent.getByLabel(mcColl_,McInfo))
       //If the Collection exists do work
       if(l2TauInfoAssoc->size()>0)
	 for(L2TauInfoAssociation::const_iterator p = l2TauInfoAssoc->begin();p!=l2TauInfoAssoc->end();++p)
	   {
	     //Retrieve The L2TauIsolationInfo Class from the AssociationMap
	     const L2TauIsolationInfo l2info = p->val;
       
	     //Retrieve the Jet From the AssociationMap
	     const Jet& jet =*(p->key);

  
      
	     if(match(jet,*McInfo))
		 {
		   ecalIsolEt->Fill(l2info.ECALIsolConeCut);
		   towerIsolEt->Fill(l2info.TowerIsolConeCut);
		   nClusters->Fill(l2info.ECALClusterNClusters);
		   seedTowerEt->Fill(l2info.SeedTowerEt);
		   clusterEtaRMS->Fill(l2info.ECALClusterEtaRMS);
		   clusterPhiRMS->Fill(l2info.ECALClusterPhiRMS);
		   clusterDeltaRRMS->Fill(l2info.ECALClusterDRRMS);
		   jetEt->Fill(jet.et());
		   jetEta->Fill(jet.eta());
		   jetPhi->Fill(jet.phi());
	      
		   EtEffDenom->Fill(jet.et());

  		   if(iEvent.getByLabel(l2Isolated_,l2Isolated))
		     {
		       if(matchJet(jet,*l2Isolated)) 
			    EtEffNum->Fill(jet.et());

		     }

		    






		 }

	   
	   } 
	       
     }

   //Plot the missing Et. To be used in SingleTau mainly
   Handle<CaloMETCollection> met;
   if(iEvent.getByLabel(met_,met))//get the Association class
     {
       MET->Fill((*met)[0].pt());
     }


}



void 
L2TauValidation::beginJob(const edm::EventSetup&)
{

}


void 
L2TauValidation::endJob() {
 
  //Get Efficiency

  EtEffNum->getTH1F()->Sumw2();
  EtEffDenom->getTH1F()->Sumw2();
  EtEff->getTH1F()->Divide(EtEffNum->getTH1F(),EtEffDenom->getTH1F(),1.,1.,"b");

  //Write file
  if(outFile_.size()>0)
    if (&*edm::Service<DQMStore>()) edm::Service<DQMStore>()->save (outFile_);

}

bool 
L2TauValidation::match(const reco::Jet& jet,const LVColl& McInfo)
{

  //Loop On the Collection and see if your tau jet is matched to one there
 //Also find the nearest Matched MC Particle to your Jet (to be complete)
 
 bool matched=false;

 if(McInfo.size()>0)
  for(std::vector<LV>::const_iterator it = McInfo.begin();it!=McInfo.end();++it)
   {
     	  double delta = ROOT::Math::VectorUtil::DeltaR(jet.p4().Vect(),*it);
	  if(delta<matchDeltaRMC_)
	    {
	      matched=true;
	     
	    }
   }



 return matched;
}

bool 
L2TauValidation::matchJet(const reco::Jet& jet,const reco::CaloJetCollection& McInfo)
{

  //Loop On the Collection and see if your tau jet is matched to one there
 //Also find the nearest Matched MC Particle to your Jet (to be complete)
 
 bool matched=false;

 if(McInfo.size()>0)
  for(reco::CaloJetCollection::const_iterator it = McInfo.begin();it!=McInfo.end();++it)
   {
     	  double delta = ROOT::Math::VectorUtil::DeltaR(jet.p4().Vect(),it->p4().Vect());
	  if(delta<matchDeltaRMC_)
	    {
	      matched=true;
	     
	    }
   }



 return matched;
}



#include "HLTriggerOffline/Tau/interface/HLTTauValidation.h"
#include "Math/GenVector/VectorUtil.h"

HLTTauValidation::HLTTauValidation(const edm::ParameterSet& ps) : 
  triggerEventObject_(ps.getUntrackedParameter<edm::InputTag>("triggerEventObject")),
  refCollection_(ps.getUntrackedParameter<edm::InputTag>("refTauCollection")),
  refLeptonCollection_(ps.getUntrackedParameter<edm::InputTag>("refLeptonCollection")),
  triggerTag_(ps.getUntrackedParameter<std::string>("TriggerTag","DoubleTau")),
  l1seedFilter_(ps.getUntrackedParameter<std::string>("L1SeedFilter","D")),
  l2filter_(ps.getUntrackedParameter<std::string>("L2EcalIsolFilter","D")),
  l25filter_(ps.getUntrackedParameter<std::string>("L25PixelIsolFilter","D")),
  l3filter_(ps.getUntrackedParameter<std::string>("L3SiliconIsolFilter","D")),
  electronFilter_(ps.getUntrackedParameter<std::string>("ElectronFilter","D")),
  muonFilter_(ps.getUntrackedParameter<std::string>("MuonFilter","D")),
  nTriggeredTaus_(ps.getUntrackedParameter<unsigned>("NTriggeredTaus",2)),
  nTriggeredLeptons_(ps.getUntrackedParameter<unsigned>("NTriggeredLeptons",0)),
  doRefAnalysis_(ps.getUntrackedParameter<bool>("DoReferenceAnalysis",false)),
  outFile_(ps.getUntrackedParameter<std::string>("OutputFileName","")),
  logFile_(ps.getUntrackedParameter<std::string>("LogFileName","log.txt")),
  matchDeltaRL1_(ps.getUntrackedParameter<double>("MatchDeltaRL1",0.3)),
  matchDeltaRHLT_(ps.getUntrackedParameter<double>("MatchDeltaRHLT",0.15))
{
  //initialize 
  NRefEvents=0;
  NLeptonEvents = 0;
  NLeptonEvents_Matched=0;
  NL1Events=0;
  NL1Events_Matched=0;
  NL2Events=0;
  NL2Events_Matched=0;
  NL25Events=0;
  NL25Events_Matched=0;
  NL3Events=0;
  NL3Events_Matched=0;

  

  //Declare DQM Store
  DQMStore* store = &*edm::Service<DQMStore>();

  if(store)
    {
      //Create the histograms
      store->setCurrentFolder("HLTTau_"+triggerTag_);
      triggerBits_Tau = store->book1D("triggerBits_Tau","Trigger Path Information(#tau)",4,0,4);
      triggerBits_Tau->setBinLabel(1,"L1");
      triggerBits_Tau->setBinLabel(2,"L2");
      triggerBits_Tau->setBinLabel(3,"L25");
      triggerBits_Tau->setBinLabel(4,"L3");
      triggerBits_Tau->setAxisTitle("#tau Sub Path Decision Bit");

      triggerBits_Ref = store->book1D("triggerBits_Ref","Trigger Path Information(Ref)",4,0,4);
      triggerBits_Ref->setBinLabel(1,"L1");
      triggerBits_Ref->setBinLabel(2,"L2");
      triggerBits_Ref->setBinLabel(3,"L25");
      triggerBits_Ref->setBinLabel(4,"L3");
      triggerBits_Ref->setAxisTitle("#tau Sub Path Decision Bit");

      etVsTriggerPath_Tau = store->book2D("etVsTriggerPath_Tau","E_{t} vs Trigger Path (Tau)",50,0,200,4,0,4);

      etVsTriggerPath_Tau->setBinLabel(1,"L1",2);
      etVsTriggerPath_Tau->setBinLabel(2,"L2",2);
      etVsTriggerPath_Tau->setBinLabel(3,"L25",2);
      etVsTriggerPath_Tau->setBinLabel(4,"L3",2);



      etVsTriggerPath_Ref = store->book2D("etVsTriggerPath_Ref","E_{t} vs Trigger Path (Reference)",50,0,200,4,0,4);

      etVsTriggerPath_Ref->setBinLabel(1,"L1",2);
      etVsTriggerPath_Ref->setBinLabel(2,"L2",2);
      etVsTriggerPath_Ref->setBinLabel(3,"L25",2);
      etVsTriggerPath_Ref->setBinLabel(4,"L3",2);
      

    }

}

HLTTauValidation::~HLTTauValidation()
{
}

//
// member functions
//

void
HLTTauValidation::endJob()
{
  printf("GENERATING OUTPUT--------------------------------------->\n");
  printf("Reference:\n");
  printf("   -Number of GOOD Ref Tau Events = %d\n",NRefEvents);
  printf("Trigger:\n");
  printf("   -Leptonic Trigger Accepted Events = %d   Accepted and Matched = %d\n",NLeptonEvents,NLeptonEvents_Matched);
  printf("   -L1 Accepted Events = %d  L1 Accepted and Matched = %d\n",NL1Events,NL1Events_Matched);
  printf("   -L2 Accepted Events = %d  L2 Accepted and Matched = %d\n",NL2Events,NL2Events_Matched);
  printf("   -L25 Accepted Events = %d  L25 Accepted and Matched = %d\n",NL25Events,NL25Events_Matched);
  printf("   -L3 Accepted Events = %d  L3 Accepted and Matched = %d\n",NL3Events,NL3Events_Matched);
  printf("HLT Efficiency with Ref to L1 Trigger(No Matching):\n");
  printf("   -L2 = %f +/- %f \n",calcEfficiency(NL2Events,NL1Events)[0],calcEfficiency(NL2Events,NL1Events)[1]);
  printf("   -L25 = %f +/- %f \n",calcEfficiency(NL25Events,NL1Events)[0],calcEfficiency(NL25Events,NL1Events)[1]);
  printf("   -L2 = %f +/- %f \n",calcEfficiency(NL3Events,NL1Events)[0],calcEfficiency(NL3Events,NL25Events)[1]);
  printf("HLT Efficiency with Ref to L1 Trigger(with Matching):\n");
  printf("   -L2 = %f +/- %f \n",calcEfficiency(NL2Events_Matched,NL1Events_Matched)[0],calcEfficiency(NL2Events_Matched,NL1Events_Matched)[1]);
  printf("   -L25 = %f +/- %f \n",calcEfficiency(NL25Events_Matched,NL1Events_Matched)[0],calcEfficiency(NL25Events_Matched,NL1Events_Matched)[1]);
  printf("   -L2 = %f +/- %f \n",calcEfficiency(NL3Events_Matched,NL1Events_Matched)[0],calcEfficiency(NL3Events_Matched,NL25Events_Matched)[1]);
  printf("HLT Efficiency with Ref to Matching Object):\n");
  printf("   -L2 = %f +/- %f \n",calcEfficiency(NL2Events_Matched,NRefEvents)[0],calcEfficiency(NL2Events_Matched,NRefEvents)[1]);
  printf("   -L25 = %f +/- %f \n",calcEfficiency(NL25Events_Matched,NRefEvents)[0],calcEfficiency(NL25Events_Matched,NRefEvents)[1]);
  printf("   -L2 = %f +/- %f \n",calcEfficiency(NL3Events_Matched,NRefEvents)[0],calcEfficiency(NL3Events_Matched,NL25Events_Matched)[1]);
  printf("--------------------------------------------------------\n");
  printf("Note: The errors are binomial..");	 



  //Write DQM thing..
  if(outFile_.size()>0)
  if (&*edm::Service<DQMStore>()) edm::Service<DQMStore>()->save (outFile_);


  //Write Log File
  FILE *fp;
  fp = fopen(logFile_.c_str(),"w");

  fprintf(fp,"GENERATING OUTPUT--------------------------------------->\n");
  fprintf(fp,"Reference:\n");
  fprintf(fp,"   -Number of GOOD Ref Events = %d\n",NRefEvents);
  fprintf(fp,"Trigger:\n");
  fprintf(fp,"   -Leptonic Trigger Accepted Events = %d   Accepted and Matched = %d\n",NLeptonEvents,NLeptonEvents_Matched);
  fprintf(fp,"   -L1 Accepted Events = %d  L1 Accepted and Matched = %d\n",NL1Events,NL1Events_Matched);
  fprintf(fp,"   -L2 Accepted Events = %d  L2 Accepted and Matched = %d\n",NL2Events,NL2Events_Matched);
  fprintf(fp,"   -L25 Accepted Events = %d  L25 Accepted and Matched = %d\n",NL25Events,NL25Events_Matched);
  fprintf(fp,"   -L3 Accepted Events = %d  L3 Accepted and Matched = %d\n",NL3Events,NL3Events_Matched);
  fprintf(fp,"HLT Efficiency with Ref to L1 Trigger(No Matching):\n");
  fprintf(fp,"   -L2 = %f +/- %f \n",calcEfficiency(NL2Events,NL1Events)[0],calcEfficiency(NL2Events,NL1Events)[1]);
  fprintf(fp,"   -L25 = %f +/- %f \n",calcEfficiency(NL25Events,NL1Events)[0],calcEfficiency(NL25Events,NL1Events)[1]);
  fprintf(fp,"   -L3 = %f +/- %f \n",calcEfficiency(NL3Events,NL1Events)[0],calcEfficiency(NL3Events,NL25Events)[1]);
  fprintf(fp,"HLT Efficiency with Ref to L1 Trigger(with Matching):\n");
  fprintf(fp,"   -L2 = %f +/- %f \n",calcEfficiency(NL2Events_Matched,NL1Events_Matched)[0],calcEfficiency(NL2Events_Matched,NL1Events_Matched)[1]);
  fprintf(fp,"   -L25 = %f +/- %f \n",calcEfficiency(NL25Events_Matched,NL1Events_Matched)[0],calcEfficiency(NL25Events_Matched,NL1Events_Matched)[1]);
  fprintf(fp,"   -L3 = %f +/- %f \n",calcEfficiency(NL3Events_Matched,NL1Events_Matched)[0],calcEfficiency(NL3Events_Matched,NL25Events_Matched)[1]);
  fprintf(fp,"HLT Efficiency with Ref to Matching Object):\n");
  fprintf(fp,"   -L2 = %f +/- %f \n",calcEfficiency(NL2Events_Matched,NRefEvents)[0],calcEfficiency(NL2Events_Matched,NRefEvents)[1]);
  fprintf(fp,"   -L25 = %f +/- %f \n",calcEfficiency(NL25Events_Matched,NRefEvents)[0],calcEfficiency(NL25Events_Matched,NRefEvents)[1]);
  fprintf(fp,"   -L3 = %f +/- %f \n",calcEfficiency(NL3Events_Matched,NRefEvents)[0],calcEfficiency(NL3Events_Matched,NL25Events_Matched)[1]);
  fprintf(fp,"--------------------------------------------------------\n");
  fprintf(fp,"Note: The errors are binomial..");	 
  fclose(fp);
 
}


void
HLTTauValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace l1extra;
  using namespace trigger;





  //Look at the reference collection (i.e MC)
  Handle<LVColl> refC;
  Handle<LVColl> refCL;
 
  if(doRefAnalysis_)
    {
      bool tau_ok = true;
      bool lepton_ok = true;

      //Tau reference
      if(iEvent.getByLabel(refCollection_,refC))
      if(refC->size()<nTriggeredTaus_)
	{
	  tau_ok = false;
	}
  
      //lepton reference
 
      if(iEvent.getByLabel(refLeptonCollection_,refCL))
	if(refCL->size()<nTriggeredLeptons_)
	{
	  lepton_ok = false;
	}
      
    
      
      if(lepton_ok&&tau_ok)
	  NRefEvents++;

    }


  //get The triggerEvent


  Handle<TriggerEventWithRefs> trigEv;
  iEvent.getByLabel(triggerEventObject_,trigEv);

  if (trigEv.isValid()) 
    {
     
      //LEPTONS 
      unsigned Leptons = 0;
      unsigned Leptons_Matched = 0;


      //electrons
      size_t ELID=0;
      ELID =trigEv->filterIndex(electronFilter_);
      if(ELID!=trigEv->size())
	{
	  VRelectron electrons;
	  trigEv->getObjects(ELID,92,electrons);
	  Leptons+= electrons.size();

	  if(doRefAnalysis_)
	  for(size_t i = 0;i<electrons.size();++i)
	    {
	      if(match((*electrons[i]).p4(),*refCL,matchDeltaRHLT_))
		Leptons_Matched++;
	    } 


	}

      //muons
      size_t MUID=0;
      MUID =trigEv->filterIndex(muonFilter_);
      if(MUID!=trigEv->size())
	{
	  VRmuon muons;
	  trigEv->getObjects(MUID,93,muons);
	  Leptons+= muons.size();

	  if(doRefAnalysis_)
	  for(size_t i = 0;i<muons.size();++i)
	    {
	      //	      const reco::Jet jet = dynamic_cast<reco::Jet>(*(muons[i])); 
	      if(match((*muons[i]).p4(),*refCL,matchDeltaRHLT_))
		Leptons_Matched++;
	    } 


	}

      
      if(Leptons>=nTriggeredLeptons_&&nTriggeredLeptons_>0)
	NLeptonEvents++;

      if(Leptons_Matched>=nTriggeredLeptons_&&nTriggeredLeptons_>0)
	NLeptonEvents_Matched++;



    //L1Analysis Seed
      size_t L1ID=0;
      L1ID =trigEv->filterIndex(l1seedFilter_);

      if(L1ID!=trigEv->size())
	{
	  //Get L1Objects
	  VRl1jet L1Taus;
	  trigEv->getObjects(L1ID,86,L1Taus);
	  //Check if the number of L1 Taus is OK
	  if(L1Taus.size()>=nTriggeredTaus_&& Leptons>=nTriggeredLeptons_)
	  {
	    NL1Events++;
	    triggerBits_Tau->Fill(0.5);
	   
	  }


	  //Loop on L1 Taus  
	  unsigned jets_matched=0;	  
	  for(size_t i = 0;i<L1Taus.size();++i)
	  {
	    etVsTriggerPath_Tau->Fill(L1Taus[i]->et(),0.5);

	    if(doRefAnalysis_)
	      if(match((*L1Taus[i]).p4(),*refC,matchDeltaRL1_))
		{
		    jets_matched++;
		    etVsTriggerPath_Ref->Fill(L1Taus[i]->et(),0.5);
		}
	  }  
	  if(jets_matched>=nTriggeredTaus_&&Leptons_Matched>=nTriggeredLeptons_)
	    {
	      NL1Events_Matched++;
	      triggerBits_Ref->Fill(0.5);
	    
	    }

	}


    //L2Analysis Seed
      size_t L2ID=0;
      L2ID =trigEv->filterIndex(l2filter_);

      if(L2ID!=trigEv->size())
	{
	  //Get L2Objects
	  VRjet L2Taus;
	  trigEv->getObjects(L2ID,94,L2Taus);
	  if(L2Taus.size()>=nTriggeredTaus_&&Leptons>=nTriggeredLeptons_)
	    {
	      NL2Events++;
	      triggerBits_Tau->Fill(1.5);
	    }
	  //Loop on L2 Taus
  	  unsigned jets_matched=0;	  
	  for(size_t i = 0;i<L2Taus.size();++i)
	  {
	     etVsTriggerPath_Tau->Fill(L2Taus[i]->et(),1.5);

	    if(doRefAnalysis_)
	      if(match((*L2Taus[i]).p4(),*refC,matchDeltaRHLT_))
		{
		  jets_matched++;
	          etVsTriggerPath_Ref->Fill(L2Taus[i]->et(),1.5);

		}
	     
	  }
		  
	  if(jets_matched>=nTriggeredTaus_&&Leptons_Matched>=nTriggeredLeptons_)
	    {
	      NL2Events_Matched++;
	      triggerBits_Ref->Fill(1.5);
	    }



	}

      //L25Analysis Seed
      size_t L25ID=0;
      L25ID =trigEv->filterIndex(l25filter_);

      if(L25ID!=trigEv->size())
	{
	  //Get L25Objects
	  VRjet L25Taus;
	  trigEv->getObjects(L25ID,94,L25Taus);
	  if(L25Taus.size()>=nTriggeredTaus_&&Leptons>=nTriggeredLeptons_)
	    {
	      NL25Events++;
	      triggerBits_Tau->Fill(2.5);
	    }
	  //Loop on L25 Taus
  	  unsigned jets_matched=0;	  
	  for(size_t i = 0;i<L25Taus.size();++i)
	  {
	     etVsTriggerPath_Tau->Fill(L25Taus[i]->et(),2.5);

	    if(doRefAnalysis_)
	      if(match((*L25Taus[i]).p4(),*refC,matchDeltaRHLT_))
		{
		    jets_matched++;
		     etVsTriggerPath_Ref->Fill(L25Taus[i]->et(),2.5);

		}
	  }  
	  if(jets_matched>=nTriggeredTaus_&&Leptons_Matched>=nTriggeredLeptons_)
	    {
	      NL25Events_Matched++;
	      triggerBits_Ref->Fill(2.5);
	    }


	}

      //L3Analysis Seed
      size_t L3ID=0;
      L3ID =trigEv->filterIndex(l3filter_);

      if(L3ID!=trigEv->size())
	{
	  //Get L3Objects
	  VRjet L3Taus;
	  trigEv->getObjects(L3ID,94,L3Taus);
	  if(L3Taus.size()>=nTriggeredTaus_&&Leptons>=nTriggeredLeptons_)
	    {
	      NL3Events++;
	      triggerBits_Tau->Fill(3.5);
	    }
	  //Loop on L3 Taus
  	  unsigned jets_matched=0;	  
	  for(size_t i = 0;i<L3Taus.size();++i)
	  {
	    etVsTriggerPath_Tau->Fill(L3Taus[i]->et(),3.5);

	    if(doRefAnalysis_)
	      if(match((*L3Taus[i]).p4(),*refC,matchDeltaRHLT_))
		{
		    jets_matched++;
	    	    etVsTriggerPath_Ref->Fill(L3Taus[i]->et(),3.5);

		}
	  }
		  
	  if(jets_matched>=nTriggeredTaus_&&Leptons_Matched>=nTriggeredLeptons_)
	    {
	      NL3Events_Matched++;
	      triggerBits_Ref->Fill(3.5);
	    }
	}
  } 
  else 
  {
    cout << "Handle invalid! Check InputTag provided." << endl;
  }
     
}




bool 
HLTTauValidation::match(const LV& jet,const LVColl& McInfo,double dr)
{
 
  bool matched=false;

  if(McInfo.size()>0)
    for(std::vector<LV>::const_iterator it = McInfo.begin();it!=McInfo.end();++it)
      {
	double delta = ROOT::Math::VectorUtil::DeltaR(jet,*it);
	if(delta<dr)
	  {
	    matched=true;
	  }
      }

  return matched;
}

double* 
HLTTauValidation::calcEfficiency(int num,int denom)
{
  double a[2];
  if(denom==0)
    {
      a[0]=0;
      a[1]=0;
    }
  else
    {    
      a[0] = ((double)num)/((double)denom);
      a[1] = sqrt(a[0]*(1-a[0])/((double)denom));
    }
  return a;
}

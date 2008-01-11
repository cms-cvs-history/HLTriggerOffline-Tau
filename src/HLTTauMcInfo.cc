#include "HLTriggerOffline/Tau/interface/HLTTauMcInfo.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace HepMC;
using namespace std;

HLTTauMcInfo::HLTTauMcInfo(const edm::ParameterSet& iConfig)
{
  genParticles = iConfig.getParameter<InputTag>("GenParticles");
  m_PDG = iConfig.getParameter<int>("BosonPID");
  etaMax = iConfig.getParameter<double>("EtaMax");
  ptMin = iConfig.getParameter<double>("PtMin");
  produces<LorentzVectorCollection>("Leptons");
  produces<LorentzVectorCollection>("Jets");
  produces<LorentzVectorCollection>("Neutrina");
}

HLTTauMcInfo::~HLTTauMcInfo(){ }

void HLTTauMcInfo::produce(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr<LorentzVectorCollection> product_Leptons(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_Jets(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_Neutrina(new LorentzVectorCollection);

  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(genParticles, evt);
  HepMC::GenEvent*  myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
  HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
  
  
  TLorentzVector taunet; 
  TLorentzVector neutrina_tmp(0.,0.,0.,0.);
  TLorentzVector neutrino_tmp(0.,0.,0.,0.);

  for (;p != myGenEvent->particles_end(); ++p ) {
    if(abs((*p)->pdg_id())==m_PDG&&(*p)->end_vertex())
      {
	TLorentzVector Boson((*p)->momentum().px(),(*p)->momentum().py(),(*p)->momentum().pz(),(*p)->momentum().e());
	HepMC::GenVertex::particle_iterator z = (*p)->end_vertex()->particles_begin(HepMC::descendants);
	
	for(; z != (*p)->end_vertex()->particles_end(HepMC::descendants); z++)
	  {
	    if(abs((*z)->pdg_id()) == 15 && (*z)->status()==2)
	      {
		bool lept_decay = false;
		TLorentzVector tau((*z)->momentum().px(),(*z)->momentum().py(),(*z)->momentum().pz(),(*z)->momentum().e());
		HepMC::GenVertex::particle_iterator t = (*z)->end_vertex()->particles_begin(HepMC::descendants);
		for(; t != (*z)->end_vertex()->particles_end(HepMC::descendants); t++)
		  {
		    if(abs((*t)->pdg_id()) == 11 || abs((*t)->pdg_id()) == 13)lept_decay=true;
		    if(abs((*t)->pdg_id()) == 11)
		      {
			LorentzVector vec((*t)->momentum().px(),(*t)->momentum().py(),(*t)->momentum().pz(),(*t)->momentum().e());
			product_Leptons->push_back(vec);
		      }
		    if(abs((*t)->pdg_id()) == 13)
		      {
			LorentzVector vec((*t)->momentum().px(),(*t)->momentum().py(),(*t)->momentum().pz(),(*t)->momentum().e());
			product_Leptons->push_back(vec);
		      }
		    if(abs((*t)->pdg_id()) == 16)taunet.SetPxPyPzE((*t)->momentum().px(),(*t)->momentum().py(),(*t)->momentum().pz(),(*t)->momentum().e());
		    if(abs((*t)->pdg_id()) == 16||abs((*t)->pdg_id()) == 14||abs((*t)->pdg_id()) == 12)
		      {
			neutrino_tmp.SetPxPyPzE((*t)->momentum().px(),(*t)->momentum().py(),(*t)->momentum().pz(),(*t)->momentum().e());
			neutrina_tmp+=neutrino_tmp;
		      }
		    
		  }
	      if(lept_decay==false)
		{
		  TLorentzVector jetMom=tau-taunet;
		  LorentzVector vec(jetMom.Px(),jetMom.Py(),jetMom.Pz(),jetMom.E());
		  if(jetMom.Perp() > ptMin && fabs(jetMom.Eta()) < etaMax)
		    product_Jets->push_back(vec);
		}
	      
	      }
	  }
      }
  }

  delete myGenEvent;
  
  LorentzVector neutrina(neutrina_tmp.Px(),neutrina_tmp.Py(),neutrina_tmp.Pz(),neutrina_tmp.E());
  product_Neutrina->push_back(neutrina);
  iEvent.put(product_Leptons,"Leptons");
  iEvent.put(product_Jets,"Jets");
  iEvent.put(product_Neutrina,"Neutrina");
}

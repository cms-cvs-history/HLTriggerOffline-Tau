// -*- C++ -*-
//
// Package:    HLTMuonTauAnalyzer
// Class:      HLTMuonTauAnalyzer
// 
/**\class HLTMuonTauAnalyzer HLTMuonTauAnalyzer.cc HLTriggerOffline/Muon/src/HLTMuonTauAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Sho Maruyama, from Muriel Vander Donckt's code
//         Created:  Tue Jul 24 12:17:12 CEST 2007
// $Id: HLTMuonTauAnalyzer.cc,v 1.3 2008/06/17 13:16:45 muriel Exp $
//

#include "HLTriggerOffline/Tau/interface/HLTMuonTauAnalyzer.h"
typedef std::vector< edm::ParameterSet > Parameters;

HLTMuonTauAnalyzer::HLTMuonTauAnalyzer(const edm::ParameterSet& pset)
{
    Parameters TriggerLists=pset.getParameter<Parameters>("TriggerCollection");
    NumberOfTriggers=TriggerLists.size();
    for( int index=0; index < NumberOfTriggers; index++) {
      HLTMuonRate *hmg=new HLTMuonRate(pset,index);
      muTriggerAnalyzer.push_back(hmg);
    }
}

HLTMuonTauAnalyzer::~HLTMuonTauAnalyzer()
{
  using namespace edm;
  for (  std::vector<HLTMuonRate *>::iterator iTrig = muTriggerAnalyzer.begin(); iTrig != muTriggerAnalyzer.end(); ++iTrig){
    delete *iTrig;
  } 
  muTriggerAnalyzer.clear();
}

void
HLTMuonTauAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   for (  std::vector<HLTMuonRate *>::iterator iTrig = muTriggerAnalyzer.begin(); iTrig != muTriggerAnalyzer.end(); ++iTrig){
     (*iTrig)->analyze(iEvent);
   } 
}

void 
HLTMuonTauAnalyzer::beginJob(const edm::EventSetup&)
{
  for (  std::vector<HLTMuonRate *>::iterator iTrig = muTriggerAnalyzer.begin(); iTrig != muTriggerAnalyzer.end(); ++iTrig){
    (*iTrig)->BookHistograms();
  } 
}

void 
HLTMuonTauAnalyzer::endJob() {
  using namespace edm;
  for (  std::vector<HLTMuonRate *>::iterator iTrig = muTriggerAnalyzer.begin(); iTrig != muTriggerAnalyzer.end(); ++iTrig){
    (*iTrig)->WriteHistograms();
  } 
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTMuonTauAnalyzer);

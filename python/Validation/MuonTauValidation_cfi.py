import FWCore.ParameterSet.Config as cms

MuonTauPathVal = cms.EDFilter("HLTTauValidation",
    triggerEventObject    = cms.untracked.InputTag("hltTriggerSummaryRAW"),
    refTauCollection      = cms.untracked.InputTag("TauMCProducer","Taus"),
    refLeptonCollection   = cms.untracked.InputTag("TauMCProducer","Muons"),
    DQMFolder             = cms.untracked.string('HLT/HLTTAU/MuonTau/Path'),
    L1SeedFilter          = cms.untracked.InputTag("hltLevel1GTSeedMuonTau","","HLT"),
    L2EcalIsolFilter      = cms.untracked.InputTag("hltFilterEcalIsolatedTauJetsMuonTau","","HLT"),
    L25PixelIsolFilter    = cms.untracked.InputTag("hltFilterPixelTrackIsolatedTauJetsMuonTau","","HLT"),
    L3SiliconIsolFilter   = cms.untracked.InputTag("DUMMY"),
    MuonFilter            = cms.untracked.InputTag("hltMuonTauIsoL3IsoFiltered","","HLT"),
    ElectronFilter        = cms.untracked.InputTag("DUMMY"),
    NTriggeredTaus        = cms.untracked.uint32(1),
    NTriggeredLeptons     = cms.untracked.uint32(1),
    DoReferenceAnalysis   = cms.untracked.bool(True),
    OutputFileName        = cms.untracked.string(''),
    LogFileName           = cms.untracked.string('MuonTauValidation.log'),
    MatchDeltaRL1         = cms.untracked.double(0.5),
    MatchDeltaRHLT        = cms.untracked.double(0.3)
)


MuonTauL2Val = cms.EDFilter("HLTTauCaloDQMOfflineSource",
    DQMFolder              = cms.string('HLT/HLTTAU/MuonTau/L2'),
    L2InfoAssociationInput = cms.InputTag("hltL2MuonTauIsolationProducer","L2TauIsolationInfoAssociator"),
    refCollection          = cms.InputTag("TauMCProducer","Taus"),
    MET                    = cms.InputTag("hltMET"),
    doReference            = cms.bool(True),
    MatchDeltaR            = cms.double(0.3),
    OutputFileName         = cms.string(''),
    L2IsolatedJets         = cms.InputTag("hltL2MuonTauIsolationSelector","Isolated"),
    EtMin                  = cms.double(0.),
    EtMax                  = cms.double(60.),
    NBins                  = cms.int32(30)                            
)


MuonTauL25Val = cms.EDFilter("HLTTauTrkDQMOfflineSource",
    DQMFolder              = cms.string('HLT/HLTTAU/MuonTau/L25'),
    ConeIsolation          = cms.InputTag("hltPixelTrackConeIsolationMuonTau"),
    InputJets              = cms.InputTag("hltL2MuonTauIsolationSelector","Isolated"),                             
    refCollection          = cms.InputTag("TauMCProducer","Taus"),
    Type                   = cms.string('L25'),                           
    doReference            = cms.bool(True),
    MatchDeltaR            = cms.double(0.3),
    OutputFileName         = cms.string(''),
    EtMin                  = cms.double(0.),
    EtMax                  = cms.double(60.),
    NBins                  = cms.int32(30)                            
)


MuonTauValidation = cms.Sequence(MuonTauPathVal + MuonTauL2Val+MuonTauL25Val)




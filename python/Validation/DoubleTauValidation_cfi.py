import FWCore.ParameterSet.Config as cms

DoubleTauPathVal = cms.EDFilter("HLTTauValidation",
    triggerEventObject    = cms.untracked.InputTag("hltTriggerSummaryRAW","","TEST"),
    refTauCollection      = cms.untracked.InputTag("TauMCProducer","Taus"),
    refLeptonCollection   = cms.untracked.InputTag("NOTHING"),
    DQMFolder             = cms.untracked.string('HLT/HLTTAU/DoubleTau/Path'),
    L1SeedFilter          = cms.untracked.InputTag("hltDoubleTauL1SeedFilter","","TEST"),
    L2EcalIsolFilter      = cms.untracked.InputTag("hltFilterDoubleTauEcalIsolation","","TEST"),
    L25PixelIsolFilter    = cms.untracked.InputTag("hltFilterL25PixelTau","","TEST"),
    L3SiliconIsolFilter   = cms.untracked.InputTag("DUMMY"),
    MuonFilter            = cms.untracked.InputTag("DUMMY"),
    ElectronFilter        = cms.untracked.InputTag("DUMMY"),
    NTriggeredTaus        = cms.untracked.uint32(2),
    NTriggeredLeptons     = cms.untracked.uint32(0),
    DoReferenceAnalysis   = cms.untracked.bool(True),
    OutputFileName        = cms.untracked.string(''),
    LogFileName           = cms.untracked.string('DoubleTauValidation.log'),
    MatchDeltaRL1         = cms.untracked.double(0.5),
    MatchDeltaRHLT        = cms.untracked.double(0.3)
)


DoubleTauL2Val = cms.EDFilter("HLTTauCaloDQMOfflineSource",
    DQMFolder              = cms.string('HLT/HLTTAU/DoubleTau/L2'),
    L2InfoAssociationInput = cms.InputTag("hltL2DoubleTauIsolationProducer","L2TauIsolationInfoAssociator"),
    refCollection          = cms.InputTag("TauMCProducer","Taus"),
    MET                    = cms.InputTag("hltMet"),
    doReference            = cms.bool(True),
    MatchDeltaR            = cms.double(0.3),
    OutputFileName         = cms.string(''),
    L2IsolatedJets         = cms.InputTag("hltL2DoubleTauIsolationSelector","Isolated"),
    EtMin                  = cms.double(0.),
    EtMax                  = cms.double(60.),
    NBins                  = cms.int32(30)                            
)


DoubleTauL25Val = cms.EDFilter("HLTTauTrkDQMOfflineSource",
    DQMFolder              = cms.string('HLT/HLTTAU/DoubleTau/L25'),
    ConeIsolation          = cms.InputTag("hltConeIsolationL25PixelTauIsolated"),
    InputJets              = cms.InputTag("hltL2DoubleTauIsolationSelector","Isolated"),                             
    IsolatedJets           = cms.InputTag("hltIsolatedL25PixelTau"),                             
    refCollection          = cms.InputTag("TauMCProducer","Taus"),
    Type                   = cms.string('L25'),                           
    doReference            = cms.bool(True),
    MatchDeltaR            = cms.double(0.3),
    OutputFileName         = cms.string(''),
    EtMin                  = cms.double(0.),
    EtMax                  = cms.double(60.),
    NBins                  = cms.int32(30)                            
)





DoubleTauValidation = cms.Sequence(DoubleTauPathVal + DoubleTauL2Val + DoubleTauL25Val)


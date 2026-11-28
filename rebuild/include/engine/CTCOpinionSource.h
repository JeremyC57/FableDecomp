#ifndef FABLE_ENGINE_CTCOPINIONSOURCE_H
#define FABLE_ENGINE_CTCOPINIONSOURCE_H
// New header (chunk 2004, row 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable). Retail opt-in only: FABLE_RETAIL_CTCOPINIONSOURCE.
// Class: Fable.exe RTTI .?AVCTCOpinionSource@@ : CTCBase; interface TCI_OPINION_SOURCE (0x50: the HasTC test `test dword [thing+28h],
// 10000h` and LowerBound key 0x50 at 0x00960B51..0x00960B76 / 0x00960FC6..0x00960FF4). Class key V (pdb_class_keys.tsv).
// Members: Ego_r.pdb TPI (struct_layouts_egor.tsv, 144 bytes) at retail offsets -- the PDB's Dinkumware vector<long> DefStack is
// 0x10, retail STLport is 0xC, so every later member sits 4 lower: UseNavLineOfSight +0x18 (read by the filter 0x00961032),
// AlwaysAwareOf +0x75 (0x00960D9D), ReactNonstopFlag +0x77 (0x0096102B), VirtualDeed +0x80 / VirtualDeedID +0x84 (0x00960BC2 /
// 0x00960BCB, 0x00960FFE / 0x0096100C); sizeof 0x8C.
// Methods: Ego_r.pdb / FableWin.pdb inline accessors ?IsAlwaysAwareOf@CTCOpinionSource@@QBE_NXZ,
// ?PeekVirtualDeed@CTCOpinionSource@@QBE?AW4EOpinionDeedType@@XZ, ?PeekVirtualDeedID@CTCOpinionSource@@QBEJXZ (no out-of-line copy in
// Fable.exe: read in place) and ?StatGetInterfaceType@CTCOpinionSource@@SI?AW4ETCInterfaceType@@XZ.
// Members with no shared model here (the vector, the faction def pointer, the counted opinion set) are opaque storage at their
// retail offsets (type and name in the comment).
#ifdef FABLE_RETAIL_CTCOPINIONSOURCE
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/EOpinionDeedType.h"
#ifdef FABLE_RETAIL_OPINION_SELECTION_PASS30
#include "engine/CFactionDef.h"
#include "engine/CDefPointer.h"
#endif
class CTCOpinionSource : public CTCBase {
#ifdef FABLE_RETAIL_GREETING_PASS23
public:
    // Both original PDB constructors: public CThing&; normal fixture lifetimes.
    CTCOpinionSource(CThing&);
private:
#endif
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_OPINION_SOURCE; }
#ifdef FABLE_RETAIL_OPINION_FILTER_PASS27
    bool IsReactToSelfPermitted() const { return ReactToSelfPermitted; }
    bool IsUnknownThreatSource() const { return UnknownThreatSource; }
#endif
#ifdef FABLE_RETAIL_OPINION_SELECTION_PASS30
    CDefPointer<const CFactionDef> PeekRestrictToFactionAndAllies() const { return RestrictToFactionAndAllies; }
    bool PeekRestrictToFactionIsReversed() const { return RestrictToFactionIsReversed; }
    const bool* PeekReactionFlags() const { return ReactionFlags; }
#endif
    bool IsAlwaysAwareOf() const { return AlwaysAwareOf; }
#ifdef FABLE_RETAIL_GET_OPINIONS_PASS30
    bool PeekBypassesPersonality() const { return BypassesPersonality; }
    bool PeekRelativeEffect() const { return RelativeEffect; }
    const COpinionSet& PeekOpinionProduced() const { return *POpinionProduced; }
#endif
    EOpinionDeedType PeekVirtualDeed() const { return VirtualDeed; }
    long PeekVirtualDeedID() const { return VirtualDeedID; }
#ifdef FABLE_RETAIL_CTCOPINIONSOURCE_AWARENESS_PEEKS
    // Retail opt-in (agentNF41, row 0x00960FC0 CThingFilter_IsOpinionSourceWithFreshDeed::operator()): Ego_r.pdb / FableWin.pdb
    // ?IsReactNonstop@CTCOpinionSource@@QBE_NXZ and ?PeekUseNavLineOfSight@CTCOpinionSource@@QBE_NXZ (inline in retail: the filter
    // reads +0x77 at 0x0096102B and +0x18 at 0x00961032 in place).
    bool IsReactNonstop() const { return ReactNonstopFlag; }
    bool PeekUseNavLineOfSight() const { return UseNavLineOfSight; }
#endif
#ifdef FABLE_RETAIL_CTCOPINIONSOURCE_SETREACTNONSTOP
    // Retail opt-in (hand3c3, row 0x0078D250 CTCBerserk::FrameUpdate): Ego_d.pdb ?SetReactNonstop@CTCOpinionSource@@QAEX_N@Z, inline in
    // retail (FrameUpdate stores 1 to +0x77 of the attached berserk opinion source in place, 0x0078D337). No layout change.
    void SetReactNonstop(bool value) { ReactNonstopFlag = value; }
#endif
private:
    unsigned long    DefStack_[3];                       // +0x0c std::vector<long> DefStack (retail STLport, 12 bytes)
    bool             UseNavLineOfSight;                  // +0x18
    bool             WitnessesWillNotAttack;             // +0x19
    bool             WitnessesWillOverrideFactionEnemy;  // +0x1a
    bool             BypassesPersonality;                // +0x1b
    bool             RelativeEffect;                     // +0x1c
#ifdef FABLE_RETAIL_OPINION_SELECTION_PASS30
    CDefPointer<const CFactionDef> RestrictToFactionAndAllies;
#else
    unsigned long    RestrictToFactionAndAllies_;        // +0x20 CDefPointer<const CFactionDef> RestrictToFactionAndAllies
#endif
    bool             RestrictToFactionIsReversed;        // +0x24
    bool             ReactionFlags[79];                  // +0x25
    bool             ReactToSelfPermitted;               // +0x74
    bool             AlwaysAwareOf;                      // +0x75
    bool             UnknownThreatSource;                // +0x76
    bool             ReactNonstopFlag;                   // +0x77
#ifdef FABLE_RETAIL_GET_OPINIONS_PASS30
    CCountedPointer<COpinionSet> POpinionProduced;
#else
    unsigned long    POpinionProduced_[2];               // +0x78 CCountedPointer<COpinionSet> POpinionProduced
#endif
    EOpinionDeedType VirtualDeed;                        // +0x80
    long             VirtualDeedID;                      // +0x84
    unsigned long    FrameToClearVirtualDeed;            // +0x88
};
FABLE_STATIC_ASSERT_AT(48, sizeof(CTCOpinionSource) == 0x8c);
#endif
#endif // FABLE_ENGINE_CTCOPINIONSOURCE_H

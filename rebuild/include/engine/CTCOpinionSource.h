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
class CTCOpinionSource : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_OPINION_SOURCE; }
    bool IsAlwaysAwareOf() const { return AlwaysAwareOf; }
    EOpinionDeedType PeekVirtualDeed() const { return VirtualDeed; }
    long PeekVirtualDeedID() const { return VirtualDeedID; }
private:
    unsigned long    DefStack_[3];                       // +0x0c std::vector<long> DefStack (retail STLport, 12 bytes)
    bool             UseNavLineOfSight;                  // +0x18
    bool             WitnessesWillNotAttack;             // +0x19
    bool             WitnessesWillOverrideFactionEnemy;  // +0x1a
    bool             BypassesPersonality;                // +0x1b
    bool             RelativeEffect;                     // +0x1c
    unsigned long    RestrictToFactionAndAllies_;        // +0x20 CDefPointer<const CFactionDef> RestrictToFactionAndAllies
    bool             RestrictToFactionIsReversed;        // +0x24
    bool             ReactionFlags[79];                  // +0x25
    bool             ReactToSelfPermitted;               // +0x74
    bool             AlwaysAwareOf;                      // +0x75
    bool             UnknownThreatSource;                // +0x76
    bool             ReactNonstopFlag;                   // +0x77
    unsigned long    POpinionProduced_[2];               // +0x78 CCountedPointer<COpinionSet> POpinionProduced
    EOpinionDeedType VirtualDeed;                        // +0x80
    long             VirtualDeedID;                      // +0x84
    unsigned long    FrameToClearVirtualDeed;            // +0x88
};
FABLE_STATIC_ASSERT(sizeof(CTCOpinionSource) == 0x8c);
#endif
#endif // FABLE_ENGINE_CTCOPINIONSOURCE_H

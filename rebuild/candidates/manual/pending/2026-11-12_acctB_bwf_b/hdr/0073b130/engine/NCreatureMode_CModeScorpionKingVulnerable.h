#ifndef FABLE_ENGINE_NCREATUREMODE_CMODESCORPIONKINGVULNERABLE_H
#define FABLE_ENGINE_NCREATUREMODE_CMODESCORPIONKINGVULNERABLE_H
// New header (chunk Q2, row 0x008E0A10 FrameUpdate). NCreatureMode::CModeScorpionKingVulnerable : CModeStandBase (Ego_r.pdb layout,
// ghidra_out/struct_layouts_egor.tsv: base 0x68, RecoveryCount +0x68, Filter +0x6C, PCreature +0x78, bRecovering +0x7C, PTarget +0x7D,
// then with the retail 12-byte STLport vector Obstructions +0x88, PEyeEmitter +0x94, PStingEmitter +0x9C, FrameCount +0xA4,
// EffectsDone +0xA8, TailEffectIndex +0xAC, CollisionRadius +0xB0, TailCurlFinished +0xB4; sizeof 0xB8). Vtable 0x0128582C.
// Member functions: Ego_r.pdb / FableWin.pdb spellings (FrameUpdate 0x008E0A10 slot 3, Recover 0x008DF3A0 private, SetTarget 0x008D73E0).
// Needs FABLE_RETAIL_CMODEBASE_VIRTUALS, FABLE_RETAIL_CMODESTANDBASE and FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThingCreatureBase;
namespace NCreatureMode {
class CModeScorpionKingVulnerable : public CModeStandBase {
public:
#ifdef FABLE_RETAIL_NCREATUREMODE_ALLOC
    // Opt-in (row 0x0073B130 CTCCreatureModeManager::RegisterModes): FableWin.pdb ?Alloc@CModeScorpionKingVulnerable@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z.
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);
#endif
    CModeScorpionKingVulnerable(CThing& thing);                          // 0x008D8120
    virtual ~CModeScorpionKingVulnerable();
    virtual unsigned long GetType() const;
    virtual void AnimUpdate(CAnimateThing& animation);
    virtual void FrameUpdate();                                          // 0x008E0A10
    virtual const CAnimFilterBase& GetAnimFilter() const;
    virtual void OnActivate();
    virtual void OnDeactivate();                                         // 0x008D9CA0
    void SetTarget(CThingCreatureBase& target);                          // 0x008D73E0
private:
    void Recover();                                                      // 0x008DF3A0
    long RecoveryCount;                                                  // +0x68
    CAnimFilterOfGroup Filter;                                           // +0x6c
    CThingCreatureBase* PCreature;                                       // +0x78
    bool bRecovering;                                                    // +0x7c
    CIntelligentPointer<CThingCreatureBase> PTarget;                     // +0x7d
    std::vector<CIntelligentPointer<CThing> > Obstructions;              // +0x88
    CIntelligentPointer<CThing> PEyeEmitter;                             // +0x94
    CIntelligentPointer<CThing> PStingEmitter;                           // +0x9c
    long FrameCount;                                                     // +0xa4
    bool EffectsDone;                                                    // +0xa8
    long TailEffectIndex;                                                // +0xac
    float CollisionRadius;                                               // +0xb0
    bool TailCurlFinished;                                               // +0xb4
    static void CheckRetailLayout();
};
inline void CModeScorpionKingVulnerable::CheckRetailLayout() {
    FABLE_STATIC_ASSERT(sizeof(CModeScorpionKingVulnerable) == 0xb8);
    FABLE_STATIC_ASSERT(offsetof(CModeScorpionKingVulnerable, PCreature) == 0x78);
    FABLE_STATIC_ASSERT(offsetof(CModeScorpionKingVulnerable, PTarget) == 0x7d);
    FABLE_STATIC_ASSERT(offsetof(CModeScorpionKingVulnerable, FrameCount) == 0xa4);
    FABLE_STATIC_ASSERT(offsetof(CModeScorpionKingVulnerable, TailCurlFinished) == 0xb4);
}
}
#endif

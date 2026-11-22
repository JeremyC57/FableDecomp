#ifndef FABLE_ENGINE_NCREATUREMODE_CMODESCREAMERSUPERATTACK_H
#define FABLE_ENGINE_NCREATUREMODE_CMODESCREAMERSUPERATTACK_H
// New header (chunk Q19, row 0x008E16A0 FrameUpdate): NCreatureMode::CModeScreamerSuperAttack : CModeStandBase (Ego_r.pdb layout,
// ghidra_out/struct_layouts_egor.tsv: base 0x68, Filter CAnimFilterOfGroup +0x68, PTarget CIntelligentPointer<const CThing> +0x74,
// PLightningBeamParticleEmitter CIntelligentPointer<CThing> +0x7C; sizeof 0x84). Member functions: Ego_r.pdb spellings
// (??0CModeScreamerSuperAttack@NCreatureMode@@QAE@AAVCThing@@ABV2@@Z, ?FrameUpdate@..@@UAEXXZ, ?OnActivate@..@@UAEXXZ,
// ?OnDeactivate@..@@UAEXXZ); vtable 0x01278338: slot 0 the deleting dtor 0x008AC4B0, 1 GetType 0x008AC490, 3 FrameUpdate 0x008E16A0,
// 5 OnActivate 0x008D8330, 6 OnDeactivate 0x008D79A0 (the CModeGuildmasterPowerUpShield vtable shape). Needs
// FABLE_RETAIL_CMODEBASE_VIRTUALS, FABLE_RETAIL_CMODESTANDBASE and FABLE_RETAIL_CINTELLIGENTPOINTER. No default path.
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
class CModeScreamerSuperAttack : public CModeStandBase {
public:
    CModeScreamerSuperAttack(CThing& thing, const CThing& target);
    virtual ~CModeScreamerSuperAttack();
    virtual unsigned long GetType() const;                               // 0x008AC490
    virtual void FrameUpdate();                                          // 0x008E16A0
    virtual void OnActivate();                                           // 0x008D8330
    virtual void OnDeactivate();                                         // 0x008D79A0
private:
    CAnimFilterOfGroup Filter;                                           // +0x68
    CIntelligentPointer<const CThing> PTarget;                           // +0x74
    CIntelligentPointer<CThing> PLightningBeamParticleEmitter;           // +0x7c
    static void CheckRetailLayout();
};
inline void CModeScreamerSuperAttack::CheckRetailLayout() {
    FABLE_STATIC_ASSERT(sizeof(CModeScreamerSuperAttack) == 0x84);
    FABLE_STATIC_ASSERT(offsetof(CModeScreamerSuperAttack, PTarget) == 0x74);
    FABLE_STATIC_ASSERT(offsetof(CModeScreamerSuperAttack, PLightningBeamParticleEmitter) == 0x7c);
}
}
#endif

#ifndef FABLE_ENGINE_CTCHERO_H
#define FABLE_ENGINE_CTCHERO_H
// CTCHero (new, chunk 765): RTTI .?AVCTCHero@@, base CTCBase at +0; retail vtable 0x0125EB54. Ego_r.pdb members
// (struct_layouts_egor.tsv, sizeof 0x38): WeaponsUsable +0x0C .. WillUseOverriddenByScripts +0x0F, Title +0x10,
// HeroTitleObjectDef +0x14, PHeroTitleSubDef +0x18, OnBoastingPodium +0x1C, LastWeaponEquipped +0x20,
// LastWeaponEquippedID +0x28, PTeleportParticle +0x30. Only the methods rows use are modelled (no members yet).
// Ego_r.pdb order: ?ProcessPendingTutorials@CTCHero@@QAEXXZ 0x00700980 then ?FrameUpdate@CTCHero@@UAEXXZ 0x00700990;
// retail 0x006C9FC0 / 0x006C9FD0 (vtable slot 9) carry the same `call Get ; mov ecx,eax ; jmp` body.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
class CTCHero : public CTCBase {
public:
    void ProcessPendingTutorials();                                      // 0x006C9FC0
    virtual void FrameUpdate();                                          // slot 9 0x006C9FD0
};
#endif

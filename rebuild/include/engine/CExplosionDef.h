#ifndef FABLE_ENGINE_CEXPLOSIONDEF_H
#define FABLE_ENGINE_CEXPLOSIONDEF_H
#include "engine/CSubDefClassBase.h"
// Original TPI0x314e9; native field ExplosionShouldKnockdown64, size70 after vector reduction.
class CExplosionDef : public CSubDefClassBase {
public:
#ifdef FABLE_RETAIL_CEXPLOSIONDEF_MEMBERS
    // Retail opt-in (wf5, row 0x0094D780 CAIStateGroup_TrollBase::Initialise): the first Ego_r.pdb members at their retail offsets
    // (= the Ego_r.pdb offsets, no shift: CDefClassBase is 0x25 in both, no container precedes them): Damage +0x28, ExplosionRadius +0x2C (`fld dword ptr [ecx+2Ch]` at 0x0094D8CD;
    // the landed 0x00750BE0 reads all three), FireDamage +0x30; and ?StatGetTypeName@CExplosionDef@@SI?AVCCharString@@XZ (inline; the
    // literal the landed GetSubDef<CExplosionDef> 0x00751190 builds). No layout change.
    static CCharString __fastcall StatGetTypeName() { return CCharString("CExplosionDef", -1); }
    unsigned char _pad_0x25[0x3];
    float Damage;                                                // +0x28
    float ExplosionRadius;                                       // +0x2C
    long FireDamage;                                             // +0x30
    unsigned char _pad_0x34[0x30];
#else
    unsigned char _pad_0x25[0x3f];
#endif
    bool ExplosionShouldKnockdown,CauseFireEffectOnHitCreatures,CausesRumble;
    unsigned char _pad_0x67[9];
};
FABLE_STATIC_ASSERT(sizeof(CExplosionDef)==0x70);
#endif

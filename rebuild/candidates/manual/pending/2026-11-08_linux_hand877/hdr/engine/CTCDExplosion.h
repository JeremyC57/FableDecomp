#ifndef FABLE_ENGINE_CTCDEXPLOSION_H
#define FABLE_ENGINE_CTCDEXPLOSION_H
#include "engine/CTCDriverBase.h"
#include "engine/CDefPointer.h"
#include "engine/CExplosionDef.h"
// Original TPI0x315c4; full native78 with PExplosionDef38; four vector reductions.
class CTCDExplosion : public CTCDriverBase {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
public: CTCDExplosion(CThing&);
#endif
private:
    float DamageRadius,PhysicsRadius,MaxDamage;
    long FireDamage,ExplosionLifeTimer;
    unsigned char StartPos[12],EndPos[12];
    CDefPointer<const CExplosionDef> PExplosionDef;
    unsigned char _pad_0x3c[0x3c];
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_EXPLOSION; }
    CDefPointer<const CExplosionDef> GetPExplosionDef() const { return PExplosionDef; }
#ifdef FABLE_RETAIL_CTCDEXPLOSION_ONHIT
    // Retail opt-in (chunk 877, row 0x00839940 CThingGameObject::OnHit): Ego_r.pdb / FableWin.pdb
    // ?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@@Z (0x0080C460, `mov edx,&pos ; mov ecx,type ; call`) and
    // ?SetSpecificThingToHit@CTCDExplosion@@QAEXAAVCThing@@@Z (0x0080CCF0), out of line in retail. Declarations only.
    static CThing* __fastcall Create(long type, const C3DVector& position);   // 0x0080C460
    void SetSpecificThingToHit(CThing& thing);                                 // 0x0080CCF0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCDExplosion)==0x78);
#endif

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
};
FABLE_STATIC_ASSERT(sizeof(CTCDExplosion)==0x78);
#endif

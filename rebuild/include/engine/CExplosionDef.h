#ifndef FABLE_ENGINE_CEXPLOSIONDEF_H
#define FABLE_ENGINE_CEXPLOSIONDEF_H
#include "engine/CSubDefClassBase.h"
// Original TPI0x314e9; native field ExplosionShouldKnockdown64, size70 after vector reduction.
class CExplosionDef : public CSubDefClassBase {
public:
    unsigned char _pad_0x25[0x3f];
    bool ExplosionShouldKnockdown,CauseFireEffectOnHitCreatures,CausesRumble;
    unsigned char _pad_0x67[9];
};
FABLE_STATIC_ASSERT(sizeof(CExplosionDef)==0x70);
#endif

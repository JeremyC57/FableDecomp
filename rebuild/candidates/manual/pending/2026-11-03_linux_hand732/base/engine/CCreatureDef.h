#ifndef FABLE_ENGINE_CCREATUREDEF_H
#define FABLE_ENGINE_CCREATUREDEF_H
#include "engine/CSubDefClassBase.h"
// Raw TPI0x14be3 public CSubDefClassBase base. Native00667df1 reads FlashOnHit+c9.
// Original PWoundMorphs through RespawnWaitInFramesMax and RadiusWeight/Spread tail
// remain opaque at complete retail offsets, after the four STLport container reductions.
class CCreatureDef : public CSubDefClassBase {
public:
    unsigned char _pad_0x25[0xa3];
    bool Short,FlashOnHit,PickPocketable,ChildThatCanRegionFollow;
    bool AllowedInProtectedTowns,UseActualHitPosForHitEffects;
    unsigned char _pad_0xce[0x1a];
};
FABLE_STATIC_ASSERT(sizeof(CCreatureDef)==0xe8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef,FlashOnHit)==0xc9);

#endif

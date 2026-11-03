#ifndef FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#define FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#include "engine/CThingCreatureBase.h"
// Original sole base CThingCreatureBase; native Create006ac91c allocates208.
// Complete original tail ControlPos through MovementBandType is unused here.
class CThingPlayerCreature:public CThingCreatureBase {
private: unsigned char _pad_0x160[0xa8];
public: CThingPlayerCreature();
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==0x208);
#endif

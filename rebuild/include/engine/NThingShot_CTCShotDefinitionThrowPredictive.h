#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONTHROWPREDICTIVE_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONTHROWPREDICTIVE_H
#include "engine/NThingShot_CTCShotDefinitionBase.h"
// Ego_r.pdb: CTCShotDefinitionThrow and CTCShotDefinitionThrowPredictive are 160 bytes like CTCShotDefinitionBase (no own members;
// retail 0x9C, the STLport vector being 4 bytes smaller). ThrowPredictive's constructor 0x004D54B2 calls Throw's 0x004D545A.
// Ego_r ?InitMovement@CTCShotDefinitionThrow@NThingShot@@MAEXXZ / ?OnSubShotTrigger@CTCShotDefinitionThrow@NThingShot@@MAEXXZ /
// ?InitMovement@CTCShotDefinitionThrowPredictive@NThingShot@@MAEXXZ (retail 0x007BC38A, vtable 0x0123CE9C slot 33).
namespace NThingShot {
class CTCShotDefinitionThrow : public CTCShotDefinitionBase {
protected:
    virtual void InitMovement();
    virtual void OnSubShotTrigger();
};
class CTCShotDefinitionThrowPredictive : public CTCShotDefinitionThrow {
protected:
    virtual void InitMovement();
};
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionThrow)==0x9c);
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionThrowPredictive)==0x9c);
#endif
}
#endif

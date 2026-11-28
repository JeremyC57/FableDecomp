#ifndef FABLE_ENGINE_NTHINGSHOT_CTC_SHOT_DEFINITION_DRAGON_FIREBALL_H
#define FABLE_ENGINE_NTHINGSHOT_CTC_SHOT_DEFINITION_DRAGON_FIREBALL_H
#ifdef FABLE_RETAIL_DRAGON_FIREBALL_MOVEMENT_PASS30
#include "engine/NThingShot_CTCShotDefinitionBase.h"
namespace NThingShot {
class CTCShotDefinitionDragonFireball : public CTCShotDefinitionBase {
protected:
    virtual void InitMovement();
};
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionDragonFireball) == sizeof(CTCShotDefinitionBase));
}
#endif
#endif

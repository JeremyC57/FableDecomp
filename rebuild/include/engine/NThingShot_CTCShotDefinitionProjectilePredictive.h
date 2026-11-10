#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONPROJECTILEPREDICTIVE_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONPROJECTILEPREDICTIVE_H
#include "engine/NThingShot_CTCShotDefinitionBase.h"
// RTTI01240ef4: Predictive -> Projectile -> Base at0; PDB sizes168/168/160.
namespace NThingShot {
class CTCShotDefinitionProjectile : public CTCShotDefinitionBase {
protected:
    unsigned char Line1[8]; // native9c, CEnginePrimitiveHandle
};
class CTCShotDefinitionProjectilePredictive : public CTCShotDefinitionProjectile {
protected:
    virtual void InitMovement(); // vtable01240ef4 slot33; Ego_r MAEXXZ
};
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionProjectile)==0xa4);
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionProjectilePredictive)==0xa4);
}
#endif

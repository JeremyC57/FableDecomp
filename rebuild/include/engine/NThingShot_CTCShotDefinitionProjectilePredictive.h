#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONPROJECTILEPREDICTIVE_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONPROJECTILEPREDICTIVE_H
#include "engine/NThingShot_CTCShotDefinitionBase.h"
// RTTI01240ef4: Predictive -> Projectile -> Base at0; PDB sizes168/168/160.
namespace NThingShot {
class CTCShotDefinitionProjectile : public CTCShotDefinitionBase {
protected:
    unsigned char Line1[8]; // native9c, CEnginePrimitiveHandle
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONPROJECTILE_INITMOVEMENT
    // Retail opt-in (hand2b, row 0x007BBB3B): Ego_r.pdb ?InitMovement@CTCShotDefinitionProjectile@NThingShot@@MAEXXZ (vtable 0x01240CCC
    // slot 33) (GetInitialVelocity: see NThingShot_CTCShotDefinitionBase.h).
    virtual void InitMovement();
#endif
};
class CTCShotDefinitionProjectilePredictive : public CTCShotDefinitionProjectile {
protected:
#ifdef FABLE_RETAIL_PREDICTIVE_SUBSHOT_PASS26
    virtual void OnSubShotTrigger();
#endif
    virtual void InitMovement(); // vtable01240ef4 slot33; Ego_r MAEXXZ
};
FABLE_STATIC_ASSERT_AT(14, sizeof(CTCShotDefinitionProjectile)==0xa4);
FABLE_STATIC_ASSERT_AT(15, sizeof(CTCShotDefinitionProjectilePredictive)==0xa4);
}
#endif

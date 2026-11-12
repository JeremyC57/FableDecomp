#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONSPREADSHOTPROJECTILE_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONSPREADSHOTPROJECTILE_H
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"
// wf8 (row 0x007BBE34): NThingShot::CTCShotDefinitionSpreadShotProjectile -> CTCShotDefinitionProjectile -> CTCShotDefinitionBase
// (RTTI vtable 0x01240D84; Ego_r.pdb `@STRUCT CTCShotDefinitionSpreadShotProjectile 168 0`: no members of its own; retail size
// 0xA4, Alloc does operator new(0xA4)). CTCShotDefinitionProjectile comes from the Predictive header.
namespace NThingShot {
class CTCShotDefinitionSpreadShotProjectile : public CTCShotDefinitionProjectile {
protected:
    virtual void InitMovement(); // vtable 0x01240D84 slot 33 = 0x007BBE34; Ego_r.pdb / FableWin.pdb ...@@MAEXXZ
};
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionSpreadShotProjectile)==0xa4);
}
#endif

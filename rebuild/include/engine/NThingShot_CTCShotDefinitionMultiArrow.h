#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONMULTIARROW_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONMULTIARROW_H
// NThingShot::CTCShotDefinitionMultiArrow (new, chunk Q20): RTTI : NThingShot::CTCShotDefinitionBase at 0. Ego_r.pdb members after the
// base (Ego_r 0xA0, retail 0x9C): Acceleration (C3DVector) +0x9C, MaxCosAngleToTurn +0xA8, MaxAngleToTurn +0xAC, StraightArrow +0xB0;
// Ego_r sizeof 184 = retail 0xB4. InitMovement is the protected virtual ?InitMovement@CTCShotDefinitionMultiArrow@NThingShot@@MAEXXZ
// (retail 0x007BCF7F). Needs FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT and engine/C3DVector.h.
#include "engine/NThingShot_CTCShotDefinitionBase.h"
namespace NThingShot {
class CTCShotDefinitionMultiArrow : public CTCShotDefinitionBase {
protected:
    virtual void InitMovement();                                         // 0x007BCF7F
private:
    C3DVector Acceleration;                                              // +0x9c
    float MaxCosAngleToTurn;                                             // +0xa8
    float MaxAngleToTurn;                                                // +0xac
    bool StraightArrow;                                                  // +0xb0
};
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionMultiArrow)==0xb4);
}
#endif

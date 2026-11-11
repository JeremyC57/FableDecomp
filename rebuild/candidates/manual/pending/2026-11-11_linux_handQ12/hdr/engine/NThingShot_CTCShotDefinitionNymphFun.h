#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONNYMPHFUN_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONNYMPHFUN_H
// NThingShot::CTCShotDefinitionNymphFun (new, chunk Q12): RTTI .?AVCTCShotDefinitionNymphFun@NThingShot@@ (ghidra_out/rtti_bases.tsv:
// NymphFun -> CTCShotDefinitionProjectile -> CTCShotDefinitionBase -> CTCBase, all at 0). Ego_r.pdb sizeof 172 (0xac) = the
// projectile's 168 + TrapCreated at PDB +0xa8; retail sits 4 lower (the base's STLport vector is 12 bytes): TrapCreated +0xa4, 0xa8.
// InitMovement is public in this class (Ego_r.pdb / FableWin.pdb ?InitMovement@CTCShotDefinitionNymphFun@NThingShot@@UAEXXZ).
// Includers define FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTHING, FABLE_RETAIL_CINTELLIGENTPOINTER and FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT.
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"
namespace NThingShot {
class CTCShotDefinitionNymphFun : public CTCShotDefinitionProjectile {
public:
    virtual void InitMovement();                                        // 0x007BC004 (slot 33)
protected:
    bool TrapCreated;                                                   // +0xa4
};
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionNymphFun)==0xa8);
}
#endif

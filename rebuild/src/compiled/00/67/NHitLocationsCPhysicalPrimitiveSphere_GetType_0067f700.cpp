// NHitLocations::CPhysicalPrimitiveSphere::GetType() const -- returns (EPhysicalPrimitiveType)1
// Leaf: ?GetType@CPhysicalPrimitiveSphere@NHitLocations@@UBE?AW4EPhysicalPrimitiveType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F700, thiscall, `ret`.
// Identity: slot 3 of NHitLocations::CPhysicalPrimitiveSphere's vtable 0x0125B9FC (defined here: the slot holds this body in 1 vtable(s), all of NHitLocations::CPhysicalPrimitiveSphere or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CPhysicalPrimitiveSphere@NHitLocations@@UBE?AW4EPhysicalPrimitiveType@2@XZ; `ret` matches the signature.
// Header: engine/NHitLocations_CPhysicalPrimitiveSphere.h (opt-ins as in the landed row NHitLocationsCPhysicalPrimitiveSphere_GetClosestPointToPos2D_00814ea0.cpp).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATRADIUS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NHitLocations_CPhysicalPrimitiveSphere.h"
NHitLocations::EPhysicalPrimitiveType NHitLocations::CPhysicalPrimitiveSphere::GetType() const
{
    return (EPhysicalPrimitiveType)1;
}

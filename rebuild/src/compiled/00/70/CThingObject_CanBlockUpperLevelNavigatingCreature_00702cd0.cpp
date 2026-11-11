// CThingObject::CanBlockUpperLevelNavigatingCreature() const -- returns false
// Leaf: ?CanBlockUpperLevelNavigatingCreature@CThingObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702CD0, thiscall, `ret`.
// Identity: slot 56 of CThingObject's vtable 0x01264DCC (defined here: the slot holds this body in 1 vtable(s), all of CThingObject or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 30 TLC-named slots of that vtable puts CanBlockUpperLevelNavigatingCreature(void) at this slot; Ego_r/FableWin name set: ?CanBlockUpperLevelNavigatingCreature@CThingObject@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingObject.h (opt-ins as in the landed row CThingObject_Create_00703210.cpp).
#define FABLE_RETAIL_CTHINGOBJECT_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGOBJECT_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY
#define FABLE_RETAIL_CTCPARAMSPHYSICSBASE_CTOR_BODY
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCParamsPhysicsBase.h"
bool CThingObject::CanBlockUpperLevelNavigatingCreature() const
{
    return false;
}

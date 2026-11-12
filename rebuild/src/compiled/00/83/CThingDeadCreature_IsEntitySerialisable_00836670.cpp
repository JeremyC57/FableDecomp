// CThingDeadCreature::IsEntitySerialisable() const -- returns true
// Leaf: ?IsEntitySerialisable@CThingDeadCreature@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836670, thiscall, `ret`.
// Identity: slot 22 of CThingDeadCreature's vtable 0x01272B04 (defined here: the slot holds this body in 1 vtable(s), all of CThingDeadCreature or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts IsEntitySerialisable(void) at this slot; Ego_r/FableWin name set: ?IsEntitySerialisable@CThingDeadCreature@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingDeadCreature.h (opt-ins as in the landed row CThingDeadCreature_PeekDefinition_00835b90.cpp).
#define FABLE_RETAIL_CTHINGDEADCREATURE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGDEADCREATURE_PEEKDEFINITION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CGameDefinitionManager.h"
bool CThingDeadCreature::IsEntitySerialisable() const
{
    return true;
}

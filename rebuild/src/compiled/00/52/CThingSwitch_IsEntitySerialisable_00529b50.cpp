// CThingSwitch::IsEntitySerialisable() const -- returns true
// Leaf: ?IsEntitySerialisable@CThingSwitch@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529B50, thiscall, `ret`.
// Identity: slot 22 of CThingSwitch's vtable 0x012453FC (defined here: the slot holds this body in 1 vtable(s), all of CThingSwitch or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 28 TLC-named slots of that vtable puts IsEntitySerialisable(void) at this slot; Ego_r/FableWin name set: ?IsEntitySerialisable@CThingSwitch@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingSwitch.h (opt-ins as in the landed row CThingSwitch_PeekDefinition_0083c1e0.cpp).
#define FABLE_RETAIL_CTHINGSWITCH_VSLOTS
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
#define FABLE_RETAIL_CTHINGSWITCH_PEEKDEFINITION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingSwitch.h"
#include "engine/CGameDefinitionManager.h"
bool CThingSwitch::IsEntitySerialisable() const
{
    return true;
}

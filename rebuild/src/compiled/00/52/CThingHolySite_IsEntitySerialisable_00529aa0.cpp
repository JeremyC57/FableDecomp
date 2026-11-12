// CThingHolySite::IsEntitySerialisable() const -- returns true
// Leaf: ?IsEntitySerialisable@CThingHolySite@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529AA0, thiscall, `ret`.
// Identity: slot 22 of CThingHolySite's vtable 0x012452DC (defined here: the slot holds this body in 1 vtable(s), all of CThingHolySite or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts IsEntitySerialisable(void) at this slot; Ego_r/FableWin name set: ?IsEntitySerialisable@CThingHolySite@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingHolySite.h (opt-ins as in the landed row CThingHolySite_ConstructFromLevelScript_0083bba0.cpp).
#define FABLE_RETAIL_CTHINGHOLYSITE_VSLOTS
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
#define FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingHolySite.h"
bool CThingHolySite::IsEntitySerialisable() const
{
    return true;
}

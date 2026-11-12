// CTCVillageMember::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCVillageMember@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAF2B, thiscall, `ret`.
// Identity: slot 23 of CTCVillageMember's vtable 0x0124007C (defined here: the slot holds this body in 1 vtable(s), all of CTCVillageMember or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCVillageMember@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCVillageMember.h (opt-ins as in the landed row CTCVillageMember_OnCreate_007ee370.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCVillageMember.h"
#include "engine/CGameDefinitionManager.h"
bool CTCVillageMember::IsComponentSerialisable() const
{
    return true;
}

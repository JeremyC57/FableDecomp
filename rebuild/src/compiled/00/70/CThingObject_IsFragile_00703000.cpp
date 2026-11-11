// CThingObject::IsFragile() const -- returns the definition's IsFragile flag
// Leaf: ?IsFragile@CThingObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00703000, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 54 of CThingObject's vtable 0x01264DCC (the slot holds this body in 1 vtable(s), owner CThingObject); the Xbox 360 Test PDB virtual order (third source) aligned on 30 TLC-named slots of that vtable puts IsFragile(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsFragile@CThingObject@@UBE_NXZ; `ret` matches the signature.
// Was: CThingObject::vfunc_54 (rebuild/src/compiled/00/70/CThingObject_vfunc54_00703000.cpp).
// Opt-ins as in the landed row CThingObject_SerialiseGameState_007030c0.cpp (the typed PDef and CThingObjectDef).
#define FABLE_RETAIL_CTHINGOBJECT_VSLOTS2
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
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CTHINGOBJECT_DEFINITION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
bool CThingObject::IsFragile() const
{
    return PDef->IsFragile;
}

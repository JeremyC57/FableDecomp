// CThingGameObject::GetThingStandingOn() -- the thing this game object is standing on (non-const overload, virtual)
// Leaf: ?GetThingStandingOn@CThingGameObject@@UAEPAVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529940, 11 bytes: `add ecx,0C0h ; jmp 0x00A01B50` (PThingStandingOn.GetPItem(), tail call).
// Identity (two sources): slot 38 of CThingGameObject's vtable 0x01244F7C (and of every derived class vtable that does not override it:
//   13 .rdata references); Ego_r.pdb 0x00532720 / FableWin.pdb 0x01D3E6B0 ?GetThingStandingOn@CThingGameObject@@UAEPAVCThing@@XZ, emitted
//   after the const overload (FableWin 0x01D3E690 = retail 0x00529930, landed), and VC7.1's reverse-declaration vtable order puts this
//   non-const overload at slot 38 (engine/CThingGameObject.h FABLE_RETAIL_CTHINGGAMEOBJECT_STANDINGON). Byte-identical to 0x00529930 but a
//   separate function (two addresses, two vtable slots).
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50.
// Was: CThingGameObject::vfunc_38 on a local CThingGameObject model (?vfunc_38@CThingGameObject@@UAEPAVCBaseObject@@XZ,
//   rebuild/src/compiled/00/52/CThingGameObject_vfunc38_00529940.cpp).
// Header: engine/CThingGameObject.h (FABLE_RETAIL_CTHINGGAMEOBJECT + FABLE_RETAIL_CTHINGGAMEOBJECT_STANDINGON), as the twin 0x00529930.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STANDINGON
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
CThing* CThingGameObject::GetThingStandingOn()
{
    return PThingStandingOn.Get();
}
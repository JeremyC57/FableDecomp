// CThingGameObject::GetThingStandingOn() const -- the thing this game object is standing on (const view, virtual)
// Leaf: ?GetThingStandingOn@CThingGameObject@@UBEPBVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529930, 11 bytes: `add ecx,0C0h ; jmp 0x00A01B50` (PThingStandingOn.GetPItem(), tail call).
// Identity (two sources): slot 39 of CThingGameObject's vtable 0x01244F7C (rtti-vtables.tsv; the catalog's "vfunc_39"), next to slot 37
//   SetThingStandingOn 0x005298E0 and slot 38 0x00529940 (byte-identical); the PDB pair ?GetThingStandingOn@CThingGameObject@@UBEPBVCThing@@XZ /
//   UAEPAVCThing@@XZ is emitted const-first in FableWin (0x01D3E690 / 0x01D3E6B0) as retail emits 0x00529930 before 0x00529940, and
//   VC7.1's reverse-declaration vtable order then puts the const overload at slot 39 (CThing's own pair: slots 38 / 39 = 0x004CD260 /
//   0x004CD250, both `xor eax,eax ; ret`). PThingStandingOn is the CIntelligentPointer<CThing> at +0xC0 (engine/CThingGameObject.h).
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50.
// Header: engine/CThingGameObject.h (FABLE_RETAIL_CTHINGGAMEOBJECT + FABLE_RETAIL_CTHINGGAMEOBJECT_STANDINGON).
// Twin: 0x00529940 (landed as "CThingGameObject::vfunc_38") is the non-const overload (NAME_FIXES.tsv).
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
const CThing* CThingGameObject::GetThingStandingOn() const
{
    return PThingStandingOn.Get();
}
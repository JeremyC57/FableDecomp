// CGameScriptInterface::CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const -- the sensing test in mode 1
// Leaf: ?CanThingBe_Seen_ByOtherThing@CGameScriptInterface@@UBE_NABVCScriptThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089CD10, 20 bytes, `ret 8`: pushes 1 and the two things, calls the private
// CanThingBe_Sensed_ByOtherThing (0x0089C910, ret 12).
// Identity (NF42, round 5): Ego_r.pdb 0x008db6a0 ?CanThingBe_Seen_ByOtherThing@CGameScriptInterface@@UBE_NABVCScriptThing@@0@Z (vtable slot 632,
// engine/CGameScriptInterface.h); the callee is Ego_r 0x008DB0B0 ?CanThingBe_Sensed_ByOtherThing@CGameScriptInterface@@ABE_NABVCScriptThing@@0J@Z
// (private const; header opt-in FABLE_RETAIL_SENSED_PASS23). Was a method view `CGameScriptInterface_Methods` with a placeholder callee;
// now on the shared header.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SENSED_PASS23
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::CanThingBe_Seen_ByOtherThing(const CScriptThing& thing, const CScriptThing& other) const
{
    return CanThingBe_Sensed_ByOtherThing(thing, other, 1);
}

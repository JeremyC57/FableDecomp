// FoundMessageIsThingHitBy(const CScriptThing& thing, const CScriptThing& hitter) -- true when the script thing has a pending "hit by" message from the named hitter
// Leaf: ?FoundMessageIsThingHitBy@@YI_NABVCScriptThing@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE0C7, 22 bytes (`ret`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?FoundMessageIsThingHitBy@@YI_NABVCScriptThing@@0@Z`.
// Callees: CScriptThing slots 1 GetName (+0x04) and 21 MsgIsHitBy (+0x54).
// Shape: a tail of two virtual calls; `#pragma optimize("y", off)` only keeps the TU's frame setting (no frame here).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString {
public:
    void* m_pStringData;
};
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

bool __fastcall FoundMessageIsThingHitBy(const CScriptThing& thing, const CScriptThing& hitter)
{
    return thing.MsgIsHitBy(hitter.GetName());
}
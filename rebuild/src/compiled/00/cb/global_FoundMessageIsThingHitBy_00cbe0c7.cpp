// FoundMessageIsThingHitBy(const CScriptThing& thing, const CScriptThing& hitter) -- true when the script thing has a pending "hit by" message from the named hitter
// Leaf: ?FoundMessageIsThingHitBy@@YI_NABVCScriptThing@@0@Z
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
class C3DVector {
public:
    float X, Y, Z;
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

bool __fastcall FoundMessageIsThingHitBy(const CScriptThing& thing, const CScriptThing& hitter)
{
    return thing.MsgIsHitBy(hitter.GetName());
}
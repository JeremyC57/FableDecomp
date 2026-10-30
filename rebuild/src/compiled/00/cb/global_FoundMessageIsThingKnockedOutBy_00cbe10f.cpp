// FoundMessageIsThingKnockedOutBy(const CScriptThing& thing, const CScriptThing& attacker) -- true when both things are alive and the thing has a pending "knocked out by" message from the attacker
// Leaf: ?FoundMessageIsThingKnockedOutBy@@YI_NABVCScriptThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE10F, 65 bytes (`ret`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?FoundMessageIsThingKnockedOutBy@@YI_NABVCScriptThing@@0@Z`.
// Callees: CScriptThing slots 75 IsAlive (+0x12C), 1 GetName (+0x04), 32 MsgIsKnockedOutBy (+0x80).
// Shape: `return (a && b && c) ? true : false;` gives retail's `mov al,1 ; jmp ; xor al,al` (a plain `return a && b
//   && c;` gives `xor eax,eax ; inc eax`).
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

bool __fastcall FoundMessageIsThingKnockedOutBy(const CScriptThing& thing, const CScriptThing& attacker)
{
    return (thing.IsAlive() && attacker.IsAlive() && thing.MsgIsKnockedOutBy(attacker.GetName())) ? true : false;
}
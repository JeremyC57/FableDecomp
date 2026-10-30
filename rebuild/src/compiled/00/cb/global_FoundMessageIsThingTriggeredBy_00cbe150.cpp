// FoundMessageIsThingTriggeredBy(const CScriptThing& thing, const CScriptThing& trigger) -- true when neither thing is null, both are alive and the thing has a pending "triggered by" message from the trigger
// Leaf: ?FoundMessageIsThingTriggeredBy@@YI_NABVCScriptThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE150, 90 bytes (`ret`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?FoundMessageIsThingTriggeredBy@@YI_NABVCScriptThing@@0@Z`.
// Callees: CScriptThing slots 77 IsNull (+0x134), 75 IsAlive (+0x12C), 1 GetName (+0x04), 17 MsgIsTriggeredBy (+0x44).
// Shape: as 0x00CBE10F, five-term `? true : false`.
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

bool __fastcall FoundMessageIsThingTriggeredBy(const CScriptThing& thing, const CScriptThing& trigger)
{
    return (!thing.IsNull() && !trigger.IsNull() && thing.IsAlive() && trigger.IsAlive() && thing.MsgIsTriggeredBy(trigger.GetName())) ? true : false;
}
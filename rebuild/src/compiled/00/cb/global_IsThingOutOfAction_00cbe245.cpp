#pragma optimize("y", off)
#pragma optimize("s", on)
// IsThingOutOfAction(const CScriptThing& thing) -- true when the script thing is not alive, has been killed or knocked out (by anything) or is unconscious (the complement of IsThingActive 0x00CBE1AA)
// Leaf: IsThingOutOfAction
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBE245, length 155 bytes, `ret`; `__fastcall` (thing in ecx);
// frame-pointer size TU: `optimize("y",off)` + `("s",on)`.
// Catalog label "CV_FisticuffsClubScript::IsKnotholeGladeNotAPunchClub" is a wrong propagation (byte twin of 0x00CBE1AA
// with the result inverted): one of the global script-thing helpers between FoundMessageIsThingTriggeredBy (0x00CBE150)
// and IsThingAwareOfHero (0x00CBE2E0). The real name is not known (descriptive).
// Callees: CScriptThing (vtable 0x01238C8C) slots 75 IsAlive (+0x12c), 18 MsgIsKilledBy (+0x48), 32 MsgIsKnockedOutBy
// (+0x80), 61 IsUnconscious (+0xf4); CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the empty literal
// @ 0x0122D70E, twice), ~CCharString @ 0x0099EAE0.
// Shape: one `||` chain; the "" arguments are IMPLICIT CCharString conversion temporaries (retail's `lea ecx ; push ecx`)
// tracked by flag bits in ebx; the chain lands in a `char` frame byte whose implicit conversion to the bool result
// gives retail's `cmp byte ptr [ebp-1],0 ; setne al` (a bool local is returned with a plain `mov al`).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
bool __fastcall IsThingOutOfAction(const CScriptThing& thing)
{
    char result = !thing.IsAlive() || thing.MsgIsKilledBy("") || thing.MsgIsKnockedOutBy("") || thing.IsUnconscious();
    return result;
}
// CScreenFilterSThingByPass::CScreenFilterSThingByPass(const CScreenFilterSThingByPass&) -- copy the script thing (its handle copy ctor), then the bypass flag
// Leaf: ??0CScreenFilterSThingByPass@@QAE@ABU0@@Z
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD2AE2, 27 bytes, thiscall, `ret 4`: `push esi ; push edi ; mov edi,[esp+0Ch] ;
//   push edi ; mov esi,ecx ; call 0x004ABE90 ; mov al,[edi+0Ch] ; mov [esi+0Ch],al ; pop edi ; mov eax,esi ; pop esi`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 452 callee "0xCD2AE2 CScreenFilterSThingByPass copy ctor"; hand452 README
//   "operator=" -> copy ctor): Ego_r.pdb CScreenFilterSThingByPass (struct, 16 bytes) = CScriptThing thing +0, bool
//   bypassSet +0xC; the callee 0x004ABE90 is the landed CScriptThing copy constructor (Ego_r ??0CScriptThing@@QAE@ABV0@@Z);
//   callers are placement copies: the landed vector<CScreenFilterSThingByPass>::push_back 0x00CD3E7B (calls at 0x00CD3E2B /
//   0x00CD3E90, which already binds ??0CScreenFilterSThingByPass@@QAE@ABU0@@Z -> 00cd2ae2) and the uninitialized-copy loop
//   0x00CD2E65 (0x00CD2E5F). It returns `this` and has no operator= return path: the landed CTavernTableDef::operator= label
//   is propagated.
// Model: the push_back row's CScriptThing (a class with an out-of-line copy ctor; engine/CScriptThing.h is data-only).
//   The compiler-generated copy constructor is spelled out so this TU emits it; the bytes are the implicit member-wise copy.
#pragma optimize("s", on)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
struct CScreenFilterSThingByPass {                      // 16 bytes
    CScriptThing thing;                                 // +0x00
    bool         bypassSet;                             // +0x0c
    CScreenFilterSThingByPass(const CScreenFilterSThingByPass& other);
};

CScreenFilterSThingByPass::CScreenFilterSThingByPass(const CScreenFilterSThingByPass& other)
    : thing(other.thing), bypassSet(other.bypassSet)
{
}

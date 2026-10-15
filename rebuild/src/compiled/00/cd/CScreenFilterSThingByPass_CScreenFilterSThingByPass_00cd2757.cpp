// CScreenFilterSThingByPass::CScreenFilterSThingByPass(const CScriptThing& thing, bool bypassSet) -- copies the script thing and keeps the bypass flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2757, length 25 bytes.
// Leaf: ??0CScreenFilterSThingByPass
// Retail bytes: push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CScriptThing::CScriptThing(const CScriptThing&) ; mov al,[esp+0Ch] ;
//               mov [esi+0Ch],al ; mov eax,esi ; pop esi ; ret 8
// Callee: the CScriptThing copy constructor @ 0x004ABE90 (out of line). Layout (PDB): CScriptThing thing +0 (12 bytes),
// bool bypassSet +0xc. Called from the cutscene-macro code (0x00CD0FFC, 0x00CD10EE: a stack record built from a script thing
// and the literal 1). Shape: the pass-through `push dword ptr [esp+8]` of the reference argument marks a size-optimised
// translation unit (`#pragma optimize("s", on)`).
#pragma optimize("s", on)

#include "engine/CScriptThing.h"

struct CScriptThingRetail {                    // CScriptThing: vptr + PImp (counted pointer), 12 bytes
    virtual void v00();
    void* PImp[2];
    CScriptThingRetail(const CScriptThingRetail& other);   // 0x004ABE90
};
FABLE_STATIC_ASSERT(sizeof(CScriptThingRetail) == sizeof(CScriptThing));

struct CScreenFilterSThingByPass {
    CScriptThingRetail thing;                  // +0x00
    bool         bypassSet;                    // +0x0c
    CScreenFilterSThingByPass(const CScriptThingRetail& thing, bool bypassSet);
};

CScreenFilterSThingByPass::CScreenFilterSThingByPass(const CScriptThingRetail& thing_, bool bypassSet_)
    : thing(thing_), bypassSet(bypassSet_)
{
}
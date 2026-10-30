// Alloc_CVtable_012ec138(void* owner, void* thing) -- script-module __fastcall factory: new CVtable_012ec138(owner, thing) with the constructor inlined (a CCharString member at +0x50)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E92900, length 64 bytes.
// Callees: operator new @ 0x00BFEA1A, CScriptBase::CScriptBase @ 0x00CB8110 (the script module's root
// constructor -- the manifest's "CHeroMorphDef"/"CActionEventRemoveObject::Alloc" labels are propagated),
// CCharString::CCharString() @ 0x0099E4B0. Vtable 0x012ec138 has no RTTI locator (quest-script module), so
// the class is named CVtable_012ec138 like the landed factory family (see 00d67740).
// Shape: ecx = owner, edx = thing; sizeof(CVtable_012ec138) = 0x54 bytes is allocated, the block is
// null-checked, then the inlined constructor runs: root CScriptBase() on this, thing at +0x40 and owner
// at +0x44 stored BEFORE this class's vtable (so they belong to an inlined abstract intermediate base,
// __declspec(novtable), whose own vtable store is dead), then the vtable store and the out-of-line
// default construction of the CCharString at +0x50. Returns the new object (0 when new failed).
#include "fable_string.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {   // abstract: no vtable store of its own
    void* m_pThing;                         // +0x40
    void* m_pOwner;                         // +0x44
    CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};

struct CVtable_012ec138 : CScriptObject {         // vtable 0x012ec138
    unsigned char _pad[0x50 - 0x48];        // +0x48..+0x50
    CCharString   m_name;                   // +0x50
    unsigned char _tail[0x54 - 0x50 - 4];
    virtual void v00();
    CVtable_012ec138(void* owner, void* thing) : CScriptObject(owner, thing), m_name() { }
};

CVtable_012ec138* __fastcall Alloc_CVtable_012ec138(void* owner, void* thing)
{
    return new CVtable_012ec138(owner, thing);
}
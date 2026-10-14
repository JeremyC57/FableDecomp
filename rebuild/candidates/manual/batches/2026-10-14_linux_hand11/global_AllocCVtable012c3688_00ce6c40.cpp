// Alloc_CVtable_012c3688(void* owner, void* thing) -- script-module __fastcall factory: new CVtable_012c3688(owner, thing) with the constructor inlined (a CCharString member at +0x48)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE6C40, length 64 bytes.
// Callees: operator new @ 0x00BFEA1A, CScriptBase::CScriptBase @ 0x00CB8110 (the script module's root
// constructor -- the manifest's "CHeroMorphDef"/"CActionEventRemoveObject::Alloc" labels are propagated),
// CCharString::CCharString() @ 0x0099E4B0. Vtable 0x012c3688 has no RTTI locator (quest-script module), so
// the class is named CVtable_012c3688 like the landed factory family (see 00d67740).
// Shape: ecx = owner, edx = thing; sizeof(CVtable_012c3688) = 0x50 bytes is allocated, the block is
// null-checked, then the inlined constructor runs: root CScriptBase() on this, thing at +0x40 and owner
// at +0x44 stored BEFORE this class's vtable (so they belong to an inlined abstract intermediate base,
// __declspec(novtable), whose own vtable store is dead), then the vtable store and the out-of-line
// default construction of the CCharString at +0x48. Returns the new object (0 when new failed).
#include "fable_string.h"

struct CScriptBase {
    virtual void v00();
    unsigned char _body[0x3c];              // +0x04..+0x40
    CScriptBase();                          // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {   // abstract: no vtable store of its own
    void* m_pThing;                         // +0x40
    void* m_pOwner;                         // +0x44
    CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};

struct CVtable_012c3688 : CScriptObject {         // vtable 0x012c3688
    CCharString   m_name;                   // +0x48
    unsigned char _tail[0x50 - 0x48 - 4];
    virtual void v00();
    CVtable_012c3688(void* owner, void* thing) : CScriptObject(owner, thing), m_name() { }
};

CVtable_012c3688* __fastcall Alloc_CVtable_012c3688(void* owner, void* thing)
{
    return new CVtable_012c3688(owner, thing);
}

// CVtable_012E8080::CVtable_012E8080(void* owner, void* thing) -- script-object constructor: inlined intermediate base (CScriptBase(), owner at +0x44, thing at +0x40), own vtable, a CCharString at +0x88
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E68F70, length 40 bytes.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (the script module's root constructor, see
// GOTCHAS -- the manifest's "CHeroMorphDef" label is propagated), CCharString::CCharString()
// @ 0x0099E4B0. The vtable 0x012E8080 has no RTTI locator (quest-script module), so the class is
// named CVtable_012E8080 as the family landings do.
// Shape: the base runs on `this`, the first argument is stored at +0x44 and the second at +0x40,
// BOTH before this class's vtable store -- init-list members of this class would leave the
// vtable store between them, so the two pointers belong to an inlined intermediate base
// constructor whose own vtable store is dead (abstract, `__declspec(novtable)`; the same
// script-object base that the landed CVtable_012f55b8 family stores its argument at +0x40 for).
// Then the vtable is stored and the name at +0x88 is default-constructed out of line.
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

struct CVtable_012E8080 : CScriptObject {       // vtable 0x012E8080
    unsigned char _pad[0x88 - 0x48];        // +0x48..+0x88
    CCharString   m_name;                   // +0x88
    virtual void v00();
    CVtable_012E8080(void* owner, void* thing);
};

CVtable_012E8080::CVtable_012E8080(void* owner, void* thing)
    : CScriptObject(owner, thing)
    , m_name()
{
}
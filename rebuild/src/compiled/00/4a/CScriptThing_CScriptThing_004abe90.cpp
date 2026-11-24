// CScriptThing::CScriptThing(const CScriptThing& other) -- copy constructor: share the other handle's implementation and add a reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ABE90, 33 bytes. Vtable 0x01238C8C = CScriptThing (rtti-vtables.tsv).
// No callees. Callers: the CScriptThing::MsgWho*() family (0x004ABE60 calls it out of line for its null case).
//
// Shape: `mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],vftable ; copy +4 ; copy +8 ; test ; je ; inc [ecx] ; ret 4`.
// CScriptThing (PDB: 0xc bytes) = CBase (vptr only, out-of-line virtual destructor 0x0099A2E0) +
// CCountedPointer<CScriptThing> PImp {Data +4, Info +8}; the count block's first word is the reference
// count. The vtable store comes FIRST because the copied words belong to the PImp member object
// whose inline copy constructor runs after the (dead, dropped) CBase vtable store and is scheduled
// after the CScriptThing one. The engine header flattens the vptr into a plain `__vftable` word and
// cannot express a constructor, so the class is modelled locally with the header's member names
// (same model as the landed deleting destructor 0x004AB170).
// The Leaf names the full copy-constructor symbol (the TU also emits the vtable's scalar deleting destructor).
// One definition (NF328, 2026-11-24, the NF326 Q4 follow-up unit): 25 landed script rows emit this copy constructor pick-any; a strong
//   definition here is LNK2005 against them, so this row defines it `inline` and emits it pick-any through the helper below.
// Leaf: ??0CScriptThing@@QAE@ABV0@@Z

class CCPPointerInfo { public:
    long m_references;                       // +0 reference count (bumped with `inc dword ptr [ecx]`)
};

struct CBase {
    virtual ~CBase();                        // 0x0099A2E0
};

class CScriptThing;

struct CCountedPointer_CScriptThing {
    CScriptThing* Data;                      // +0
    CCPPointerInfo* Info;                    // +4
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};

class CScriptThing : public CBase { public:
    CCountedPointer_CScriptThing PImp;       // +0x04
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing();
};

inline CScriptThing::CScriptThing(const CScriptThing& other)
    : PImp(other.PImp)
{
}

namespace {
// Inline-budget: scaffolding, not retail: returning a copy by value calls the inline copy constructor above, and with inline expansion
// switched off for this one function this object emits ??0CScriptThing@@QAE@ABV0@@Z out of line as a pick-any COMDAT (retail's
// 0x004ABE90) like the script TUs that expand or emit it (the nf86 / nf88 / NF326 pattern; the address of a constructor cannot be
// taken). Internal to this TU; never called.
#pragma inline_depth(0)
CScriptThing ReferenceScriptThingCopy(const CScriptThing& thing)
{
    return thing;
}
#pragma inline_depth()
}
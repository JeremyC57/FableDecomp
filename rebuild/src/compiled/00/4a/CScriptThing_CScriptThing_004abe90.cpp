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
// Leaf `??0CScriptThing`: the TU also emits the vtable's scalar deleting destructor.
// Leaf: ??0CScriptThing

struct CCPPointerInfo {
    long m_references;                       // +0 reference count (bumped with `inc dword ptr [ecx]`)
};

struct CBase {
    virtual ~CBase();                        // 0x0099A2E0
};

struct CScriptThing;

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

struct CScriptThing : CBase {
    CCountedPointer_CScriptThing PImp;       // +0x04
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing();
};

CScriptThing::CScriptThing(const CScriptThing& other)
    : PImp(other.PImp)
{
}
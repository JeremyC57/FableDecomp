// CTCActionUseReadable::CTCActionUseReadable(CThing& owner) -- base CTCActionUseBase(owner) out of line, this class's vtable, a null PDef and a default-constructed TextTag wide string
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA9D7, length 36 bytes.
// Leaf: ??0CTCActionUseReadable
// Callees: CTCActionUseBase::CTCActionUseBase(CThing&) @ 0x004DA939 (Ego_r.pdb `??0CTCActionUseBase@@QAE@AAVCThing@@@Z`),
// CWideString::CWideString() @ 0x0099AED0 (the manifest's "CCharString::CCharString" is the ICF-shared default constructor).
// Vtable 0x0123F234 = CTCActionUseReadable (rtti-vtables.tsv). Ego_r.pdb CTCActionUseReadable (0x1c bytes): CDefPointer
// <CReadableDef const> PDef +0x14, CWideString TextTag +0x18.
// Shape: the size-optimised factory module (`push dword ptr [esp+8]` pass-through, `and dword ptr [..],0`):
// `#pragma optimize("s", on)`. The vtable store precedes both members because both are member objects with their own
// constructors (the def pointer's inline zeroing, the string's out-of-line constructor).
#pragma optimize("s", on)
class CThing;
struct CDefPointee;

class CWideString { public:
    void* m_pData;
    CWideString();                             // 0x0099AED0
};

struct CDefPointer {                           // CDefPointer<CReadableDef const>
    CDefPointee* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCActionUseBase { public:                      // vtable 0x0123F09C; 0x14 bytes
    virtual void v00();
    unsigned char _body[0x10];                 // +0x04..+0x14 (ParentThing, flags, PActionUseDef, Usable)
    CTCActionUseBase(CThing& owner);           // 0x004DA939
};

class CTCActionUseReadable : public CTCActionUseBase { public:   // vtable 0x0123F234
    CDefPointer PDef;                          // +0x14
    CWideString TextTag;                       // +0x18
    virtual void v00();
    CTCActionUseReadable(CThing& owner);
};

CTCActionUseReadable::CTCActionUseReadable(CThing& owner)
    : CTCActionUseBase(owner)
{
}
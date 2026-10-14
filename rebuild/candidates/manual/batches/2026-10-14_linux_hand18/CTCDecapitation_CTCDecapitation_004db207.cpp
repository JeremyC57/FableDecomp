// CTCDecapitation::CTCDecapitation(CThing*) -- base CTCBase(thing), own vtable, Decapitated = false, null def pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB207, length 32 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (vtable 0x0125BE0C = CTCBase; manifest label propagated).
// Vtable 0x01240534 = CTCDecapitation (rtti-vtables.tsv). PDB CTCDecapitation (0x14 bytes):
// Decapitated +0xc, PDef (CDefPointer<const CDecapitationDef>, one pointer) +0x10. The def pointer
// is nulled with `and dword ptr [esi+10h],0`, the size-optimised zero store, so the TU is /Os.
#pragma optimize("s", on)
struct CThing;
struct CDecapitationDef_Calls;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

template<class T> struct CDefPointer {
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

struct CTCDecapitation : CTCBase_Calls {
    bool Decapitated;                                       // +0x0c
    CDefPointer<const CDecapitationDef_Calls> PDef;         // +0x10
    virtual void v00();                                     // own vtable 0x01240534
    CTCDecapitation(CThing* parent);
};

CTCDecapitation::CTCDecapitation(CThing* parent)
    : CTCBase_Calls(parent)
    , Decapitated(false)
    , PDef()
{
}

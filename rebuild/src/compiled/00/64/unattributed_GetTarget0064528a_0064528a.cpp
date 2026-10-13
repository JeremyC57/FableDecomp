// CClass_0064528a::GetTarget_0064528a() -- returns m_pointer.Method_006445ba() for the smart pointer at +0x48 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064528A, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x48 ; jmp CBaseIntelligentPointer::Method_006445ba
// Virtual getter that forwards to Method_006445ba @ 0x006445BA on the embedded smart pointer at
// +0x48: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_006445ba();               // 0x006445BA
};

struct CClass_0064528a {
    virtual void v00();
    unsigned char _pad[0x44];
    CBaseIntelligentPointer m_pointer;     // +0x48
    virtual CBaseObject* GetTarget_0064528a();
};

CBaseObject* CClass_0064528a::GetTarget_0064528a()
{
    return m_pointer.Method_006445ba();
}
// CClass_00787c1b::GetTarget_00787c1b() -- returns m_pointer.Method_0048da30() for the smart pointer at +0x38 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787C1B, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x38 ; jmp CBaseIntelligentPointer::Method_0048da30
// Virtual getter that forwards to Method_0048da30 @ 0x0048DA30 on the embedded smart pointer at
// +0x38: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_0048da30();               // 0x0048DA30
};

struct CClass_00787c1b {
    virtual void v00();
    unsigned char _pad[0x34];
    CBaseIntelligentPointer m_pointer;     // +0x38
    virtual CBaseObject* GetTarget_00787c1b();
};

CBaseObject* CClass_00787c1b::GetTarget_00787c1b()
{
    return m_pointer.Method_0048da30();
}

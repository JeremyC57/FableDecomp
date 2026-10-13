// CClass_006b8410::GetTarget_006b8410() -- returns m_pointer.Method_00881210() for the smart pointer at +0x90 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8410, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x90 ; jmp CBaseIntelligentPointer::Method_00881210
// Virtual getter that forwards to Method_00881210 @ 0x00881210 on the embedded smart pointer at
// +0x90: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_00881210();               // 0x00881210
};

struct CClass_006b8410 {
    virtual void v00();
    unsigned char _pad[0x8c];
    CBaseIntelligentPointer m_pointer;     // +0x90
    virtual CBaseObject* GetTarget_006b8410();
};

CBaseObject* CClass_006b8410::GetTarget_006b8410()
{
    return m_pointer.Method_00881210();
}
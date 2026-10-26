// CClass_00644644::GetTarget_00644644() -- returns m_pointer.Method_006445ba() for the smart pointer at +0x28 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644644, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x28 ; jmp CBaseIntelligentPointer::Method_006445ba
// Virtual getter that forwards to Method_006445ba @ 0x006445BA on the embedded smart pointer at
// +0x28: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_006445ba();               // 0x006445BA
};

struct CClass_00644644 {
    virtual void v00();
    unsigned char _pad[0x24];
    CBaseIntelligentPointer m_pointer;     // +0x28
    virtual CBaseObject* GetTarget_00644644();
};

CBaseObject* CClass_00644644::GetTarget_00644644()
{
    return m_pointer.Method_006445ba();
}
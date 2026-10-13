// CClass_00c309e0::GetTarget_00c309e0() -- returns m_pointer.Method_00c0ba40() for the smart pointer at +0x4 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C309E0, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x4 ; jmp CBaseIntelligentPointer::Method_00c0ba40
// Virtual getter that forwards to Method_00c0ba40 @ 0x00C0BA40 on the embedded smart pointer at
// +0x4: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_00c0ba40();               // 0x00C0BA40
};

struct CClass_00c309e0 {
    virtual void v00();
    unsigned char _pad[0x0];
    CBaseIntelligentPointer m_pointer;     // +0x4
    virtual CBaseObject* GetTarget_00c309e0();
};

CBaseObject* CClass_00c309e0::GetTarget_00c309e0()
{
    return m_pointer.Method_00c0ba40();
}

// CClass_00643e9a::GetTarget_00643e9a() -- returns m_pointer.Method_0082b7c0() for the smart pointer at +0x28 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643E9A, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x28 ; jmp CBaseIntelligentPointer::Method_0082b7c0
// Virtual getter that forwards to Method_0082b7c0 @ 0x0082B7C0 on the embedded smart pointer at
// +0x28: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_0082b7c0();               // 0x0082B7C0
};

struct CClass_00643e9a {
    virtual void v00();
    unsigned char _pad[0x24];
    CBaseIntelligentPointer m_pointer;     // +0x28
    virtual CBaseObject* GetTarget_00643e9a();
};

CBaseObject* CClass_00643e9a::GetTarget_00643e9a()
{
    return m_pointer.Method_0082b7c0();
}
// CTCTargetingAI::vfunc_29() -- returns m_pointer.GetPItem() for the smart pointer at +0x38 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDAE5, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x38 ; jmp CBaseIntelligentPointer::GetPItem
// Virtual getter that forwards to GetPItem @ 0x00A01B50 on the embedded smart pointer at
// +0x38: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem();               // 0x00A01B50
};

struct CTCTargetingAI {
    virtual void v00();
    unsigned char _pad[0x34];
    CBaseIntelligentPointer m_pointer;     // +0x38
    virtual CBaseObject* vfunc_29();
};

CBaseObject* CTCTargetingAI::vfunc_29()
{
    return m_pointer.GetPItem();
}

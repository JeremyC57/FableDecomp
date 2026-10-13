// CTCTargetingPlayer::vfunc_35() -- returns m_pointer.GetPItem() for the smart pointer at +0xdc (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E231D, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0xdc ; jmp CBaseIntelligentPointer::GetPItem
// Virtual getter that forwards to GetPItem @ 0x00A01B50 on the embedded smart pointer at
// +0xdc: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem();               // 0x00A01B50
};

struct CTCTargetingPlayer {
    virtual void v00();
    unsigned char _pad[0xd8];
    CBaseIntelligentPointer m_pointer;     // +0xdc
    virtual CBaseObject* vfunc_35();
};

CBaseObject* CTCTargetingPlayer::vfunc_35()
{
    return m_pointer.GetPItem();
}
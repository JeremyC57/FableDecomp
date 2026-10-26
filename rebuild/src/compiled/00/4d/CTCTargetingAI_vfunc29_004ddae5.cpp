// CTCTargetingAI::vfunc_29() -- returns m_pointer.GetPItem() for the smart pointer at +0x38 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDAE5, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x38 ; jmp CBaseIntelligentPointer::GetPItem
// Virtual getter that forwards to GetPItem @ 0x00A01B50 on the embedded smart pointer at
// +0x38: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
class CBaseObject;

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CTCTargetingAI { public:
    virtual void v00();
    unsigned char _pad[0x34];
    CBaseIntelligentPointer m_pointer;     // +0x38
    virtual CBaseObject* vfunc_29();
};

CBaseObject* CTCTargetingAI::vfunc_29()
{
    return m_pointer.GetPItem();
}
// CThingGameObject::vfunc_38() -- returns m_pointer.GetPItem() for the smart pointer at +0xc0 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00529940, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0xc0 ; jmp CBaseIntelligentPointer::GetPItem
// Virtual getter that forwards to GetPItem @ 0x00A01B50 on the embedded smart pointer at
// +0xc0: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem();               // 0x00A01B50
};

struct CThingGameObject {
    virtual void v00();
    unsigned char _pad[0xbc];
    CBaseIntelligentPointer m_pointer;     // +0xc0
    virtual CBaseObject* vfunc_38();
};

CBaseObject* CThingGameObject::vfunc_38()
{
    return m_pointer.GetPItem();
}

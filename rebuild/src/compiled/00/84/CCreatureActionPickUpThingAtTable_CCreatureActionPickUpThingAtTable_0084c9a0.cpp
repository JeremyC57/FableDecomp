// CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable&) -- copy constructor: base CCreatureActionBase(other), own vtable (0x012753E4), the CIntelligentPointer at +0xa8 copy-constructed (inlined family-44 copy: base @ 0x00A01B10, vtable 0x01238C6C, SetPItem(other.GetPItem())), the word at +0xb0 copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C9A0, length 80 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: `ret 4`; the trailing word is copied through eax after the pointer member. Class name from the vtable (rtti-vtables.tsv); the chunk label "CCreatureAction_PickUpJug" is propagated.
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CCreatureActionBase { public:
    virtual void v00();
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:   // vtable 0x012753E4; sizeof 0xb4
    unsigned char _pad[0xa4];
    CIntelligentPointer m_target;           // +0xa8
    long m_amount;                          // +0xb0
    virtual void v00();
    CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable& other);
};

CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable& other)
    : CCreatureActionBase(other)
    , m_target(other.m_target)
    , m_amount(other.m_amount)
{
}
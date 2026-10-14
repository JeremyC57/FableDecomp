// CCreatureAction_FireMissileTest::CCreatureAction_FireMissileTest(const CCreatureAction_FireMissileTest&) -- copy constructor: base CCreatureActionBase(other), own vtable (0x01285544), the CIntelligentPointer at +0xa8 copy-constructed (inlined family-44 copy: base @ 0x00A01B10, vtable 0x01237F1C, SetPItem(other.GetPItem())), the word at +0xb0 copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D63F0, length 79 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: `ret 4`; the trailing word is copied through eax after the pointer member. Class name from the vtable (rtti-vtables.tsv); the chunk label "CCreatureAction_PickUpJug" is propagated.
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCreatureActionBase {
    virtual void v00();
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

struct CCreatureAction_FireMissileTest : CCreatureActionBase {   // vtable 0x01285544; sizeof 0xb4
    unsigned char _pad[0xa4];
    CIntelligentPointer m_target;           // +0xa8
    long m_amount;                          // +0xb0
    virtual void v00();
    CCreatureAction_FireMissileTest(const CCreatureAction_FireMissileTest& other);
};

CCreatureAction_FireMissileTest::CCreatureAction_FireMissileTest(const CCreatureAction_FireMissileTest& other)
    : CCreatureActionBase(other)
    , m_target(other.m_target)
    , m_amount(other.m_amount)
{
}
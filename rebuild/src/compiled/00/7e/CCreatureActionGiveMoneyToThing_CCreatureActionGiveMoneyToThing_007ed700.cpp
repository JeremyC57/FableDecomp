// CCreatureAction_GiveMoneyToThing::CCreatureAction_GiveMoneyToThing(const CCreatureAction_GiveMoneyToThing&) -- copy constructor: base CCreatureActionBase(other), own vtable (0x0127139C), the CIntelligentPointer at +0xa8 copy-constructed (inlined family-44 copy: base @ 0x00A01B10, vtable 0x01238C6C, SetPItem(other.GetPItem())), the word at +0xb0 copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED700, length 80 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: `ret 4`; the trailing word is copied through eax after the pointer member. Class name from the vtable (rtti-vtables.tsv); the chunk label "CCreatureAction_PickUpJug" is propagated.
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
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureActionBase { public:
    virtual void v00();
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_GiveMoneyToThing : public CCreatureActionBase { public:   // vtable 0x0127139C; sizeof 0xb4
    unsigned char _pad[0xa4];
    CIntelligentPointer<CThing> m_target;           // +0xa8
    long m_amount;                          // +0xb0
    virtual void v00();
    CCreatureAction_GiveMoneyToThing(const CCreatureAction_GiveMoneyToThing& other);
};

CCreatureAction_GiveMoneyToThing::CCreatureAction_GiveMoneyToThing(const CCreatureAction_GiveMoneyToThing& other)
    : CCreatureActionBase(other)
    , m_target(other.m_target)
    , m_amount(other.m_amount)
{
}
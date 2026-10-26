// CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures() -- base CCombatActionBase(150), own vtable (0x0127ADDC), the flag at +0x114 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B83C0, length 32 bytes.
// Callees: CCombatActionBase::CCombatActionBase(long) @ 0x00857E40.
// Shape: the default-constructor shell of the landed 11-body family (0x008C9D20) plus one byte member cleared after the vtable store.
class CThingCreatureBase;
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);     // 0x00857E40
};

class CCreatureAction_SummonCreatures : public CCombatActionBase { public:   // vtable 0x0127ADDC
    unsigned char m_isFromInventory;        // +0x114 (the bool the (owner, target, bool) constructor @ 0x008B83E0 stores)
    virtual void v00();
    CCreatureAction_SummonCreatures();
};

CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures()
    : CCombatActionBase(150)
    , m_isFromInventory(0)
{
}
// CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons() -- base CCombatActionBase(150), own vtable (0x0127B1B4), the mode at +0x114 set to 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8560, length 48 bytes.
// Callees: CCombatActionBase::CCombatActionBase(long) @ 0x00857E40.
// Shape: the default-constructor shell of the landed 11-body family (0x008C9D20) plus one long member stored after the vtable.
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);     // 0x00857E40
};

class CCreatureAction_UnsheatheWeapons : public CCombatActionBase { public:   // vtable 0x0127B1B4
    long m_unsheatheMode;                   // +0x114 (the long the (owner, long) constructor @ 0x008B85D0 stores)
    virtual void v00();
    CCreatureAction_UnsheatheWeapons();
};

CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons()
    : CCombatActionBase(150)
    , m_unsheatheMode(2)
{
}
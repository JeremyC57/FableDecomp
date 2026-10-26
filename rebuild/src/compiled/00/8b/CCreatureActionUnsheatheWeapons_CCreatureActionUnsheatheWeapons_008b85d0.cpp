// CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase*, long) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), own vtable (0x0127B1B4), the mode at +0x114 from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B85D0, length 63 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Shape: the sound pair is zeroed by its inline constructor (register-zeroed stack stores); the long argument is read from its
// slot after the base call and stored after the vtable (as Level in the landed 0x008CE2F0). `ret 8`: two arguments.
class CThingCreatureBase;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCreatureAction_UnsheatheWeapons : public CCombatActionBase { public:   // vtable 0x0127B1B4
    long m_unsheatheMode;                   // +0x114
    virtual void v00();
    CCreatureAction_UnsheatheWeapons(CThingCreatureBase* owner, long unsheatheMode);
};

CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase* owner, long unsheatheMode)
    : CCombatActionBase(owner, 150, CSoundPair())
    , m_unsheatheMode(unsheatheMode)
{
}
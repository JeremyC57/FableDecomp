// CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase*) -- base CCombatActionBase(owner, 0, zeroed CSoundPair), own vtable (0x0127B1B4), the mode at +0x114 set to 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8590, length 64 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Shape: the owner is loaded into edx before the frame; the action code 0 shares the zero register that also builds the sound
// pair (inline constructor, register-zeroed stack stores); the long member is stored after the vtable. `ret 4`.
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
    CCreatureAction_UnsheatheWeapons(CThingCreatureBase* owner);
};

CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 0, CSoundPair())
    , m_unsheatheMode(2)
{
}
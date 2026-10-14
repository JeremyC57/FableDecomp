// CCreatureAction_SummonerSheatheWeapon::CCreatureAction_SummonerSheatheWeapon(CThingCreatureBase*) -- an inlined __declspec(novtable) sheathe-weapon base (CCombatActionBase(owner, 0, zeroed CSoundPair) and its flag at +0x114 cleared) then this class's vtable (0x0128360C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CBF00, length 63 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Shape: the byte at +0x114 is stored BEFORE the vtable store, so it belongs to an intermediate base whose inline constructor is
// inlined here and which stores no vtable of its own (`__declspec(novtable)`); the action code 0 is pushed as an immediate and the
// sound pair zeroed with immediate stores (no shared zero register, unlike the register-zeroed 150-code shells). `ret 4`.
struct CThingCreatureBase;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct __declspec(novtable) CCreatureAction_SheatheWeaponsBase : CCombatActionBase {   // abstract: no vtable store of its own
    unsigned char m_isDone;                 // +0x114
    virtual void v00();
    CCreatureAction_SheatheWeaponsBase(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 0, CSoundPair())
        , m_isDone(0)
    {
    }
};

struct CCreatureAction_SummonerSheatheWeapon : CCreatureAction_SheatheWeaponsBase {   // vtable 0x0128360C
    virtual void v00();
    CCreatureAction_SummonerSheatheWeapon(CThingCreatureBase* owner);
};

CCreatureAction_SummonerSheatheWeapon::CCreatureAction_SummonerSheatheWeapon(CThingCreatureBase* owner)
    : CCreatureAction_SheatheWeaponsBase(owner)
{
}

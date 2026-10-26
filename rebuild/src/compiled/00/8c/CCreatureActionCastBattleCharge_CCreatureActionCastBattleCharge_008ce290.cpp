// CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(CThingCreatureBase*, CThing*, bool, long) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), Level from the argument, own vtable (0x0128439C), Override cleared, IsZTargetting from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE290, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30
// Shape: init-list members in declaration order (Override, Level, IsZTargetting); the scheduler hoists the Level store
// (loaded from its argument slot) above the vtable store and keeps the two byte stores after it. `ret 10h`: four arguments.
// The retail layout is byte-packed (bool members followed by unaligned dwords/vectors: +0x115, +0x125...), as the
// PDB-derived engine headers also declare (#pragma pack(push, 1)); the pragma here reproduces that real layout.
class CThingCreatureBase;
class CThing;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);                                                                         // 0x00857E40
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
};

#pragma pack(push, 1)
class CCreatureAction_CastBattleCharge : public CCombatActionBase { public:   // vtable 0x0128439C; retail sizeof 0x130
    unsigned char Override;                 // +0x114 (engine header: bool at PDB +0x12c)
    unsigned char Dir[12];                  // +0x115 C3DVector (PDB +0x12d), untouched by these constructors
    unsigned char _pad121[3];               // +0x121
    float         Distance;                 // +0x124 (PDB +0x13c), untouched
    long          Level;                    // +0x128 (PDB +0x140)
    unsigned char IsZTargetting;            // +0x12c (PDB +0x144)
    unsigned char _pad12d[3];               // +0x12d
    virtual void v00();
    CCreatureAction_CastBattleCharge();
    CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, long level);
    CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, CThing* target, bool isZTargetting, long level);
};
#pragma pack(pop)

CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, CThing* target, bool isZTargetting, long level)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , Override(0)
    , Level(level)
    , IsZTargetting(isZTargetting)
{
}
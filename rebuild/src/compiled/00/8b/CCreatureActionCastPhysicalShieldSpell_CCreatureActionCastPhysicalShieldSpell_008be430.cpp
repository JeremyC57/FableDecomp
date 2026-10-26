// CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(const CCreatureAction_CastPhysicalShieldSpell&) -- copy constructor: base CCombatActionBase(other), own vtable (0x0127BDCC), five members copied one register each
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BE430, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0
// Shape: the memberwise copy constructor (ImmediateCast, OneAnimCast, LevelToCast, TimeLimit, ShieldHealth -- the
// engine-header names, PDB +0x12c.., retail +0x114.. because retail CCombatActionBase is 0x114 bytes); ShieldHealth is a
// float in the PDB but retail moves it with an integer register, so it is modelled as a dword (a float copy would go
// through the FPU). `ret 4`: one argument.
class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase { public:   // vtable 0x0127BDCC; sizeof 0x124
    unsigned char ImmediateCast;            // +0x114 (bool)
    unsigned char OneAnimCast;              // +0x115 (bool)
    long          LevelToCast;              // +0x118
    long          TimeLimit;                // +0x11c
    long          ShieldHealth;             // +0x120 (float in the PDB; copied as a dword)
    virtual void v00();
    CCreatureAction_CastPhysicalShieldSpell(const CCreatureAction_CastPhysicalShieldSpell& other);
};

CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(const CCreatureAction_CastPhysicalShieldSpell& other)
    : CCombatActionBase(other)
    , ImmediateCast(other.ImmediateCast)
    , OneAnimCast(other.OneAnimCast)
    , LevelToCast(other.LevelToCast)
    , TimeLimit(other.TimeLimit)
    , ShieldHealth(other.ShieldHealth)
{
}
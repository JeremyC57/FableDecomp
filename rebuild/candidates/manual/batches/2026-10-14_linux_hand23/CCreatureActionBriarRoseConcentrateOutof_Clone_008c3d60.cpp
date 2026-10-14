// CCreatureAction_BriarRoseConcentrateOutof::Clone() -- new CCreatureAction_BriarRoseConcentrateOutof(*this) with two levels of implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C3D60, length 77 bytes.
// Callees: operator new @ 0x00BFEA1A (0x11c bytes, null-checked), CCombatActionBase::CCombatActionBase(const CCombatActionBase&)
// @ 0x0062F3B0
// Shape: the base CCreatureAction_CastWillDrainLifeSpell's implicit copy constructor is inlined (base copy out of line,
// its vtable 0x012645EC, the two dwords at +0x114/+0x118 copied one register each), then this class's vtable
// 0x01280BB4 replaces it (rtti-vtables.tsv names both). sizeof = 0x11c: the class adds no data. Vtable slot 11.
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCreatureAction_CastWillDrainLifeSpell : CCombatActionBase {   // vtable 0x012645EC; sizeof 0x11c
    long m_level;                           // +0x114 (the two longs the (creature, target, long, long) constructor @ 0x008BBBA0 stores)
    long m_defIndex;                        // +0x118
    virtual void v00();                     // implicit copy constructor
};

struct CCreatureAction_BriarRoseConcentrateOutof : CCreatureAction_CastWillDrainLifeSpell {   // vtable 0x01280BB4; adds no data
    virtual void v00();
    CCombatActionBase* Clone();
};

CCombatActionBase* CCreatureAction_BriarRoseConcentrateOutof::Clone()
{
    return new CCreatureAction_BriarRoseConcentrateOutof(*this);
}

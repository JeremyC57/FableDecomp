// CCreatureAction_CastStrengthMultiStrikeProper::Clone() const -- new CCreatureAction_CastStrengthMultiStrikeProper(*this): the implicit copy constructor inlined (base copy out of line, own vtable, members copied one register each)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6F50, length 59 bytes. Vtable slot 11 (Clone).
// Callees: operator new @ 0x00BFEA1A (0x118 bytes, null-checked), CCombatActionBase::CCombatActionBase(const CCombatActionBase&)
// @ 0x0062F3B0. Vtable 0x012877A4 (rtti-vtables.tsv). SpellLevel is the engine-header name.
// Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so the members sit at +0x114.. (PDB +0x12c..); modelled locally.
struct CCreatureActionBase;

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCreatureAction_CastStrengthMultiStrikeProper : CCombatActionBase {   // vtable 0x012877A4; sizeof 0x118
    long SpellLevel;                        // +0x114 (engine header: PDB +0x12c)
    virtual void v00();                     // implicit copy constructor
    CCreatureActionBase* Clone() const;
};

CCreatureActionBase* CCreatureAction_CastStrengthMultiStrikeProper::Clone() const
{
    return (CCreatureActionBase*)new CCreatureAction_CastStrengthMultiStrikeProper(*this);
}

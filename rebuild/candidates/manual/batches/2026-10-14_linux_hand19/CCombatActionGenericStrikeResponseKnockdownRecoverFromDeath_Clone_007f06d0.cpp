// CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath::Clone() -- new CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath(*this) with both implicit copy constructors inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F06D0, length 77 bytes.
// Callees: operator new @ 0x00BFEA1A (0x118 bytes, __cdecl), CCombatActionBase::CCombatActionBase(const
// CCombatActionBase&) @ 0x0062F3B0.
// The implicit copy constructor of the intermediate CCombatAction_GenericStrikeResponseKnockdownRecoverBase is
// inlined (base copy, its vtable 0x01271BAC, CanBeHit +0x114 and Flip +0x115 copied byte-wise), then this
// class's vtable 0x01271A64 (both named in rtti-vtables.tsv). sizeof = 0x118 (retail base 0x114 + 2 bools,
// padded); the engine header describes the PDB build (0x130), so the layout is modelled locally with its names.
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);     // 0x0062F3B0
};

struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase {
    bool CanBeHit;                              // +0x114
    bool Flip;                                  // +0x115
    virtual void v00();                         // vtable 0x01271BAC; copy constructor implicit
};

struct CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath : CCombatAction_GenericStrikeResponseKnockdownRecoverBase {
    virtual void v00();                         // own vtable 0x01271A64; copy constructor implicit
    CCombatActionBase* Clone() const;
};

CCombatActionBase* CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath::Clone() const
{
    return new CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath(*this);
}

// CCreatureAction_CastWillFireballSpell::Clone() const -- new CCreatureAction_CastWillFireballSpell(*this): the implicit copy constructor inline (base copy, vtable, the class's own members)
// Leaf: ?Clone@CCreatureAction_CastWillFireballSpell@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BE9C0, 83 bytes, thiscall, plain `ret`.
// Identity (proven): CCreatureAction_CastWillFireballSpell vtable 0x0127DF4C slot 11 (the Clone slot); Ego_r.pdb ?Clone@CCreatureAction_CastWillFireballSpell@@UBEPAVCCreatureActionBase@@XZ
//   (donor 0x008F63F0); it allocates 0x11C bytes = Ego_r.pdb sizeof less the CCombatActionBase shrink (0x12C -> 0x114) and
//   copies the PDB members at their retail offsets.
// Callees: operator new 0x00BFEA1A, CCombatActionBase::CCombatActionBase(const CCombatActionBase&) 0x0062F3B0.
// Shape (family K037, representative 0x008B8840): `return new X(*this);` with X's implicit copy constructor inlined
//   (the base copy constructor is out of line); the null-tested allocation returns 0 on failure.
class CThing;
template <class T> class CIntelligentPointer;
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual CCreatureActionBase* Clone() const = 0;
};
class CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114 (Ego_r.pdb 0x12C)
public:
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
    unsigned char _body[0x114 - 4];
};
class CCreatureAction_CastWillFireballSpell : public CCombatActionBase {
public:
    virtual CCreatureActionBase* Clone() const;
    bool ImmediateCast;                                  // +0x114 (PDB +300)
    bool OneAnimCast;                                    // +0x115
    long LevelToCast;                                    // +0x118
};
CCreatureActionBase* CCreatureAction_CastWillFireballSpell::Clone() const
{
    return new CCreatureAction_CastWillFireballSpell(*this);
}
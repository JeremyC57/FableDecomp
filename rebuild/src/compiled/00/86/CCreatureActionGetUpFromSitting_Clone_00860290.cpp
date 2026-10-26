// CCreatureAction_GetUpFromSitting::Clone() const -- new CCreatureAction_GetUpFromSitting(*this): the implicit copy constructor inlined (base copy out of line, own vtable, members copied one register each)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860290, length 71 bytes. Vtable slot 11 (Clone).
// Callees: operator new @ 0x00BFEA1A (0x11c bytes, null-checked), CCombatActionBase::CCombatActionBase(const CCombatActionBase&)
// @ 0x0062F3B0. Vtable 0x01276C2C (rtti-vtables.tsv). SittingMode / RemoveOnPerformAction are the Ego_r.pdb names.
// Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so the members sit at +0x114.. (PDB +0x12c..); modelled locally.
class CCreatureActionBase;

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

class CCreatureAction_GetUpFromSitting : public CCombatActionBase { public:   // vtable 0x01276C2C; sizeof 0x11c
    long          SittingMode;            // +0x114 (PDB +0x12c)
    unsigned char RemoveOnPerformAction;   // +0x118 (PDB +0x130, bool)
    virtual void v00();                     // implicit copy constructor
    CCreatureActionBase* Clone() const;
};

CCreatureActionBase* CCreatureAction_GetUpFromSitting::Clone() const
{
    return (CCreatureActionBase*)new CCreatureAction_GetUpFromSitting(*this);
}
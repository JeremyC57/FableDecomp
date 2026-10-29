// CCreatureAction_InterruptableMidAttackAutoTurn::CCreatureAction_InterruptableMidAttackAutoTurn(const CCreatureAction_InterruptableMidAttackAutoTurn&) -- copy constructor: base copy, the required facing, the weapon pointer, the strike-end flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E690, length 115 bytes (+ int3 padding). Vtable 0x012778CC.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: `ret 4`; explicit memberwise copy: RequiredFacing (+0x114) as three dword moves through eax/ecx/edx (a vector with a
// memberwise user copy constructor over dword members, so no FPU and no `lea`), PWeapon (+0x120) through the inline intelligent-pointer
// copy constructor (its address kept in ebx), PerformedStrikeEnd (+0x128). Members = PDB names (PDB +0x12c.. sit 0x18 high).
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(((CThing*)other.GetPItem())); }
};
class C3DVector { public:                          // floats in the engine; copied as dwords
    long X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public:   // vtable 0x012778CC
    C3DVector           RequiredFacing;     // +0x114
    CIntelligentPointer PWeapon;            // +0x120 CIntelligentPointer<CThing const>
    bool                PerformedStrikeEnd; // +0x128
    virtual void v00();
    CCreatureAction_InterruptableMidAttackAutoTurn(const CCreatureAction_InterruptableMidAttackAutoTurn& other);
};

CCreatureAction_InterruptableMidAttackAutoTurn::CCreatureAction_InterruptableMidAttackAutoTurn(const CCreatureAction_InterruptableMidAttackAutoTurn& other)
    : CCombatActionBase(other)
    , RequiredFacing(other.RequiredFacing)
    , PWeapon(other.PWeapon)
    , PerformedStrikeEnd(other.PerformedStrikeEnd)
{
}
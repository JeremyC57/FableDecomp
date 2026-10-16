// CTCInventoryAbilities::OnInitialActivate() -- base activation, morph refresh, then steps the ability and expression row cursors back one row (not below minus one row) and re-cycles both
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6B47, 143 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: NInventory::CTCInventoryBase::OnInitialActivate() @ 0x005BD1EB (qualified base call), UpdateWillMorph()
//          @ 0x005EE887, GetNumAbilitiesPerRow() @ 0x005EDCF0 (four times), CycleAssignedAbilities(long)
//          @ 0x005F64DD, CycleAssignedExpressions(long) @ 0x005F6793.
// Shape: each cursor is `level = Max(level - perRow, -perRow)` through an inline reference `Max(a, b)` returning
// `a < b ? b : a` (both operands are frame temporaries, the right one built first; `lea eax,[...] ; jl ; lea ;
// mov eax,[eax]`); the second clamp's temporaries land in the opposite slots.
// Layout: retail CTCInventoryAbilities CurrentAssignedAbilityLevel +0x144, CurrentAssignedExpressionLevel +0x1B8
// (PDB +0x15C / +0x1F4).
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

struct CTCInventoryBase {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x13C];
    void OnInitialActivate();                        // 0x005BD1EB
};

struct CTCInventoryAbilities : CTCInventoryBase {
    long          CurrentAssignedAbilityLevel;       // +0x144
    unsigned char _pad_0x148[0x70];
    long          CurrentAssignedExpressionLevel;    // +0x1B8

    long GetNumAbilitiesPerRow() const;              // 0x005EDCF0
    void UpdateWillMorph();                          // 0x005EE887
    void CycleAssignedAbilities(long direction);     // 0x005F64DD
    void CycleAssignedExpressions(long direction);   // 0x005F6793
    void OnInitialActivate();
};

void CTCInventoryAbilities::OnInitialActivate()
{
    CTCInventoryBase::OnInitialActivate();
    UpdateWillMorph();
    CurrentAssignedAbilityLevel = Max(CurrentAssignedAbilityLevel - GetNumAbilitiesPerRow(), -GetNumAbilitiesPerRow());
    CurrentAssignedExpressionLevel = Max(CurrentAssignedExpressionLevel - GetNumAbilitiesPerRow(), -GetNumAbilitiesPerRow());
    CycleAssignedAbilities(1);
    CycleAssignedExpressions(1);
}

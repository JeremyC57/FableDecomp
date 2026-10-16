// CTCInventoryAbilities::GetNumAssignedAbilityRows() const -- counts the rows of the ability grid that hold at least one ability
// Leaf: GetNumAssignedAbilityRows
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE183, 132 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callee: GetNumAbilitiesPerRow() @ 0x005EDCF0 (the main player's quick-access row width; manifest "_Dest_val<...>"
//         is propagated), called before each inner pass, for every cell and for the loop test.
// Shape: the row counter is the `push ecx` frame word sharing the outer index's zero (ebx); for each row
// `for (long column = 0; column < perRow; ++column)` looks at cell `perRow * row + column` (`imul eax,ebx`), an
// in-range non-zero cell counts the row and breaks; the outer loop is `for (unsigned row = 0; row < size(); ++row)`
// over the same vector, read as `*(begin() + cell)`.
// Layout: retail CTCInventoryAbilities sits 0x18 below the PDB here: ReceivedAbilityOrder vector {first, last} +0x154
// (PDB +0x170).
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CTCInventoryAbilities {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x14C];
    CRetailVector<long> ReceivedAbilityOrder;        // +0x154 vector<EHeroAbility>

    long GetNumAbilitiesPerRow() const;              // 0x005EDCF0
    long GetNumAssignedAbilityRows() const;
};

long CTCInventoryAbilities::GetNumAssignedAbilityRows() const
{
    long rows = 0;
    for (unsigned long row = 0; row < ReceivedAbilityOrder.size(); ++row) {
        for (long column = 0; column < GetNumAbilitiesPerRow(); ++column) {
            unsigned long cell = GetNumAbilitiesPerRow() * row + column;
            if (cell < ReceivedAbilityOrder.size() && ReceivedAbilityOrder[cell] != 0) {
                ++rows;
                break;
            }
        }
    }
    return rows;
}

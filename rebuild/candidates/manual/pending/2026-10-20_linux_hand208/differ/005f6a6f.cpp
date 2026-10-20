// CTCInventoryAbilities::ResetAssignedAbilitiesPC() -- copies the assigned abilities into the save slots and the received order, steps the HUD row back one page and re-cycles the assignment
// Grade: DIFFER(223v216) -- only the final def release: retail `test ecx,ecx ; je ; dec [ecx+4] ; jne`, ours `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0 ; jne` (the documented inlined null-tested release lane gap; plain, __forceinline and local-copy dtor forms tried)
// Leaf: ResetAssignedAbilitiesPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6A6F, 216 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCInventoryAbilities::GetMiscThingsDef() const @ 0x005EE3B3 (CDefPointer by hidden pointer; manifest label
//          "CCreatureAction_CastStrengthMultiStrikeProper" is propagated), vector<EHeroAbility>::erase(first, last)
//          @ 0x005F8F89 and ::push_back(const EHeroAbility&) @ 0x005F9BCD (manifest labels generic), CTCInventoryAbilities::
//          GetNumAbilitiesPerRow() const @ 0x005EDCF0 (landed name; twice), CycleAssignedAbilities(bool) @ 0x005F64DD
//          (Ego_r.pdb ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z).
// Shape: the misc-things def is a named local held to the end (released with a null test, `dec [ecx+4] ; jne`);
// ReceivedAbilityOrder is cleared through the inline clear() = erase(begin, end); the copy loop runs while the index
// is inside BOTH vectors (rotated: the first size tested on entry and at the bottom), elements reached with the
// STLport `*(begin() + i)` form; the row level is the Dinkumware `Max(level - perRow, -perRow)` over two frame
// temporaries (the right argument evaluated first).
// Layout: retail CTCInventoryAbilities (CTCInventoryBase 0x130 + the screen base vptr, 12-byte vectors; engine/
// CTCInventoryAbilities.h carries PDB offsets): CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154,
// AssignedAbilities +0x17C, AssignedAbilitiesSave +0x188.
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T> inline const T& Max(const T& left, const T& right)
{
    return left < right ? right : left;
}

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CUIMiscThingsDef : CDefBase {
};
struct CMiscThingsDefPointer {                       // CDefPointer<const CUIMiscThingsDef>
    CUIMiscThingsDef* Object;
    CMiscThingsDefPointer() {}
    ~CMiscThingsDefPointer() { if (Object) Object->Release(); }
};

enum EHeroAbility { HERO_ABILITY_NULL };

struct CAbilityVector {                              // retail std::vector<EHeroAbility> {first, last, end}
    EHeroAbility* _M_start;
    EHeroAbility* _M_finish;
    EHeroAbility* _M_end_of_storage;
    EHeroAbility* begin() { return _M_start; }
    EHeroAbility* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    EHeroAbility& operator[](unsigned long n) { return *(begin() + n); }
    EHeroAbility* erase(EHeroAbility* first, EHeroAbility* last);   // 0x005F8F89
    void push_back(const EHeroAbility& value);       // 0x005F9BCD
    void clear() { erase(begin(), end()); }
};

struct CTCInventoryAbilities {
    void*          __vftable;                        // +0x000
    unsigned char  _pad_0x004[0x140];
    long           CurrentAssignedAbilityLevel;      // +0x144 (PDB +0x15C)
    unsigned char  _pad_0x148[0x0C];
    CAbilityVector ReceivedAbilityOrder;             // +0x154 (PDB +0x170)
    unsigned char  _pad_0x160[0x1C];
    CAbilityVector AssignedAbilities;                // +0x17C (PDB +0x1A4)
    CAbilityVector AssignedAbilitiesSave;            // +0x188 (PDB +0x1B4)

    CMiscThingsDefPointer GetMiscThingsDef() const;  // 0x005EE3B3
    long GetNumAbilitiesPerRow() const;              // 0x005EDCF0
    void CycleAssignedAbilities(bool forward);       // 0x005F64DD
    void ResetAssignedAbilitiesPC();
};

void CTCInventoryAbilities::ResetAssignedAbilitiesPC()
{
    CMiscThingsDefPointer miscThings = GetMiscThingsDef();
    ReceivedAbilityOrder.clear();
    for (unsigned long i = 0; i < AssignedAbilities.size() && i < AssignedAbilitiesSave.size(); ++i) {
        AssignedAbilitiesSave[i] = AssignedAbilities[i];
        ReceivedAbilityOrder.push_back(AssignedAbilities[i]);
    }
    CurrentAssignedAbilityLevel = Max(CurrentAssignedAbilityLevel - GetNumAbilitiesPerRow(), -GetNumAbilitiesPerRow());
    CycleAssignedAbilities(true);
}

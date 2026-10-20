// Grade: DIFFER(292v290) -- only the final def release: ours `mov eax,[ebp-4] ; test eax,eax ; je ; dec [eax+4] ; mov ecx,eax`, retail loads the def straight into ecx (`mov ecx,[ebp-4] ; dec [ecx+4]`); known def-release load-register gap
// CTCInventoryAbilities::ResetAssignedAbilities() -- rebuilds the received-ability order from the assigned abilities (saving them), swaps each palette row's slot-1 entry with its neighbour, steps the assigned-level cursor back one row and re-cycles the palette
// Retail 0x005F694D, 290 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: GetUIMiscThingsDef() 0x005EE3B3 (CDefPointer by value), vector<EHeroAbility>::erase(first, last) 0x005F8F89
// (manifest "CRGBColour_CopyAssign"), vector<EHeroAbility>::push_back(const value_type&) 0x005F9BCD (manifest
// "Vector_PushElement_OrReallocate"), GetNumAbilitiesPerRow() const 0x005EDCF0 (x2), CycleAssignedAbilities(bool)
// 0x005F64DD, the def's slot 1 on release.
// Shape: ReceivedAbilityOrder is cleared through an inline `clear() { erase(begin(), end()); }`; the copy loop runs while
// both AssignedAbilities and AssignedAbilitiesSave have an element (`i < a.size() && i < b.size()`, rotated); the
// swap is an inline std::swap of two vector elements; the level cursor is
// `std::max(level - perRow, -perRow)` (right argument first: the negated row width is the [ebp-8] temporary).
// Layout (retail; Ego_r.pdb names): CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154,
// AssignedAbilities +0x17C, AssignedAbilitiesSave +0x188; CUIMiscThingsDef::TotalSpellsInContainer +0x14C.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {
template<class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
}

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void Release() { if (DecRefToZero(RefCount)) Destroy(); }
};
struct CUIMiscThingsDef : CDefPointeeBase {
    unsigned char _pad_0x08[0x14c - 0x08];
    unsigned long TotalSpellsInContainer;              // +0x14c
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };
struct CAbilityVector {                                // vector<EHeroAbility> (12 bytes)
    EHeroAbility* _M_start;
    EHeroAbility* _M_finish;
    EHeroAbility* _M_end_of_storage;
    EHeroAbility* begin() { return _M_start; }
    EHeroAbility* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    EHeroAbility& operator[](unsigned long n) { return *(begin() + n); }
    EHeroAbility* erase(EHeroAbility* first, EHeroAbility* last);   // 0x005F8F89
    void push_back(const EHeroAbility& value);                      // 0x005F9BCD
    void clear() { erase(begin(), end()); }
};

struct CTCInventoryAbilities {
    unsigned char _pad_0x000[0x144];
    long CurrentAssignedAbilityLevel;                  // +0x144
    unsigned char _pad_0x148[0x154 - 0x148];
    CAbilityVector ReceivedAbilityOrder;               // +0x154
    unsigned char _pad_0x160[0x17c - 0x160];
    CAbilityVector AssignedAbilities;                  // +0x17c
    CAbilityVector AssignedAbilitiesSave;              // +0x188
    CDefPointer<CUIMiscThingsDef> GetUIMiscThingsDef();   // 0x005EE3B3
    long GetNumAbilitiesPerRow() const;                // 0x005EDCF0
    void CycleAssignedAbilities(bool forward);         // 0x005F64DD
    void ResetAssignedAbilities();
};

void CTCInventoryAbilities::ResetAssignedAbilities()
{
    CDefPointer<CUIMiscThingsDef> misc = GetUIMiscThingsDef();
    ReceivedAbilityOrder.clear();
    for (unsigned long i = 0; i < AssignedAbilities.size() && i < AssignedAbilitiesSave.size(); ++i) {
        AssignedAbilitiesSave[i] = AssignedAbilities[i];
        ReceivedAbilityOrder.push_back(AssignedAbilities[i]);
    }
    for (unsigned long j = 0; j < ReceivedAbilityOrder.size(); ++j) {
        if (j % misc->TotalSpellsInContainer == 1 && j < ReceivedAbilityOrder.size() - 1) {
            EHeroAbility swapped = ReceivedAbilityOrder[j];
            ReceivedAbilityOrder[j] = ReceivedAbilityOrder[j + 1];
            ReceivedAbilityOrder[j + 1] = swapped;
        }
    }
    CurrentAssignedAbilityLevel = std::max(CurrentAssignedAbilityLevel - GetNumAbilitiesPerRow(), -GetNumAbilitiesPerRow());
    CycleAssignedAbilities(true);
}

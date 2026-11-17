// CTCInventoryAbilities::ResetAssignedAbilitiesPC() -- copies the assigned abilities into the save slots and the received order, steps the HUD row back one page and re-cycles the assignment
// Leaf: ResetAssignedAbilitiesPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6A6F, 216 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCInventoryAbilities::GetMiscThingsDef() const @ 0x005EE3B3 (CDefPointer by hidden pointer; manifest label
//          "CCreatureAction_CastStrengthMultiStrikeProper" is propagated), vector<EHeroAbility>::erase(first, last)
//          @ 0x005F8F89 and ::push_back(const EHeroAbility&) @ 0x005F9BCD (manifest labels generic), CTCInventoryAbilities::
//          GetNumAssignableSpecialMoveButtons() const @ 0x005EDCF0 (twice), CycleAssignedAbilities(bool) @ 0x005F64DD
//          (Ego_r.pdb ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z).
// Shape: the misc-things def is a named local held to the end (released with a null test, `dec [ecx+4] ; jne`), returned
// by GetMiscThingsDef, a same-TU noinline stand-in body (retail's LTCG saw it keep no reference to the hidden result
// slot; with the callee extern the release is the long `add eax,4 ; dec [eax] ; cmp [eax],0` form);
// ReceivedAbilityOrder is cleared through the inline clear() = erase(begin, end); the copy loop runs while the index
// is inside BOTH vectors (rotated: the first size tested on entry and at the bottom), elements reached with the
// STLport `*(begin() + i)` form; the row level is the Dinkumware `Max(level - perRow, -perRow)` over two frame
// temporaries (the right argument evaluated first).
// Layout: retail CTCInventoryAbilities (CTCInventoryBase 0x130 + the screen base vptr, 12-byte vectors; engine/
// CTCInventoryAbilities.h carries PDB offsets): CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154,
// AssignedAbilities +0x17C, AssignedAbilitiesSave +0x188.
// NF48 (round 7): the ability lists are STLport 4.6.2 std::vector<EHeroAbility> (Ego_r.pdb CTCInventoryAbilities::ReceivedAbilityOrder /
// AssignedAbilities / AssignedAbilitiesSave) from the shared stlport/fable_containers.h, as is std::max (a local CAbilityVector stand-in
// and a local namespace-std max before): clear() is the header's inline erase(begin(), end()) over the out-of-line
// vector<EHeroAbility>::erase(iterator, iterator) 0x005F8F89 (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE); push_back is the
// out-of-line 0x005F9BCD (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE).
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "stlport/fable_containers.h"
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
namespace NUISystem { class CUIMiscThingsDef : public CDefBase { public:
}; }
using NUISystem::CUIMiscThingsDef;
template <class T> class CDefPointer;
template <> class CDefPointer<const NUISystem::CUIMiscThingsDef> { public:   // Ego_r.pdb (was the row-local CMiscThingsDefPointer)
    CUIMiscThingsDef* Object;
    CDefPointer() {}
    CDefPointer(CUIMiscThingsDef* object) : Object(object) {}   // adopts a reference the caller already owns
    ~CDefPointer() { if (Object) Object->Release(); }
};

enum EHeroAbility { HERO_ABILITY_NULL };


class CTCInventoryAbilities { public:
    void*          __vftable;                        // +0x000
    unsigned char  _pad_0x004[0x140];
    long           CurrentAssignedAbilityLevel;      // +0x144 (PDB +0x15C)
    unsigned char  _pad_0x148[0x0C];
    std::vector<EHeroAbility> ReceivedAbilityOrder;             // +0x154 (PDB +0x170)
    unsigned char  _pad_0x160[0x1C];
    std::vector<EHeroAbility> AssignedAbilities;                // +0x17C (PDB +0x1A4)
    std::vector<EHeroAbility> AssignedAbilitiesSave;            // +0x188 (PDB +0x1B4)

private:
    const CDefPointer<const NUISystem::CUIMiscThingsDef> GetMiscThingsDef() const;   // 0x005EE3B3 (Ego_r.pdb: private)
public:
    long GetNumAssignableSpecialMoveButtons() const;              // 0x005EDCF0
    void CycleAssignedAbilities(bool forward);       // 0x005F64DD
    void ResetAssignedAbilitiesPC();
};
CUIMiscThingsDef* Lookup_CTCInventoryAbilities_GetMiscThingsDef(const void* owner);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    return CDefPointer<const NUISystem::CUIMiscThingsDef>(Lookup_CTCInventoryAbilities_GetMiscThingsDef(this));
}
#pragma auto_inline(on)

void CTCInventoryAbilities::ResetAssignedAbilitiesPC()
{
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThings = GetMiscThingsDef();
    ReceivedAbilityOrder.clear();
    for (unsigned long i = 0; i < AssignedAbilities.size() && i < AssignedAbilitiesSave.size(); ++i) {
        AssignedAbilitiesSave[i] = AssignedAbilities[i];
        ReceivedAbilityOrder.push_back(AssignedAbilities[i]);
    }
    CurrentAssignedAbilityLevel = Max(CurrentAssignedAbilityLevel - GetNumAssignableSpecialMoveButtons(), -GetNumAssignableSpecialMoveButtons());
    CycleAssignedAbilities(true);
}
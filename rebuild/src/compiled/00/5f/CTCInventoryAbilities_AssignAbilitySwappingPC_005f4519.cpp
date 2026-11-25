// CTCInventoryAbilities::AssignAbilitySwappingPC(EHeroAbility, unsigned long, unsigned long, CComponent*, CComponent*) -- assigns an ability to a PC quick slot, swapping with the slot it was dragged from and clearing a duplicate in the paired slot
// Leaf: AssignAbilitySwappingPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F4519, 156 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 14h`.
// Callees: UnAssignAbilityPC(unsigned long slot, CComponent*) @ 0x005F1320, AssignAbilityPC(EHeroAbility,
//          unsigned long slot, CComponent*) @ 0x005F0DC5.
// Shape: slots pair up (0/1, 2/3, ...): an even slot with a partner looks at slot + 1, otherwise a non-zero slot
// looks at slot - 1 (unsigned `jae` / `jbe`). Dropping into an empty slot from its own partner just unassigns the
// partner; otherwise a partner already holding this ability is cleared. The ability previously in the slot moves
// to the source slot, then the new ability is assigned (VC threads the unassign-the-partner arm past the
// `if (current)` test because current is known to be zero there). The slot's element address is formed once
// (`lea eax,[eax+ebx*4]`) and the partner read as [eax+4] / [eax-4].
// Layout: retail CTCInventoryAbilities AssignedAbilities vector<EHeroAbility> {first, last} +0x17C (PDB +0x1A4).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CComponent;

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005F0DC5 ?AssignAbilityPC@CTCInventoryAbilities@@QAEXJKPAVCComponent@NUISystem@@@Z (was ?AssignAbilityPC@CTCInventoryAbilities@@QAEXJKPAUCComponent@@@Z)
//   0x005F1320 ?UnAssignAbilityPC@CTCInventoryAbilities@@QAEXKPAVCComponent@NUISystem@@@Z (was ?UnAssignAbilityPC@CTCInventoryAbilities@@QAEXKPAUCComponent@@@Z)
namespace NUISystem { class CComponent; }
class CTCInventoryAbilities { public:
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x174];
    CRetailVector<long> AssignedAbilities;           // +0x17C vector<EHeroAbility>

    void UnAssignAbilityPC(unsigned long, NUISystem::CComponent*);                  // 0x005F1320
    void AssignAbilityPC(long, unsigned long, NUISystem::CComponent*);      // 0x005F0DC5
    void AssignAbilitySwappingPC(long ability, unsigned long slot, unsigned long fromSlot,
                                 CComponent* slotComponent, CComponent* fromComponent);
};

void CTCInventoryAbilities::AssignAbilitySwappingPC(long ability, unsigned long slot, unsigned long fromSlot,
                                                    CComponent* slotComponent, CComponent* fromComponent)
{
    long current = AssignedAbilities[slot];
    if (!(slot & 1) && slot < AssignedAbilities.size() - 1) {
        if (current == 0 && fromSlot == slot + 1)
            UnAssignAbilityPC(fromSlot, reinterpret_cast<NUISystem::CComponent*>(fromComponent));
        else if (AssignedAbilities[slot + 1] == ability)
            UnAssignAbilityPC(slot, reinterpret_cast<NUISystem::CComponent*>(slotComponent));
    } else if (slot > 0) {
        if (current == 0 && fromSlot == slot - 1)
            UnAssignAbilityPC(fromSlot, reinterpret_cast<NUISystem::CComponent*>(fromComponent));
        else if (AssignedAbilities[slot - 1] == ability)
            UnAssignAbilityPC(slot, reinterpret_cast<NUISystem::CComponent*>(slotComponent));
    }
    if (current)
        AssignAbilityPC(current, fromSlot, reinterpret_cast<NUISystem::CComponent*>(fromComponent));
    AssignAbilityPC(ability, slot, reinterpret_cast<NUISystem::CComponent*>(slotComponent));
}
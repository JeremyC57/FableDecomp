// CTCInventoryAbilities::ResetAssignedAbilities() -- rebuilds the received-ability order from the assigned abilities (saving them), swaps each palette row's slot-1 entry with its neighbour, steps the assigned-level cursor back one row and re-cycles the palette
// Retail 0x005F694D, 290 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: GetMiscThingsDef() 0x005EE3B3 (CDefPointer by value), vector<EHeroAbility>::erase(first, last) 0x005F8F89
// (manifest "CRGBColour_CopyAssign"), vector<EHeroAbility>::push_back(const value_type&) 0x005F9BCD (manifest
// "Vector_PushElement_OrReallocate"), GetNumAssignableSpecialMoveButtons() const 0x005EDCF0 (x2), CycleAssignedAbilities(bool)
// 0x005F64DD, the def's slot 1 on release.
// Shape: ReceivedAbilityOrder is cleared through an inline `clear() { erase(begin(), end()); }`; the copy loop runs while
// both AssignedAbilities and AssignedAbilitiesSave have an element (`i < a.size() && i < b.size()`, rotated); the
// swap is an inline std::swap of two vector elements; the level cursor is
// `std::max(level - perRow, -perRow)` (right argument first: the negated row width is the [ebp-8] temporary).
// Fix (retry 10): GetMiscThingsDef (0x005EE3B3, same TU, CDefPointer by hidden pointer) has a same-TU body below; with
// only the declaration the final release loaded the def through eax (`mov ecx,eax`), with it straight into ecx as retail.
// Layout (retail; Ego_r.pdb names): CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154,
// AssignedAbilities +0x17C, AssignedAbilitiesSave +0x188; CUIMiscThingsDef::TotalSpellsInContainer +0x14C.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {
template<class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
}

static FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void Release() { if (DecRefToZero(RefCount)) Destroy(); }
};
namespace NUISystem { class CUIMiscThingsDef : public CDefPointeeBase { public:
    unsigned char _pad_0x08[0x14c - 0x08];
    unsigned long TotalSpellsInContainer;              // +0x14c
}; }
using NUISystem::CUIMiscThingsDef;
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer(const CDefPointer& other);
    explicit CDefPointer(typename CDefNonConst<T>::Type* object) : Object(object) { if (object) ++object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
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

class CTCInventoryAbilities { public:
    unsigned char _pad_0x000[0x144];
    long CurrentAssignedAbilityLevel;                  // +0x144
    unsigned char _pad_0x148[0x154 - 0x148];
    CAbilityVector ReceivedAbilityOrder;               // +0x154
    unsigned char _pad_0x160[0x17c - 0x160];
    CAbilityVector AssignedAbilities;                  // +0x17c
    CAbilityVector AssignedAbilitiesSave;              // +0x188
private:
    const CDefPointer<const NUISystem::CUIMiscThingsDef> GetMiscThingsDef() const;   // 0x005EE3B3 (Ego_r.pdb: private)
public:
    long GetNumAssignableSpecialMoveButtons() const;                // 0x005EDCF0
    void CycleAssignedAbilities(bool forward);         // 0x005F64DD
    void ResetAssignedAbilities();
};

CUIMiscThingsDef* FindMiscThingsDef_s7();   // stand-in: the "UI_MISC_THINGS_DEF" index lookup and GetDef (0x005EE3B3's work)
#pragma auto_inline(off)
// Same-TU stand-in for GetMiscThingsDef (0x005EE3B3, matched separately): fetch the def and return an add-ref'd counted
// pointer through the hidden result.
__declspec(noinline) const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    return CDefPointer<const NUISystem::CUIMiscThingsDef>(FindMiscThingsDef_s7());
}
#pragma auto_inline(on)

void CTCInventoryAbilities::ResetAssignedAbilities()
{
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc = GetMiscThingsDef();
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
    CurrentAssignedAbilityLevel = std::max(CurrentAssignedAbilityLevel - GetNumAssignableSpecialMoveButtons(), -GetNumAssignableSpecialMoveButtons());
    CycleAssignedAbilities(true);
}
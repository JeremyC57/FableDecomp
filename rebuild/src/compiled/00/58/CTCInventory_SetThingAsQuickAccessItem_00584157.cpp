// CTCInventory::SetThingAsQuickAccessItem(long, long, bool) -- puts an owned item into a quick-access slot, optionally giving the quick-access tutorial
// Leaf: SetThingAsQuickAccessItem
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584157, 162 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 0Ch`.
// Callees: CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08 (on `this`), the item-def lookup
//          LookUpInventoryItemDef(long) @ 0x005D9710 (__fastcall), std::bitset::test @ 0x00410DE0, CVectorMap<
//          ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCHero::GiveTutorialForCategory(ETutorialCategory,
//          bool) @ 0x006C9FA0 (category 0x17).
// Shape: the slot is tested signed (`jl`) and then unsigned against size() (`jae`); every failed test shares the
// single exit. The hero component (interface 0x29) is HasTC + an inline GetTC (key homed in the dead flag slot)
// with its own null test before the tutorial call; the store `QuickAccessItems[slot] = item` re-reads begin.
// Layout: retail CTCInventory quick-access vector<long> {first, last, end} at +0x148 (PDB +0x168; engine/
// CTCInventory.h carries the 16-byte PDB containers, so `this` is a local model).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
};

enum ETutorialCategory { TUTORIAL_CATEGORY_NULL, TUTORIAL_CATEGORY_QUICK_ACCESS = 0x17 };

class CTCHero { public:                                     // interface type 0x29
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};

long __fastcall LookUpInventoryItemDef(long defIndex);   // 0x005D9710

struct CLongVector {                                 // std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long* begin() { return _M_start; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};

class CTCInventory { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x140];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)

    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08 (CTCInventoryBase)
    void SetThingAsQuickAccessItem(long item, long slot, bool giveTutorial);
};

void CTCInventory::SetThingAsQuickAccessItem(long item, long slot, bool giveTutorial)
{
    if (slot >= 0 && (unsigned long)slot < QuickAccessItems.size() && item > 0 &&
        GetNumberOfItemsOfTypeInInventory(item) > 0 && LookUpInventoryItemDef(item) != 0) {
        if (giveTutorial) {
            CThing* thing = ParentThing;
            if (thing->HasTC(0x29)) {
                CTCHero* hero = thing->GetTC<CTCHero>(0x29);
                if (hero)
                    hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_QUICK_ACCESS, false);
            }
        }
        QuickAccessItems[slot] = item;
    }
}
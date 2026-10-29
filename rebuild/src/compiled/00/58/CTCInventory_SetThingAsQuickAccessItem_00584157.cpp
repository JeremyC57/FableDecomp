// CTCInventory::SetThingAsQuickAccessItem(long, long, bool) -- puts an owned item into a quick-access slot, optionally giving the quick-access tutorial
// Leaf: SetThingAsQuickAccessItem
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
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
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
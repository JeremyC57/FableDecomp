// CTCInventory::IsQuickAccessItemUsable(long) const -- a quick-access slot holding an item is usable unless it is an augmentation and the owner's hero component (interface 0x29, CTCHero) does not allow those
// Leaf: IsQuickAccessItemUsable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005849EA, 137 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (this
//          TU's instantiation), IsAugmentation(long) @ 0x005D97F0 (__fastcall).
// Shape: the slot's scaled index (`shl eax,2`) is spilled to [ebp-4] and reused for the second element read
// (`*(begin() + slot)`, begin re-read); an empty slot (`<= 0`) falls to the trailing 8-bit `return false`; the component comes
// from a `__forceinline` helper that declares a zeroed out pointer and returns it, so the pointer and the lookup key
// share the dead `slot` argument slot (`and [ebp+8],0`, absent path re-reads it); the tail
// `return component->AllowAugmentations || !IsAugmentation(item);` gives the widened 0/1 exits.
// Layout: retail CTCInventory quick-access item vector<long> {first, ...} at +0x148; CTCBase ParentThing +0x04;
// retail CThing interface bitset +0x20, map +0x44.
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
    const SEntry* LowerBound(const ETCInterfaceType& type) const;            // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component = 0;
        GetInterface(&component, wanted);
        return component;
    }
};

bool __fastcall IsAugmentation(long item);           // 0x005D97F0

class CTCHero { public:                         // interface type 0x29
    unsigned char _pad_0x00[0xC];
    bool          AllowAugmentations;                // +0x0C
};

struct CLongVector {                                 // std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    const long& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInventory { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x140];
    CLongVector   QuickAccessItems;                  // +0x148

    bool IsQuickAccessItemUsable(long slot) const;
};

bool CTCInventory::IsQuickAccessItemUsable(long slot) const
{
    if (QuickAccessItems[slot] > 0) {
        CTCHero* hero = ParentThing->GetTC<CTCHero>(0x29);
        return hero->AllowAugmentations || !IsAugmentation(QuickAccessItems[slot]);
    }
    return false;
}
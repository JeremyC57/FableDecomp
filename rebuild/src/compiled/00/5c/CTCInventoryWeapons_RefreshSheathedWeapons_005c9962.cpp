// CTCInventoryWeapons::RefreshSheathedWeapons() -- looks up the owner's carrier component (interface 0x29) and either puts the weapons in their sheathes or takes them out, following its sheathed flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C9962, 92 bytes.
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0,
//          CTCInventoryWeapons::PutWeaponsInSheathes @ 0x005C8101, the opposite weapon-sheathing update @ 0x005C552C
//          (manifest label "CTCCarryable_PrepareWeaponSheathing"; name here descriptive).
//
// Shape: the inline out-parameter GetInterface (landed 0x007BEA08 family) whose result is IGNORED: the component local
// starts at 0 (`xor ebx,ebx` scheduled before the bitset call), is only assigned on a hit, and is dereferenced
// unconditionally (`cmp byte ptr [ebx+0Ch],0`). Frame pointer kept in a size TU (`#pragma optimize("y", off)` +
// `optimize("s", on)`); `this` lives in the frame slot [ebp-4] across the lookup and both tails are `call ; leave ;
// ret` (no sibling jump with a frame). The helper must be `__forceinline` under "s". +4 is CTCBase::ParentThing; the
// component's flag at +0x0C is named from use. Local model.
#include "engine/CTCBase.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;

struct CPresenceBits {
    bool test(long bit);                      // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                        // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                  // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;               // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_present.test(type)) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CTCCarrier {                           // interface 0x29
    unsigned char _pad_0x00[0xc];
    bool m_weaponsSheathed;                   // +0x0C
};

class CTCInventoryWeapons : public CTCBase { public:
    void PutWeaponsInSheathes();              // 0x005C8101
    void TakeWeaponsOutOfSheathes();          // 0x005C552C
    void RefreshSheathedWeapons();
};

void CTCInventoryWeapons::RefreshSheathedWeapons()
{
    CTCCarrier* carrier = 0;
    ParentThing->GetInterface(0x29, &carrier);
    if (carrier->m_weaponsSheathed)
        PutWeaponsInSheathes();
    else
        TakeWeaponsOutOfSheathes();
}
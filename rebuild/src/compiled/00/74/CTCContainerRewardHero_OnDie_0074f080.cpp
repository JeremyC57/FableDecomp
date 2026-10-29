// CTCContainerRewardHero::OnDie() -- when the container thing dies, drop its rewards as an item-pickup container: unless its interface-0xA0 component already flags them handled (+0x10), and only while Enabled (+0x18) and the owner is neither a creature (type 2) nor type 13
// Leaf: OnDie
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074F080, length 95 bytes; vtable CTCSearchableContainer 0x012431F4 slot 13
// (slot 13 is OnDie across the TC vtables).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the landed GetInterface family),
// CTCContainerRewardHero::CreateItemPickupContainer @ 0x0074EC50.
// Label: the manifest "CTCSearchableContainer::vfunc_13" is a slot placeholder; the body sits in the CTCContainerRewardHero
// code (0x0074E240..0x0074FCB0) and is inherited by CTCSearchableContainer. PDB CTCContainerRewardHero (0x20 bytes) has
// Enabled at +0x1c; retail reads +0x18 (the retail class is 4 bytes narrower), so the class is modelled on CTCBase.h.
// Shape: the thing's inline GetInterface(type, &out) of the landed 0x0075D8F0 (type 0xA0 -> bitset word 5 at +0x34, bit 0:
// `test byte ptr [edi+34h],1`; key in the `push ecx` slot; map pointer local in esi; a miss yields the end entry); the
// owner is a named local (edi); `if (GetInterface && flag) return;` then one `&&` chain on Enabled and the owner type byte.
#include "engine/CTCBase.h"

struct CTCComponent;

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char m_type;                           // +0x10 (2 = creature)
    unsigned char _pad11[0xf];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)((CTCComponent*)it->second);
            return true;
        }
        return false;
    }
};

struct CTCRewardsHandled {                          // the interface-0xA0 component (class not established)
    unsigned char _pad[0x10];
    bool          m_handled;                        // +0x10
};

class CTCContainerRewardHero : public CTCBase { public:
    unsigned char _pad0c[0xc];
    bool          Enabled;                          // +0x18 (PDB +0x1c)
    void CreateItemPickupContainer();               // 0x0074EC50
    void OnDie();
};

void CTCContainerRewardHero::OnDie()
{
    CThing* owner = ParentThing;
    CTCRewardsHandled* handled;
    if (owner->GetInterface(0xa0, &handled) && handled->m_handled)
        return;
    if (Enabled && owner->m_type != 2 && owner->m_type != 0xd)
        CreateItemPickupContainer();
}
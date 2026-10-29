// CTCInventoryTrade::IsItemAffordable(long itemType, long count) -- whether the hero's gold covers the shop's current buying price for `count` items of `itemType`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059DA10, length 171 bytes (`ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::GetTradingThing() @ 0x005BC4C4 (the shop's thing; the catalog label "CWorld::DrawGetEnvironment" is propagated),
// std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound @ 0x0040F020 (the inline GetInterface, types 0x53 -- CTCShop -- and 4),
// CTCShop::GetCurrentBuyPrice(long, long) @ 0x007743B0, CPlayerManager::GetMainPlayer() const @ 0x00449960 (g_pMainGameComponent
// 0x013B86A0 +0x1C), CPlayer::GetPControlledCreature @ 0x00487DC0. The gold sits at +0x3C of the hero's type-4 component.
// Shape: the shop component is not initialised (a miss leaves ecx = the count slot), the gold owner is (`xor ebx,ebx`); the
// price overwrites the dead itemType slot, the second lookup key the dead count slot; the result is `setge` on `gold >= price`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
class CTCBase;
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
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->first) ? map->last : it;
            *out = (T*)((void*)found->second);
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CTCShop { public:                                     // interface type 0x53
    long GetCurrentBuyPrice(long itemType, long count);   // 0x007743B0
};
struct CTCGoldOwner {                                // interface type 4: the hero's purse
    unsigned char _pad00[0x3c];
    long Gold;                                       // +0x3c
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                          // 0x00487DC0
};
class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;                  // 0x00449960 (const)
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

namespace NInventory {
struct CTCInventoryTrade {
    CThing* GetTradingThing();                       // 0x005BC4C4 (CTCInventoryBase)
    bool IsItemAffordable(long itemType, long count);
};
bool CTCInventoryTrade::IsItemAffordable(long itemType, long count)
{
    CTCShop* shop;
    GetTradingThing()->GetInterface(&shop, 0x53);
    long price = shop->GetCurrentBuyPrice(itemType, count);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCGoldOwner* purse = 0;
    hero->GetInterface(&purse, 4);
    bool affordable = purse->Gold >= price;      // a NAMED bool: `return a >= b` zero-extends (xor eax,eax ; setge al)
    return affordable;
}
}
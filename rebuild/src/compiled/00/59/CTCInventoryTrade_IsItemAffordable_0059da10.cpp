// CTCInventoryTrade::IsItemAffordable(long itemType, long count) -- whether the hero's gold covers the shop's current buying price for `count` items of `itemType`
// Leaf: ?IsItemAffordable@CTCInventoryTrade@NInventory@@QAE_NJJ@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059DA10, length 171 bytes (`ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::GetTradingThing() @ 0x005BC4C4 (the shop's thing; the catalog label "CWorld::DrawGetEnvironment" is propagated),
// std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound @ 0x0040F020 (the inline GetInterface, types 0x53 -- CTCShop -- and 4),
// CTCShop::GetCurrentBuyPrice(long, long) @ 0x007743B0, CPlayerManager::GetMainPlayer() const @ 0x00449960 (CMainGameComponent::Get()
// 0x013B86A0 +0x1C), CPlayer::GetPControlledCreature @ 0x00487DC0. The gold sits at +0x3C of the hero's type-4 component.
// Shape: the shop component is not initialised (a miss leaves ecx = the count slot), the gold owner is (`xor ebx,ebx`); the
// price overwrites the dead itemType slot, the second lookup key the dead count slot; the result is `setge` on `gold >= price`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
    const CPlayerManager& PeekPlayerManager() const { return *PlayerManager; }   // FableWin.pdb ?PeekPlayerManager@CMainGameComponent@@QBEABVCPlayerManager@@XZ
};

namespace NInventory {
struct CTCInventoryTrade {
    CThing* GetTradingThing();                       // 0x005BC4C4 (CTCInventoryBase)
    bool IsItemAffordable(long itemType, long count);
};
bool CTCInventoryTrade::IsItemAffordable(long itemType, long count)
{
    CTCShop* shop;
    GetTradingThing()->GetTC(TCI_SHOP, &shop);
    long price = shop->GetCurrentBuyPrice(itemType, count);
    CThing* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCGoldOwner* purse = 0;
    hero->GetTC(TCI_HERO_STATS, &purse);
    bool affordable = purse->Gold >= price;      // a NAMED bool: `return a >= b` zero-extends (xor eax,eax ; setge al)
    return affordable;
}
}
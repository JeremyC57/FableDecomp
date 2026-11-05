// CTCBuyableHouse::DestroyMoneyBags(bool giveToHero) -- kill every live money bag (interface 0xCC) in the house, first crediting its gold to the live hero's stats when asked
// Leaf: DestroyMoneyBags
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail 0x006C0580, 336 bytes, `ret 4`.
// Callees: CVectorMap::LowerBound 0x0040F020 (interfaces 0x21 thing list, 0xCC gold, 4 hero stats), CBaseIntelligentPointer::
// GetPItem 0x00A01B50, the TC's player-manager accessor 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970,
// CPlayer::GetPControlledCreature 0x00487DC0, CTCGold::GetGold 0x007CBDC0, CTCHeroStats::AddMoney(long, bool) 0x0057B327,
// CThing::Kill(bool) 0x004C9B80.
// Layout: CTCBase ParentThing +4; interface-0x21 component list header +0x0C (landed CountMoneyBags 0x006C04D0 model);
// CThing bitset +0x20, map +0x44, dead flag +0x91.
// Shape: the CountMoneyBags walk (three GetPItem calls for the alive and interface tests) with HasTC + GetTC lookups;
// `this` is homed in the frame across the loop; AddMoney's `true` is pushed before GetGold runs.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
class CTCBase;
class CBaseObject;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};

class CThingPlayerCreature : public CThing { };
class CTCGold { public:
    long GetGold() const;                                     // 0x007CBDC0
};
class CTCHeroStats { public:
    void AddMoney(long amount, bool b);                 // 0x0057B327
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                             // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !p.Get()->IsDead();
}
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCBuyableHouse : public CTCBase {
public:
    CThing*       ParentThing;                          // +0x04
    void DestroyMoneyBags(bool giveToHero);
};

void CTCBuyableHouse::DestroyMoneyBags(bool giveToHero)
{
    CTCThingList_0x21* list;
    if (ParentThing->GetTC(TCI_THING_OWNER, &list)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next) {
            CTCGold* gold;
            if (IsThingAlive(node->thing) && node->thing.Get()->GetTC(TCI_GOLD, &gold)) {
                CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                if (hero && !hero->IsDead() && giveToHero && hero->HasTC(TCI_HERO_STATS))
                    ((CTCHeroStats*)&hero->GetTC(TCI_HERO_STATS))->AddMoney(gold->GetGold(), true);
                node->thing.Get()->Kill(true);
            }
        }
    }
}
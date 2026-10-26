// Behaviour test for CTCShopKeeper::CalcAreOtherPeopleInShop (0x00799140): keeper/building gates, villager inside/blocking, hero checks.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class C3DVector { public: float X, Y, Z; };
class CBaseObject;
class CThing;
struct CBaseIntelligentPointer {
    void* __vftable;
    CBaseObject* m_pItem;
    CThing* GetPItem() const { return (CThing*)m_pItem; }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    SEntry* LowerBound(const long& key) { SEntry* it = first; while (it != last && it->type < key) ++it; return it; }
};
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char State;                            // +0x10
    unsigned char _pad11[0x0f];
    unsigned long TCBits[4];                        // +0x20
    unsigned char _pad30[0x14];
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                         // +0x91 bit 0
    unsigned char _pad92[0x170 - 0x92];
    CBaseIntelligentPointer HomeBuilding;           // +0x170
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (TCBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
};
class CThingBuilding : public CThing { public:
    float MinX, MaxX; CThing* Blocker;
    bool IsPointInside(const C3DVector& pos) { return pos.X >= MinX && pos.X <= MaxX; }
    bool IsDoorBlockedBy(CThing* thing) { return thing == Blocker; }
};
__forceinline bool IsThingAlive(const CThing* thing) { return thing && !thing->Dead; }
struct CVillagerNode { CVillagerNode* _M_next; CVillagerNode* _M_prev; CBaseIntelligentPointer Villager; };
struct CVillage {
    unsigned char _pad00[0x2c];
    CVillagerNode* Villagers;                       // +0x2c (list header)
};
class CTCVillageMember { public: CVillage* Village; CVillage* GetPVillage() { return Village; } };
struct CTCHeroBuilding { unsigned char _pad00[0x0c]; CBaseIntelligentPointer Building; };   // TC 0x75, +0x0c
class CPlayer { public: CThing* Hero; CThing* GetHeroCreature() { return Hero; } };
class CPlayerManager { public: CPlayer* Player; CPlayer* GetMainPlayer() { return Player; } };
class CTCShopKeeper { public:
    void*   __vftable;
    CThing* ParentThing;                            // +0x04
    CPlayerManager* Manager;
    CPlayerManager* GetPlayerManager() { return Manager; }
    bool CalcAreOtherPeopleInShop();
};

bool CTCShopKeeper::CalcAreOtherPeopleInShop()
{
    CThing* owner = ParentThing;
    if (owner && !owner->Dead && owner->State != 1)
        return false;
    CThingBuilding* building = (CThingBuilding*)owner->HomeBuilding.GetPItem();
    if (!IsThingAlive(building))
        return false;
    CTCVillageMember* member;
    if (owner->GetInterface(0x23, &member))
    {
        CVillage* village = member->GetPVillage();
        if (village)
        {
            for (CVillagerNode* node = village->Villagers->_M_next; node != village->Villagers; node = node->_M_next)
            {
                CBaseIntelligentPointer& villager = node->Villager;
                if (villager.GetPItem() && !villager.GetPItem()->Dead && villager.GetPItem() != owner)
                {
                    CThing* other = villager.GetPItem();
                    CThing* home = other->HomeBuilding.GetPItem();
                    if (building->IsPointInside(villager.GetPItem()->GetPosition()) != (home == building))
                        return true;
                }
                if (villager.GetPItem() && !villager.GetPItem()->Dead && villager.GetPItem() != owner)
                {
                    if (building->IsDoorBlockedBy(villager.GetPItem()))
                        return true;
                }
            }
        }
    }
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    CTCHeroBuilding* heroBuilding;
    if (hero && !hero->Dead && hero->GetInterface(0x75, &heroBuilding))
    {
        if (heroBuilding->Building.GetPItem() == building)
            return true;
        if (building->IsDoorBlockedBy(hero))
            return true;
    }
    return false;
}

static void SetTC(CThing& t, long type, CInterfaceMap::SEntry* entries, int n)
{
    t.TCBits[type >> 5] |= 1UL << (type & 31);
    t.Interfaces.first = entries; t.Interfaces.last = entries + n; t.Interfaces.end = entries + n;
}
static void Clear(CThing& t) { memset(&t.TCBits, 0, sizeof(t.TCBits)); t.State = 0; t.Dead = 0; t.HomeBuilding.m_pItem = 0; t.Interfaces.first = t.Interfaces.last = t.Interfaces.end = 0; t.Pos.X = t.Pos.Y = t.Pos.Z = 0; }
int main()
{
    int fails = 0;
    static CThing owner, villagerThing, hero; static CThingBuilding building, otherHome;
    Clear(owner); Clear(villagerThing); Clear(hero); Clear(building); Clear(otherHome);
    building.MinX = 0; building.MaxX = 10; building.Blocker = 0;
    CPlayer player; player.Hero = 0; CPlayerManager pm; pm.Player = &player;
    CTCShopKeeper keeper; keeper.ParentThing = &owner; keeper.Manager = &pm;
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // alive keeper not in state 1 -> false
    owner.State = 1;
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // no building -> false
    owner.HomeBuilding.m_pItem = (CBaseObject*)&building;
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // nobody around -> false
    CVillagerNode header, node; header._M_next = &node; node._M_next = &header; node.Villager.m_pItem = (CBaseObject*)&villagerThing;
    CVillage village; village.Villagers = &header;
    CTCVillageMember member; member.Village = &village;
    CInterfaceMap::SEntry ownerEntries[1] = { { 0x23, (CTCBase*)&member } };
    SetTC(owner, 0x23, ownerEntries, 1);
    villagerThing.Pos.X = 50; villagerThing.HomeBuilding.m_pItem = (CBaseObject*)&otherHome;
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // villager outside, lives elsewhere -> false
    villagerThing.Pos.X = 5;
    if (!keeper.CalcAreOtherPeopleInShop()) ++fails;                // inside, lives elsewhere -> true
    villagerThing.HomeBuilding.m_pItem = (CBaseObject*)&building;
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // inside and lives there -> false
    villagerThing.Pos.X = 50;
    if (!keeper.CalcAreOtherPeopleInShop()) ++fails;                // lives there but outside -> true
    villagerThing.Pos.X = 5; building.Blocker = &villagerThing;
    if (!keeper.CalcAreOtherPeopleInShop()) ++fails;                // blocks the door -> true
    villagerThing.Dead = 1;
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // dead villagers ignored
    building.Blocker = 0; player.Hero = &hero;
    CTCHeroBuilding heroTC; heroTC.Building.m_pItem = 0;
    CInterfaceMap::SEntry heroEntries[1] = { { 0x75, (CTCBase*)&heroTC } };
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // hero without TC -> false
    SetTC(hero, 0x75, heroEntries, 1);
    if (keeper.CalcAreOtherPeopleInShop()) ++fails;                 // hero elsewhere -> false
    heroTC.Building.m_pItem = (CBaseObject*)&building;
    if (!keeper.CalcAreOtherPeopleInShop()) ++fails;                // hero in this shop -> true
    heroTC.Building.m_pItem = 0; building.Blocker = &hero;
    if (!keeper.CalcAreOtherPeopleInShop()) ++fails;                // hero blocks door -> true
    if (fails) { printf("00799140_TEST FAIL %d\n", fails); return 1; }
    printf("00799140_TEST PASS\n");
    return 0;
}
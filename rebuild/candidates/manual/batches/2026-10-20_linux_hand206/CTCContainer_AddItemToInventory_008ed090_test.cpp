// Behaviour test for CTCContainer::AddItemToInventory (0x008ED090): an inventory item goes into the hero's inventory
// component of its type with (false, true, 0, false, false); a non-item thing is killed (not immediately).
#include <cstdio>
#include <stdlib.h>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    int Kills; bool KillFlag;
    unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    bool TestComponent(unsigned long position) const
    {
        if (position >= 274) { puts("bitset"); abort(); }
        return (UsingComponents[position >> 5] & (1UL << (position & 31))) != 0;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc = 0;                                // the retail local is unset when absent; the test only takes the present arm
        if (TestComponent(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
    void Kill(bool immediately) { ++Kills; KillFlag = immediately; }
    void Init() { for (int i = 0; i < 9; ++i) UsingComponents[i] = 0; Components.first = Components.last = 0; Kills = 0; KillFlag = true; }
};
struct CTCInventoryBase { int id; };
struct CTCInventoryItem {
    long Type; int Adds; CTCInventoryBase* Into; bool A, B, D, E; long C;
    long GetInventoryType() { return Type; }
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool e)
    { ++Adds; Into = inventory; A = a; B = b; C = c; D = d; E = e; return true; }
};
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroCreature() { return g_hero; } };
static CPlayer g_player;
struct CPlayerManager { CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_pm;
struct CTCContainer {
    virtual void v00() {}
    CThing* ParentThing;
    CPlayerManager* GetPlayerManager() { return &g_pm; }
    void AddItemToInventory(CThing* item);
};

void CTCContainer::AddItemToInventory(CThing* item)
{
    if (item->HasTC(0x1a)) {
        CTCInventoryItem* inventoryItem = (CTCInventoryItem*)item->GetTC(0x1a);
        long type = inventoryItem->GetInventoryType();
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
        inventoryItem->AddToInventory((CTCInventoryBase*)hero->GetTCIfPresent(type), false, true, 0, false, false);
    } else
        item->Kill(false);
}

int main()
{
    int bad = 0;
    CTCInventoryBase weapons, potions;
    CThing hero; hero.Init();
    CInterfaceMap::SEntry he[2] = { { 0x60, (CTCBase*)&weapons }, { 0x61, (CTCBase*)&potions } };
    hero.Components.first = he; hero.Components.last = he + 2; hero.UsingComponents[3] = 3;   // 0x60, 0x61
    g_hero = &hero;
    CTCInventoryItem invItem; invItem.Type = 0x61; invItem.Adds = 0;
    CThing item; item.Init();
    CInterfaceMap::SEntry ie[1] = { { 0x1a, (CTCBase*)&invItem } };
    item.Components.first = ie; item.Components.last = ie + 1; item.UsingComponents[0] = 1UL << 26;
    CTCContainer c; c.ParentThing = 0;
    c.AddItemToInventory(&item);
    if (invItem.Adds != 1 || invItem.Into != &potions || invItem.A || !invItem.B || invItem.C || invItem.D || invItem.E || item.Kills) bad |= 1;
    invItem.Type = 0x60;
    c.AddItemToInventory(&item);
    if (invItem.Adds != 2 || invItem.Into != &weapons) bad |= 2;
    CThing junk; junk.Init();
    c.AddItemToInventory(&junk);
    if (junk.Kills != 1 || junk.KillFlag || invItem.Adds != 2) bad |= 4;
    if (bad) { std::printf("008ed090_TEST FAIL\n"); return 1; }
    std::printf("008ed090_TEST PASS\n");
    return 0;
}

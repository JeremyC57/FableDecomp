// Behaviour test for 005ee418: body copied from src/005ee418.cpp (CTCInventoryAbilities::FrameUpdate), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CPresenceBits {
    unsigned long w[9];
    bool test(long bit) { return (w[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTCUnchecked(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
static int g_tutCalls = 0; static long g_tutCat = -1; static int g_tutForce = -1;
struct CTCHero {
    bool GiveTutorialForCategory(long category, bool force) { ++g_tutCalls; g_tutCat = category; g_tutForce = force; return true; }
};
struct CWorld { unsigned char _pad_0x00[0xA4]; bool Paused; };
struct CPlayerGui { unsigned char _pad_0x00[0xFD]; bool BlockingTutorials; };
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
static CWorld g_world;
struct CTCInventoryAbilities {
    void* __vftable;
    CThing* ParentThing;
    unsigned char _pad_0x08[0x50];
    bool InventoryOpen;
    unsigned char _pad_0x59[0xE7];
    bool WaitingToGiveTutorial;
    CWorld* GetWorld() const { return &g_world; }
    void FrameUpdate();
};
void CTCInventoryAbilities::FrameUpdate()
{
    if (WaitingToGiveTutorial && !InventoryOpen && !g_pPlayerGui->BlockingTutorials && !GetWorld()->Paused) {
        CThing* thing = ParentThing;
        if (thing->HasTC(0x29)) {
            CTCHero* hero = thing->GetTCUnchecked<CTCHero>(0x29);
            if (hero)
                hero->GiveTutorialForCategory(2, false);
        }
        WaitingToGiveTutorial = false;
    }
}
int main()
{
    CThing thing; memset(&thing, 0, sizeof(thing)); CTCHero hero;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x29, (CTCBase*)&hero } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCInventoryAbilities a; memset(&a, 0, sizeof(a)); a.ParentThing = &thing;
    g_gui.BlockingTutorials = false; g_world.Paused = false;
    // 1. not waiting: nothing
    a.FrameUpdate(); CHECK(g_tutCalls == 0);
    // 2. waiting but inventory open / gui blocking / world paused: flag stays set
    a.WaitingToGiveTutorial = true;
    a.InventoryOpen = true; a.FrameUpdate(); CHECK(g_tutCalls == 0); CHECK(a.WaitingToGiveTutorial); a.InventoryOpen = false;
    g_gui.BlockingTutorials = true; a.FrameUpdate(); CHECK(g_tutCalls == 0); CHECK(a.WaitingToGiveTutorial); g_gui.BlockingTutorials = false;
    g_world.Paused = true; a.FrameUpdate(); CHECK(g_tutCalls == 0); CHECK(a.WaitingToGiveTutorial); g_world.Paused = false;
    // 3. all clear, no hero component: flag cleared, no tutorial
    a.FrameUpdate(); CHECK(g_tutCalls == 0); CHECK(!a.WaitingToGiveTutorial);
    // 4. hero component present: tutorial category 2, not forced; flag cleared
    a.WaitingToGiveTutorial = true; thing.m_present.w[0x29 >> 5] = 1UL << (0x29 & 31);
    a.FrameUpdate(); CHECK(g_tutCalls == 1); CHECK(g_tutCat == 2); CHECK(g_tutForce == 0); CHECK(!a.WaitingToGiveTutorial);
    // 5. bit set but the entry's component is null: no call
    a.WaitingToGiveTutorial = true; entries[1].component = 0;
    a.FrameUpdate(); CHECK(g_tutCalls == 1); CHECK(!a.WaitingToGiveTutorial);
    if (fails == 0) printf("005ee418_TEST PASS\n"); else printf("005ee418_TEST FAIL\n");
    return fails;
}
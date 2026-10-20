// Behaviour test for 00784600: body copied from src/00784600.cpp (CTCCreatureGroupBoastingCrowd::OnHeroApproachBoastingArea), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};

static int g_tutorials = 0; static long g_tutCategory = -1; static bool g_tutForce = true; static void* g_tutThis = 0;
struct CTCHero { bool GiveTutorialForCategory(long category, bool force) { ++g_tutorials; g_tutCategory = category; g_tutForce = force; g_tutThis = this; return true; } };
struct SListNode { SListNode* next; SListNode* prev; };
struct CQuestManager {
    unsigned char _pad00[0x68];
    SListNode* m_boasts;
    bool BoastsEmpty() const { return m_boasts->next == m_boasts; }
};
CQuestManager* g_pQuestManager;
static int g_getPItem = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CThing* GetPItem() const { ++g_getPItem; return (CThing*)m_pItem; }
};
static int g_enterState = 0; static long g_state = -1;
struct CTCCreatureGroupBoastingCrowd {
    virtual void v00() {}
    CThing* ParentThing;
    unsigned char _pad08[0x24 - 0x08];
    CBaseIntelligentPointer PHero;
    long CrowdState;
    long CurrentEncouragementState;
    bool HeroBoasted;
    bool HeroMadeGoodBoast;
    void EnterCrowdState(long state) { ++g_enterState; g_state = state; }
    void OnHeroApproachBoastingArea();
};
void CTCCreatureGroupBoastingCrowd::OnHeroApproachBoastingArea()
{
    if (!g_pQuestManager->BoastsEmpty() && !HeroBoasted) {
        if (PHero.GetPItem() && !PHero.GetPItem()->IsDead()) {
            CTCHero* hero = 0;
            CThing* heroThing = PHero.GetPItem();
            if (heroThing->HasTC(0x29))
                hero = heroThing->GetTC<CTCHero>(0x29);
            if (hero)
                hero->GiveTutorialForCategory(5, false);
        }
        EnterCrowdState(1);
    }
}
int main()
{
    CQuestManager qm; SListNode head; SListNode boast; head.next = &boast; boast.next = &head; head.prev = &boast; boast.prev = &head; qm.m_boasts = &head; g_pQuestManager = &qm;
    CThing hero; memset(&hero, 0, sizeof(hero)); CTCHero heroTC;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x29, (CTCBase*)&heroTC } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    CTCCreatureGroupBoastingCrowd crowd; memset(&crowd, 0, sizeof(crowd)); crowd.PHero.m_pItem = &hero;
    // 1. no boasts: nothing
    head.next = &head; crowd.OnHeroApproachBoastingArea(); CHECK(g_enterState == 0 && g_getPItem == 0);
    head.next = &boast;
    // 2. hero already boasted: nothing
    crowd.HeroBoasted = true; crowd.OnHeroApproachBoastingArea(); CHECK(g_enterState == 0); crowd.HeroBoasted = false;
    // 3. no hero thing: state entered, no tutorial
    crowd.PHero.m_pItem = 0; crowd.OnHeroApproachBoastingArea(); CHECK(g_enterState == 1 && g_state == 1 && g_tutorials == 0 && g_getPItem == 1);
    // 4. dead hero: state entered, no tutorial
    crowd.PHero.m_pItem = &hero; hero.Flags91 = 1; crowd.OnHeroApproachBoastingArea(); CHECK(g_enterState == 2 && g_tutorials == 0 && g_getPItem == 3); hero.Flags91 = 0;
    // 5. hero without the hero component: no tutorial
    crowd.OnHeroApproachBoastingArea(); CHECK(g_enterState == 3 && g_tutorials == 0 && g_getPItem == 6);
    // 6. hero component present: the tutorial
    hero.m_presentBits[0x29 >> 5] = 1UL << (0x29 & 31); crowd.OnHeroApproachBoastingArea();
    CHECK(g_enterState == 4 && g_tutorials == 1 && g_tutCategory == 5 && g_tutForce == false && g_tutThis == &heroTC);
    if (fails == 0) printf("00784600_TEST PASS\n"); else printf("00784600_TEST FAIL\n");
    return fails;
}
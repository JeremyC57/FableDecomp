// Behaviour test for ~CTCCreatureGroupBoastingCrowd (0x00784BD0): with a live hero the crowd state is reset (0), every pushed
// opinion source is cleared (flag true) until the count is 0, and the hero's 0x73 component drops the boast; the hero pointer
// and the group base are destroyed in every case.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    ~CBaseIntelligentPointer() { Log('p'); }
    CThing* GetPItem() const { return m_pItem; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { };
static int g_cleared = 0; static CThing* g_clearedThing = 0; static bool g_clearedFlag = false;
struct COpinionReactionManager {
    void ClearOpinionSource(CThing* thing, bool flag) { ++g_cleared; g_clearedThing = thing; g_clearedFlag = flag; Log('c'); }
};
struct CWorld { COpinionReactionManager* OpinionReactionManager; };
static COpinionReactionManager g_orm; static CWorld g_world = { &g_orm };
struct CTCBoastComponent {
    CThing* Removed;
    void RemoveBoast(CThing* hero) { Removed = hero; Log('r'); }
};
struct CTCBase {
    virtual ~CTCBase() { Log('b'); }
    CThing* ParentThing;
    CWorld* GetWorld() { return &g_world; }
};
struct CTCCreatureGroup : CTCBase {
    virtual ~CTCCreatureGroup() { Log('g'); }
};
struct CTCCreatureGroupBoastingCrowd : CTCCreatureGroup {
    CIntelligentPointer PHero;
    long HeroOpinionSourcesPushed;
    long LastState;
    void EnterCrowdState(long state) { LastState = state; Log('e'); }
    virtual ~CTCCreatureGroupBoastingCrowd();
};

CTCCreatureGroupBoastingCrowd::~CTCCreatureGroupBoastingCrowd()
{
    if (PHero.GetPItem() && !PHero.GetPItem()->IsDead()) {
        EnterCrowdState(0);
        while (HeroOpinionSourcesPushed > 0) {
            COpinionReactionManager* manager = GetWorld()->OpinionReactionManager;
            manager->ClearOpinionSource(PHero.GetPItem(), true);
            --HeroOpinionSourcesPushed;
        }
        CThing* hero = PHero.GetPItem();
        if (hero->HasTC(0x73)) {
            CTCBoastComponent* boast = (CTCBoastComponent*)hero->GetTC(0x73);
            boast->RemoveBoast(PHero.GetPItem());
        }
    }
}

int main()
{
    int bad = 0;
    CTCBoastComponent boast; boast.Removed = 0;
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0; hero.Flags91 = 0;
    CInterfaceMap::SEntry e[1] = { { 0x73, (CTCBase*)&boast } };
    hero.m_interfaces.first = e; hero.m_interfaces.last = e + 1; hero.m_presentBits[3] = 1UL << 19;
    {
        CTCCreatureGroupBoastingCrowd* c = new CTCCreatureGroupBoastingCrowd;
        c->PHero.m_pItem = &hero; c->HeroOpinionSourcesPushed = 3; c->LastState = -1;
        g_log[0] = 0;
        delete c;
        if (std::strcmp(g_log, "ecccrpgb") || g_cleared != 3 || g_clearedThing != &hero || !g_clearedFlag || boast.Removed != &hero) bad |= 1;
    }
    {
        hero.Flags91 = 1; g_cleared = 0;
        CTCCreatureGroupBoastingCrowd* c = new CTCCreatureGroupBoastingCrowd;
        c->PHero.m_pItem = &hero; c->HeroOpinionSourcesPushed = 2;
        g_log[0] = 0;
        delete c;                                    // dead hero: only the member/base dtors
        if (std::strcmp(g_log, "pgb") || g_cleared) bad |= 2;
    }
    {
        hero.Flags91 = 0; hero.m_presentBits[3] = 0;
        CTCCreatureGroupBoastingCrowd* c = new CTCCreatureGroupBoastingCrowd;
        c->PHero.m_pItem = &hero; c->HeroOpinionSourcesPushed = 0;
        g_log[0] = 0;
        delete c;                                    // nothing pushed, no boast component
        if (std::strcmp(g_log, "epgb")) bad |= 4;
    }
    if (bad) { std::printf("00784bd0_TEST FAIL\n"); return 1; }
    std::printf("00784bd0_TEST PASS\n");
    return 0;
}

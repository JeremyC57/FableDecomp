// Behaviour test for CTCBoastingPodium::OnHeroBoast (0x00813C50).
#include <cstdio>
#include <cstring>

struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    bool HasTC(long t) const { if (m_presentBits[t >> 5] & (1UL << (t & 31))) return true; return false; }
    template<class T> T* GetTC(long t)
    {
        long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
};
class CCharString { public: const char* PStringData; CCharString(const char* text, long = -1) : PStringData(text) {} };
struct CBoast { long ID; long Cost; long Reward; };
static CBoast g_boast = { 9, 250, 0 };
static long g_madeId = -1; static bool g_nameEmpty = false;
class CQuestManager { public:
    CBoast* GetBoast(long id, CCharString scriptName) { g_nameEmpty = std::strcmp(scriptName.PStringData, "") == 0; return id == 9 ? &g_boast : 0; }
    void SetBoastAsMade(long id, CCharString) { g_madeId = id; }
};
static CQuestManager g_qm;
static CQuestManager* g_pQuestManager = &g_qm;
static CBoast* GetBoast(long id) { CQuestManager* questManager = g_pQuestManager; return questManager->GetBoast(id, ""); }
static void SetBoastAsMade(long id) { CQuestManager* questManager = g_pQuestManager; questManager->SetBoastAsMade(id, ""); }
static long g_money = 1000; static bool g_show = false;
class CTCHeroStats { public: void AddMoney(long amount, bool show) { g_money += amount; g_show = show; } };
class CTCBoastingPodium {
public:
    long LastPositionsWithBoastCount;
    void OnHeroBoast(long boastId, CThing* hero)
    {
        if (hero && !hero->IsDead()) {
            CBoast* boast = GetBoast(boastId);
            if (boast && hero->HasTC(4))
                hero->GetTC<CTCHeroStats>(4)->AddMoney(-boast->Cost, true);
        }
        SetBoastAsMade(boastId);
        --LastPositionsWithBoastCount;
    }
};

int main()
{
    CTCHeroStats stats;
    CInterfaceMap::SEntry entries[1] = { { 4, (CTCBaseInterface*)&stats } };
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    hero.m_presentBits[0] = 1UL << 4; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1; hero.Flags91 = 0;
    CTCBoastingPodium podium; podium.LastPositionsWithBoastCount = 3;
    podium.OnHeroBoast(9, &hero);
    bool ok = g_money == 750 && g_show && g_nameEmpty && g_madeId == 9 && podium.LastPositionsWithBoastCount == 2;
    podium.OnHeroBoast(5, &hero);                       // unknown boast: no payment
    ok = ok && g_money == 750 && g_madeId == 5 && podium.LastPositionsWithBoastCount == 1;
    hero.Flags91 = 1;
    podium.OnHeroBoast(9, &hero);                       // dead hero: no payment, still made
    ok = ok && g_money == 750 && g_madeId == 9 && podium.LastPositionsWithBoastCount == 0;
    podium.OnHeroBoast(9, 0);
    ok = ok && g_money == 750 && podium.LastPositionsWithBoastCount == -1;
    if (!ok) { std::printf("00813c50_TEST FAIL\n"); return 1; }
    std::printf("00813c50_TEST PASS\n");
    return 0;
}
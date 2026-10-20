// Behaviour test for CTCHeroStats::AddRenown (0x0057D8BE): floors both counters, levels up while the level is
// paid for (carrying the remainder), shows the change on the overhead display (if present) and in the GUI.
#include <cstdio>
// --- test-side copy of the CThing interface lookup (std::bitset presence + sorted {type, component} map) ---
struct CTCBase;
struct CPresenceBits {
    long bits[16];
    bool test(long bit) { return bit >= 0 && bit < 512 && ((bits[bit >> 5] >> (bit & 31)) & 1) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry entries[4];
    void Init() { for (int i = 0; i < 16; ++i) m_present.bits[i] = 0; m_interfaces.first = m_interfaces.last = entries; }
    void Add(long type, void* comp) { m_present.bits[type >> 5] |= 1L << (type & 31); m_interfaces.last->type = type; m_interfaces.last->component = (CTCBase*)comp; ++m_interfaces.last; }
    template<class T> bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
// --- end lookup copy ---
static inline void ClampMin(long& value, long minimum) { if (value < minimum) value = minimum; }
struct CLongVector { long* m_pFirst; long* m_pLast; long* m_pEnd; };
struct CHeroStatsDef { CLongVector RenownLevelCosts; };
struct CDefinitionManager { CHeroStatsDef* PHeroStatsDef; };
static CDefinitionManager g_defs;
static float g_overhead = 0.0f; static int g_overheadCalls = 0;
struct CTCOverheadDisplay { void AddRenownEntry(float renown) { g_overhead = renown; ++g_overheadCalls; } };
static long g_gui = 0; static int g_guiCalls = 0;
struct CPlayerGui { void SetRenownChange_00437920(long renown) { g_gui = renown; ++g_guiCalls; } };
static CPlayerGui g_gui_object; CPlayerGui* g_pPlayerGui = &g_gui_object;
struct CTCHeroStats {
    CThing* ParentThing;
    long RenownLevel, RenownMaxLevel, RenownPointsInLevel, RenownTotal;
    int upgrades;
    CDefinitionManager* GetDefinitionManager() const { return &g_defs; }
    float GetPercentageThroughRenownLevel() const
    {
        const CHeroStatsDef* def = GetDefinitionManager()->PHeroStatsDef;
        if (RenownLevel >= 0 && RenownLevel < def->RenownLevelCosts.m_pLast - def->RenownLevelCosts.m_pFirst)
            return (float)RenownPointsInLevel / def->RenownLevelCosts.m_pFirst[RenownLevel];
        return 0.0f;
    }
    void UpgradeRenownLevel() { ++upgrades; if (++RenownLevel > RenownMaxLevel) RenownLevel = RenownMaxLevel; RenownPointsInLevel = 0; }
    void AddRenown(long renown);
};
void CTCHeroStats::AddRenown(long renown)
{
    RenownPointsInLevel += renown;
    ClampMin(RenownPointsInLevel, 0);
    RenownTotal += renown;
    ClampMin(RenownTotal, 0);
    while (GetPercentageThroughRenownLevel() >= 1.0f) {
        const CHeroStatsDef* def = GetDefinitionManager()->PHeroStatsDef;
        long carried = RenownPointsInLevel - def->RenownLevelCosts.m_pFirst[RenownLevel];
        UpgradeRenownLevel();
        RenownPointsInLevel = carried;
    }
    if (renown != 0) {
        CTCOverheadDisplay* display;
        if (ParentThing->GetInterface(&display, 0xB7))
            display->AddRenownEntry((float)renown);
        g_pPlayerGui->SetRenownChange_00437920(renown);
    }
}
int main()
{
    long costs[3] = { 10, 20, 40 };
    CHeroStatsDef def; def.RenownLevelCosts.m_pFirst = costs; def.RenownLevelCosts.m_pLast = def.RenownLevelCosts.m_pEnd = costs + 3;
    g_defs.PHeroStatsDef = &def;
    CThing thing; thing.Init();
    CTCHeroStats s; s.ParentThing = &thing; s.RenownLevel = 0; s.RenownMaxLevel = 3; s.RenownPointsInLevel = 5; s.RenownTotal = 5; s.upgrades = 0;
    s.AddRenown(30);   // 35: level 0 costs 10 -> 25, level 1 costs 20 -> 5 at level 2
    if (s.RenownLevel != 2 || s.RenownPointsInLevel != 5 || s.RenownTotal != 35 || s.upgrades != 2 || g_overheadCalls != 0 || g_guiCalls != 1 || g_gui != 30) { std::printf("0057d8be_TEST FAIL\n"); return 1; }
    CTCOverheadDisplay display; thing.Add(0xB7, &display);
    s.AddRenown(-100);
    if (s.RenownPointsInLevel != 0 || s.RenownTotal != 0 || s.RenownLevel != 2 || g_overheadCalls != 1 || g_overhead != -100.0f || g_gui != -100) { std::printf("0057d8be_TEST FAIL\n"); return 1; }
    s.AddRenown(0);
    if (g_guiCalls != 2 || g_overheadCalls != 1) { std::printf("0057d8be_TEST FAIL\n"); return 1; }
    std::printf("0057d8be_TEST PASS\n");
    return 0;
}

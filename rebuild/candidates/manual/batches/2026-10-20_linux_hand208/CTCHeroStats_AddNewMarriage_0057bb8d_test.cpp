// Behaviour test for CTCHeroStats::AddNewMarriage (0x0057BB8D): the record carries both UIDs (0 without a home),
// the state and the same-sex flag; the wedding counter and both scoreboard activities are updated.
#include <cstdio>
#ifndef _MSC_VER
#define __int64 long long
#endif
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
    unsigned __int64 UID;
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
struct CThingCreatureBase : CThing {
    long Sex;
    long GetSex() { return Sex; }
};
enum EMarriageState { MARRIAGE_STATE_NONE, MARRIAGE_STATE_MARRIED };
struct CHeroMarriageInfo {
    unsigned __int64 WifeUID; unsigned __int64 MaritalHomeUID; EMarriageState State; bool GayMarriage;
    CHeroMarriageInfo(unsigned __int64 wife, unsigned __int64 home, EMarriageState state, bool gay)
        : WifeUID(wife), MaritalHomeUID(home), State(state), GayMarriage(gay) {}
};
static CHeroMarriageInfo g_last(0, 0, MARRIAGE_STATE_NONE, false);
static int g_pushes = 0;
struct CHeroMarriageList { void* _M_node; void push_back(const CHeroMarriageInfo& info) { g_last = info; ++g_pushes; } };
static long g_act[4]; static float g_score[4]; static int g_calls = 0;
struct CTCHeroOnlineScoreboard {
    void UpdateScoreForActivity(long activity, float score, bool) { if (g_calls < 4) { g_act[g_calls] = activity; g_score[g_calls] = score; } ++g_calls; }
};
struct CTCHeroStats {
    CThing* ParentThing;
    long NumWeddings, NumSpousesMurdered, NumSpousesDied;
    CHeroMarriageList Marriages;
    void AddNewMarriage(CThingCreatureBase* wife, CThing* home, EMarriageState state);
};
void CTCHeroStats::AddNewMarriage(CThingCreatureBase* wife, CThing* home, EMarriageState state)
{
    unsigned __int64 homeUID;
    if (home)
        homeUID = home->UID;
    else
        homeUID = 0;
    unsigned __int64 wifeUID = wife->UID;
    Marriages.push_back(CHeroMarriageInfo(wifeUID, homeUID, state, wife->GetSex() == 1));
    ++NumWeddings;
    CTCHeroOnlineScoreboard* scoreboard;
    ParentThing->GetInterface(&scoreboard, 0x10D);
    scoreboard->UpdateScoreForActivity(12, (float)NumWeddings, false);
    long currentSpouses = NumWeddings - NumSpousesDied - NumSpousesMurdered;
    scoreboard->UpdateScoreForActivity(9, (float)currentSpouses, false);
}
int main()
{
    CThing hero; hero.Init(); CTCHeroOnlineScoreboard board; hero.Add(0x10D, &board);
    CThingCreatureBase wife; wife.Init(); wife.UID = 0x123456789ULL; wife.Sex = 2;
    CThing house; house.Init(); house.UID = 0x55AA00000001ULL;
    CTCHeroStats s; s.ParentThing = &hero; s.NumWeddings = 3; s.NumSpousesMurdered = 1; s.NumSpousesDied = 1;
    s.AddNewMarriage(&wife, &house, MARRIAGE_STATE_MARRIED);
    if (g_pushes != 1 || g_last.WifeUID != 0x123456789ULL || g_last.MaritalHomeUID != 0x55AA00000001ULL ||
        g_last.State != MARRIAGE_STATE_MARRIED || g_last.GayMarriage || s.NumWeddings != 4 ||
        g_calls != 2 || g_act[0] != 12 || g_score[0] != 4.0f || g_act[1] != 9 || g_score[1] != 2.0f) { std::printf("0057bb8d_TEST FAIL\n"); return 1; }
    wife.Sex = 1;
    s.AddNewMarriage(&wife, 0, MARRIAGE_STATE_NONE);
    if (g_pushes != 2 || g_last.MaritalHomeUID != 0 || !g_last.GayMarriage || s.NumWeddings != 5) { std::printf("0057bb8d_TEST FAIL\n"); return 1; }
    std::printf("0057bb8d_TEST PASS\n");
    return 0;
}

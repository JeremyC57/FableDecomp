// Behaviour test for CTCHeroStats::AddMoney (0x0057B327): clamp to [0, def max], high-water mark from the
// argument, earned money reported to the scoreboard component (interface 0x10D), spent money booked.
#include <cstdio>
// --- test-side copy of the CThing interface lookup (std::bitset presence + sorted {type, component} map) ---
class CTCBase;
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
class CThing { public:
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
template <class T> inline const T& Max(const T& left, const T& right) { return left < right ? right : left; }
template <class T> inline void GFLimit(T& value, const T& lo, const T& hi)
{
    if (value < lo) value = lo;
    else if (value > hi) value = hi;
}
class CHeroStatsDef { public: long MaxMoney; };
class CDefinitionManager { public: CHeroStatsDef* PHeroStatsDef; };
static long g_activity = -1; static float g_score = -1.0f; static int g_calls = 0; static bool g_flag = true;
class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter) { g_activity = activity; g_score = score; g_flag = onlyIfBetter; ++g_calls; }
};
static CDefinitionManager g_defs;
class CTCHeroStats { public:
    CThing* ParentThing;
    long Money, MaxMoney, ConfiscatedMoney, TotalMoneyEarned, TotalMoneySpent;
    CDefinitionManager* GetDefinitionManager() const { return &g_defs; }
    void AddMoney(long amount, bool countTransaction);
};
void CTCHeroStats::AddMoney(long amount, bool countTransaction)
{
    long oldMoney = Money;
    Money = oldMoney + amount;
    GFLimit(Money, 0L, GetDefinitionManager()->PHeroStatsDef->MaxMoney);
    MaxMoney = Max(MaxMoney, amount);
    long change = Money - oldMoney;
    if (change > 0) {
        if (countTransaction) {
            TotalMoneyEarned += change;
            CTCHeroOnlineScoreboard* scoreboard;
            ParentThing->GetInterface(&scoreboard, 0x10D);
            scoreboard->UpdateScoreForActivity(5, (float)TotalMoneyEarned, false);
        }
    } else if (change < 0) {
        if (countTransaction)
            TotalMoneySpent -= change;
    }
}
int main()
{
    CHeroStatsDef def; def.MaxMoney = 1000; g_defs.PHeroStatsDef = &def;
    CThing thing; thing.Init();
    CTCHeroOnlineScoreboard board; thing.Add(0x10D, &board);
    CTCHeroStats s; s.ParentThing = &thing; s.Money = 100; s.MaxMoney = 50; s.ConfiscatedMoney = 0; s.TotalMoneyEarned = 10; s.TotalMoneySpent = 0;
    s.AddMoney(200, true);
    if (s.Money != 300 || s.MaxMoney != 200 || s.TotalMoneyEarned != 210 || g_calls != 1 || g_activity != 5 || g_score != 210.0f || g_flag) { std::printf("0057b327_TEST FAIL\n"); return 1; }
    s.AddMoney(5000, false);            // clamped to the def maximum, nothing booked
    if (s.Money != 1000 || s.MaxMoney != 5000 || s.TotalMoneyEarned != 210 || g_calls != 1) { std::printf("0057b327_TEST FAIL\n"); return 1; }
    s.AddMoney(-1500, true);            // clamped at 0, the real change (-1000) booked as spent
    if (s.Money != 0 || s.TotalMoneySpent != 1000 || s.MaxMoney != 5000 || g_calls != 1) { std::printf("0057b327_TEST FAIL\n"); return 1; }
    std::printf("0057b327_TEST PASS\n");
    return 0;
}
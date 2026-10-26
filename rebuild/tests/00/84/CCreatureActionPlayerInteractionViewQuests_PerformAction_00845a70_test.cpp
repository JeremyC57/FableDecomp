// Behaviour test for 0x00845A70 (see src/00845a70.cpp).
#include <cstdio>
#include <cstring>

// Interface table model shared by the GetInterface-based rows: presence bits + sorted {type, component} entries.
struct CEntry { int type; void* component; };
class CThing { public:
    unsigned int presentBits[9];
    CEntry entries[4];
    int count;
    unsigned char flags91;                  // bit 0: dead
    unsigned char state10;
    void* cached5c;                         // the cached driver component
    unsigned char driverFlags28;
    CThing() { std::memset(presentBits, 0, sizeof(presentBits)); count = 0; flags91 = 0; state10 = 0; cached5c = 0; driverFlags28 = 0; }
    void Add(int type, void* c) { presentBits[type >> 5] |= 1u << (type & 31); entries[count].type = type; entries[count].component = c; ++count; }
    template<class T> bool GetInterface(T** out, int type)
    {
        if (presentBits[type >> 5] & (1u << (type & 31))) {
            CEntry* it = entries;
            CEntry* last = entries + count;
            while (it != last && it->type < type) ++it;
            if (it == last || type < it->type) it = last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
inline bool IS_THING_ALIVE(CThing* p) { return p != 0 && !(p->flags91 & 1); }
static int g_log[32];
static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
static bool Expect(const int* want, int n) { if (n != g_n) return false; for (int i = 0; i < n; ++i) if (g_log[i] != want[i]) return false; return true; }
static void Reset() { g_n = 0; }

#define CHECK(c) do { if (!(c)) { std::printf("00845a70_TEST FAIL\n"); return 1; } } while (0)

struct CGuide { int opened; void OpenQuestSelectInterface() { ++opened; Log(1); } };
struct CAction { CThing* creature; CThing* guide; void FinishAction() { Log(2); }
    void PerformAction()
    {
        if (IS_THING_ALIVE(creature) && IS_THING_ALIVE(guide)) {
            CGuide* g; guide->GetInterface(&g, 0x6B);
            if (g) { g->OpenQuestSelectInterface(); FinishAction(); }
        }
    } };
int main()
{
    CThing hero, npc; CGuide g; g.opened = 0; npc.Add(0x6B, &g);
    CHECK((npc.presentBits[3] & 0x800) != 0);
    CAction a; a.creature = &hero; a.guide = &npc; a.PerformAction();
    { int want[] = { 1, 2 }; CHECK(Expect(want, 2)); }
    hero.flags91 = 1; Reset(); a.PerformAction(); CHECK(g_n == 0);
    hero.flags91 = 0; CGuide* none = 0; npc.entries[0].component = none; a.PerformAction(); CHECK(g_n == 0);
    std::printf("00845a70_TEST PASS\n");
    return 0;
}
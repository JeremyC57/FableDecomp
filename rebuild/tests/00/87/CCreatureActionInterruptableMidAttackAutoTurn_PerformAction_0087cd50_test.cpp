// Behaviour test for 0x0087CD50 (see src/0087cd50.cpp).
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

#define CHECK(c) do { if (!(c)) { std::printf("0087cd50_TEST FAIL\n"); return 1; } } while (0)

struct CParams { unsigned char pad[8]; bool autoTurn; };
struct CEntryAnim { CParams* params; void GetParameters(CParams** out) { *out = params; } };
struct CModes { int added; void AddMode(unsigned int m) { added = (int)m; Log(1); } };
struct CAction { CEntryAnim* anim; CThing* target;
    bool GetAutoTurn() { CParams* p; anim->GetParameters(&p); return p->autoTurn; }
    void PerformAction() { if (GetAutoTurn() && IS_THING_ALIVE(target)) { CModes* m; target->GetInterface(&m, 0x31); m->AddMode(0x1F); } } };
int main()
{
    CParams p; p.autoTurn = true; CEntryAnim e; e.params = &p;
    CThing t; CModes m; m.added = 0; t.Add(0x31, &m);
    CAction a; a.anim = &e; a.target = &t; a.PerformAction(); CHECK(m.added == 0x1F);
    m.added = 0; p.autoTurn = false; a.PerformAction(); CHECK(m.added == 0);
    p.autoTurn = true; t.flags91 = 1; a.PerformAction(); CHECK(m.added == 0);
    std::printf("0087cd50_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x008C4810 (see src/008c4810.cpp).
#include <cstdio>
#include <cstring>

// Interface table model shared by the GetInterface-based rows: presence bits + sorted {type, component} entries.
struct CEntry { int type; void* component; };
struct CThing {
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

#define CHECK(c) do { if (!(c)) { std::printf("008c4810_TEST FAIL\n"); return 1; } } while (0)

struct CDriver { CThing* excluded; void AddExcludedThing(CThing* t) { excluded = t; Log(2); } };
static CThing g_explosion;
static CThing* CreateThing(long index, int pos) { Log(1); return index == 7 && pos == 3 ? &g_explosion : 0; }
static long GetDefIndex(const char* name) { return std::strcmp(name, "THING_EXPLOSION_SCORPION_KING_PLUNGE") == 0 ? 7 : -1; }
struct CAction { CThing* creature; int position;
    void PerformAction()
    {
        long index = GetDefIndex("THING_EXPLOSION_SCORPION_KING_PLUNGE");
        CThing* explosion = CreateThing(index, position);
        if (explosion) {
            CDriver* d = 0;
            if (explosion->driverFlags28 & 2) d = (CDriver*)explosion->cached5c;
            d->AddExcludedThing(creature);
        }
    } };
int main()
{
    CDriver d; d.excluded = 0; g_explosion.driverFlags28 = 2; g_explosion.cached5c = &d;
    CThing scorpion; CAction a; a.creature = &scorpion; a.position = 3; a.PerformAction();
    { int want[] = { 1, 2 }; CHECK(Expect(want, 2)); }
    CHECK(d.excluded == &scorpion);
    a.position = 4; Reset(); a.PerformAction(); { int want[] = { 1 }; CHECK(Expect(want, 1)); }
    std::printf("008c4810_TEST PASS\n");
    return 0;
}

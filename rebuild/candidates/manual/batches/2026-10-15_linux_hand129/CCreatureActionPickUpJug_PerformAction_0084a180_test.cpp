// Behaviour test for 0x0084A180 (see src/0084a180.cpp).
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

#define CHECK(c) do { if (!(c)) { std::printf("0084a180_TEST FAIL\n"); return 1; } } while (0)

struct CJug { unsigned char pad[16]; bool putDown; };
struct CAction { CThing* thing; void BasePerformAction() { Log(1); }
    void PerformAction()
    {
        BasePerformAction();
        if (IS_THING_ALIVE(thing)) { CJug* jug; thing->GetInterface(&jug, 0xBB); jug->putDown = false; }
    } };
int main()
{
    CThing t; CJug j; j.putDown = true; t.Add(0xBB, &j);
    CHECK((t.presentBits[5] & 0x8000000) != 0);
    CAction a; a.thing = &t; a.PerformAction(); CHECK(!j.putDown);
    j.putDown = true; t.flags91 = 1; a.PerformAction(); CHECK(j.putDown);
    a.thing = 0; Reset(); a.PerformAction();
    { int want[] = { 1 }; CHECK(Expect(want, 1)); }
    std::printf("0084a180_TEST PASS\n");
    return 0;
}

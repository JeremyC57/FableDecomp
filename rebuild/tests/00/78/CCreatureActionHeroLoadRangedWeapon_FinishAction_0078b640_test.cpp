// Behaviour test for 0x0078B640 (see src/0078b640.cpp).
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

#define CHECK(c) do { if (!(c)) { std::printf("0078b640_TEST FAIL\n"); return 1; } } while (0)

struct CScaled { void* keys[4]; int n; int recalcs; CScaled() : n(0), recalcs(0) {}
    void Erase(void* k) { for (int i = 0; i < n; ++i) if (keys[i] == k) { keys[i] = keys[--n]; return; } }
    void Recalculate() { ++recalcs; Log(3); } };
struct CAction {
    CThing* creature; bool Finished; bool loaded;
    void SetWeaponLoaded() { loaded = true; Log(1); }
    void BaseFinishAction() { Log(2); }
    void FinishAction()
    {
        if (Finished) SetWeaponLoaded();
        BaseFinishAction();
        CScaled* scaled;
        if (creature->GetInterface(&scaled, 0x8C)) { scaled->Erase(this); scaled->Recalculate(); }
    }
};
int main()
{
    CThing t; CScaled s; t.Add(0x8C, &s);
    CAction a; a.creature = &t; a.Finished = true; a.loaded = false;
    s.keys[s.n++] = (void*)0x10; s.keys[s.n++] = &a;
    a.FinishAction();
    { int want[] = { 1, 2, 3 }; CHECK(Expect(want, 3)); }
    CHECK(a.loaded && s.n == 1 && s.keys[0] == (void*)0x10 && s.recalcs == 1);
    CThing bare; CAction b; b.creature = &bare; b.Finished = false; b.loaded = false; Reset();
    b.FinishAction();
    { int want[] = { 2 }; CHECK(Expect(want, 1)); }
    CHECK(!b.loaded);
    std::printf("0078b640_TEST PASS\n");
    return 0;
}
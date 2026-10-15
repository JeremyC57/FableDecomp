// Behaviour test for 0x008E6490 (see src/008e6490.cpp).
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

#define CHECK(c) do { if (!(c)) { std::printf("008e6490_TEST FAIL\n"); return 1; } } while (0)

struct CBerserk { float time; void InitAbility() { Log(2); } void SetAnimationTime(float t) { time = t; Log(4); } };
struct CAction { CThing* creature; void BaseInitialise() { Log(1); } float GetAnimationTime() { Log(3); return 2.5f; }
    void Initialise()
    {
        BaseInitialise();
        CBerserk* b;
        if (creature->GetInterface(&b, 0x84)) { b->InitAbility(); b->SetAnimationTime(GetAnimationTime()); }
    } };
int main()
{
    CThing t; CBerserk b; b.time = 0.0f; t.Add(0x84, &b);
    CHECK((t.presentBits[4] & 0x10) != 0);
    CAction a; a.creature = &t; a.Initialise();
    { int want[] = { 1, 2, 3, 4 }; CHECK(Expect(want, 4)); }
    CHECK(b.time == 2.5f);
    CThing none; a.creature = &none; Reset(); a.Initialise();
    { int want[] = { 1 }; CHECK(Expect(want, 1)); }
    std::printf("008e6490_TEST PASS\n");
    return 0;
}

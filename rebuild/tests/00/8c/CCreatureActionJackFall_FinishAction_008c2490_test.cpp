// Behaviour test for 0x008C2490 (see src/008c2490.cpp).
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

#define CHECK(c) do { if (!(c)) { std::printf("008c2490_TEST FAIL\n"); return 1; } } while (0)

struct CModes { unsigned int mask[4];
    bool HasMode(unsigned int m) { return ((mask[m >> 5] >> (m & 31)) & 1) != 0; }
    void RemoveMode(unsigned int m) { mask[m >> 5] &= ~(1u << (m & 31)); Log(1); } };
struct CCreature;
struct CGoUnderground { CCreature* owner; CGoUnderground(CCreature* c) : owner(c) { Log(3); } };
struct CCreature { CThing thing; CCreature* actionOwner; bool SetCurrentAction(CGoUnderground* a) { actionOwner = a->owner; Log(4); return true; } };
struct CAction { CCreature* creature; void BaseFinishAction() { Log(2); }
    void FinishAction()
    {
        CModes* modes; creature->thing.GetInterface(&modes, 0x31);
        if (modes->HasMode(0x33)) modes->RemoveMode(0x33);
        CCreature* c = creature;
        BaseFinishAction();
        CGoUnderground action(c);
        creature->SetCurrentAction(&action);
    } };
int main()
{
    CCreature jack; jack.actionOwner = 0; CModes m; std::memset(&m, 0, sizeof(m)); m.mask[1] = 1u << (0x33 - 32);
    jack.thing.Add(0x31, &m);
    CAction a; a.creature = &jack; a.FinishAction();
    { int want[] = { 1, 2, 3, 4 }; CHECK(Expect(want, 4)); }
    CHECK(!m.HasMode(0x33) && jack.actionOwner == &jack);
    Reset(); a.FinishAction(); { int want[] = { 2, 3, 4 }; CHECK(Expect(want, 3)); }
    std::printf("008c2490_TEST PASS\n");
    return 0;
}
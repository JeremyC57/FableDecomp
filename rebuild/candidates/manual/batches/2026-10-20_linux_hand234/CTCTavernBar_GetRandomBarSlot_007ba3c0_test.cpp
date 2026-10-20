// Behaviour test for 0x007BA3C0 CTCTavernBar::GetRandomBarSlot (copy of the source logic over spy callees).
#include <cstdio>
#include <cstring>
struct CTCBase { virtual ~CTCBase() {} };
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float m[6]; };
struct CCharString {
    char buf[64];
    CCharString() { buf[0] = 0; }
    CCharString(const char* t, long = -1) { std::strcpy(buf, t); }
    CCharString(const CCharString& o) { std::strcpy(buf, o.buf); }
    CCharString& operator+=(const char* t) { std::strcat(buf, t); return *this; }
    CCharString& operator+=(const CCharString& o) { std::strcat(buf, o.buf); return *this; }
};
static CCharString GFIntToCharString(long v) { CCharString s; std::sprintf(s.buf, "%ld", v); return s; }
static unsigned long GFROR13(unsigned long v) { v &= 0xffffffffUL; return ((v >> 13) | (v << 19)) & 0xffffffffUL; }
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = (seed * 0x24A1 + 0x24DF) & 0xffffffffUL;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
static char g_askedName[64]; static CThing* g_user = 0; static bool g_free = true;
struct CTCNamedUserActionPoints : CTCBase {
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet*, CCharString name, long* index, long, bool)
    { std::strcpy(g_askedName, name.buf); g_user = user; if (!g_free) return false; pos->X = 5.0f; *index = 42; return true; }
};
struct CJugPointer { void* a; void* b; };
static unsigned long g_seed = 12345;
struct CTCBar {
    CThing* ParentThing;
    CJugPointer* Jugs_start; CJugPointer* Jugs_finish;
    unsigned long* GetPRandomSeed() const { return &g_seed; }
    bool GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot);
};
bool CTCBar::GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot)
{
    CTCNamedUserActionPoints* points;
    ParentThing->GetInterface(&points, 0x2e);
    unsigned long count = (unsigned long)(Jugs_finish - Jugs_start);
    slot = WorldRandomRange(*GetPRandomSeed(), count);
    CCharString unused;
    CCharString pointName("ACTION_POINT_BARMAN", -1);
    pointName += "_0";
    pointName += GFIntToCharString(slot + 1);
    if (points->GetFreeNamedActionPoint(&creature, &position, &orientation, pointName, &actionPoint, 0, true))
        return true;
    return false;
}
static int fail() { std::printf("007ba3c0_TEST FAIL\n"); return 1; }
int main()
{
    CTCNamedUserActionPoints points;
    CInterfaceMap::SEntry e[1] = { { 0x2e, &points } };
    CThing bar; std::memset(bar.m_presentBits, 0, sizeof bar.m_presentBits);
    bar.m_presentBits[0x2e >> 5] = 1UL << (0x2e & 31); bar.m_interfaces.first = e; bar.m_interfaces.last = e + 1;
    CJugPointer jugs[3];
    CTCBar tb; tb.ParentThing = &bar; tb.Jugs_start = jugs; tb.Jugs_finish = jugs + 3;
    CThing barman; C3DVector pos = { 0, 0, 0 }; CRightHandedSet rh; long ap = -1, slot = -1;
    unsigned long expectSeed = GFROR13((12345UL * 0x24A1 + 0x24DF) & 0xffffffffUL);
    if (!tb.GetRandomBarSlot(barman, pos, rh, ap, slot)) return fail();
    char expect[64]; std::sprintf(expect, "ACTION_POINT_BARMAN_0%ld", (long)(expectSeed % 3) + 1);
    if (slot != (long)(expectSeed % 3) || std::strcmp(g_askedName, expect) != 0 || g_user != &barman || ap != 42 || pos.X != 5.0f || g_seed != expectSeed) return fail();
    g_free = false;
    if (tb.GetRandomBarSlot(barman, pos, rh, ap, slot)) return fail();
    tb.Jugs_finish = jugs;                                  // no jugs: slot 0
    g_free = true;
    if (!tb.GetRandomBarSlot(barman, pos, rh, ap, slot) || slot != 0 || std::strcmp(g_askedName, "ACTION_POINT_BARMAN_01") != 0) return fail();
    std::printf("007ba3c0_TEST PASS\n");
    return 0;
}

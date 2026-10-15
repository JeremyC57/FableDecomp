#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_has = 0; static long g_filterType = 0; static bool g_hasResult = false;
struct CTCBase { };
struct CMap { struct SEntry { long type; CTCBase* comp; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* t) { SEntry* p = first; while (p != last && p->type < *t) ++p; return p; } };
struct CThing { unsigned char pad[0x20]; unsigned long bits[9]; CMap map;
    template<class T> bool GetInterface(T** out, long type) { if (bits[type >> 5] & (1UL << (type & 31))) { long k = type; CMap* m = &map; CMap::SEntry* it = m->LowerBound(&k); if (it == m->last || type < it->type) it = m->last; *out = (T*)it->comp; return true; } return false; } };
struct CIsEventOfType { long type; CIsEventOfType(long t) : type(t) { } };
struct CEvents : CTCBase { bool HasEvent(CIsEventOfType& f) { ++g_has; g_filterType = f.type; return g_hasResult; } };
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CMode : CModeBase { bool FindHitEvent() { CEvents* ev; if (thing->GetInterface(&ev, 0x42)) { CIsEventOfType f(1); if (ev->HasEvent(f)) return true; } return false; } };
int main()
{
    CThing t; std::memset(&t, 0, sizeof t); CEvents ev; CMap::SEntry entries[2]; entries[0].type = 0x10; entries[0].comp = 0; entries[1].type = 0x42; entries[1].comp = &ev; t.map.first = entries; t.map.last = entries + 2;
    CMode m; m.thing = &t;
    if (m.FindHitEvent() || g_has != 0) { std::printf("008e79a0_TEST FAIL\n"); return 1; }
    t.bits[2] = 4; g_hasResult = true; if (!m.FindHitEvent() || g_has != 1 || g_filterType != 1) { std::printf("008e79a0_TEST FAIL\n"); return 1; }
    g_hasResult = false; if (m.FindHitEvent() || g_has != 2) { std::printf("008e79a0_TEST FAIL\n"); return 1; }
    std::printf("008e79a0_TEST PASS\n");
    return 0;
}
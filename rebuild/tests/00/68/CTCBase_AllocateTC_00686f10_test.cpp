// Behaviour test for 0x00686F10 CTCBase::AllocateTC(name, owner): the registered allocator for an exact name match, else null (standalone copy of the logic)
#include <cstdio>
#include <cstring>
struct Thing { int id; };
struct TC { Thing* owner; int kind; };
static TC g_made[2];
TC* AllocA(Thing& t) { g_made[0].owner = &t; g_made[0].kind = 1; return &g_made[0]; }
TC* AllocB(Thing& t) { g_made[1].owner = &t; g_made[1].kind = 2; return &g_made[1]; }
struct Entry { const char* name; TC* (*alloc)(Thing&); };
static Entry g_tcs[2] = { { "CTCHeroStats", AllocB }, { "CTCInventory", AllocA } };   // sorted
TC* AllocateTC(const char* name, Thing& owner)
{
    Entry* it = g_tcs; Entry* end = g_tcs + 2;
    while (it != end && std::strcmp(it->name, name) < 0) ++it;        // lower bound
    if (it == end || std::strcmp(name, it->name) < 0) return 0;
    return it->alloc(owner);
}

int main()
{
    Thing t; TC* a = AllocateTC("CTCInventory", t); if (!a || a->kind != 1 || a->owner != &t) { std::printf("00686f10_TEST FAIL\n"); return 1; }
    TC* b = AllocateTC("CTCHeroStats", t); if (!b || b->kind != 2) { std::printf("00686f10_TEST FAIL\n"); return 1; }
    if (AllocateTC("CTCAAA", t) || AllocateTC("CTCZZZ", t) || AllocateTC("CTCHeroStatz", t)) { std::printf("00686f10_TEST FAIL\n"); return 1; }
    std::printf("00686f10_TEST PASS\n");
    return 0;
}
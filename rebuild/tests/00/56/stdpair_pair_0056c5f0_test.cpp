// 0056c5f0 behaviour test for std::pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>::pair(const&, const&)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
static int g_boastCopies = 0;
struct CMenuEntry { int Id; };
struct CBoast {
    long ID, Cost, Reward;
    bool Evil;
    long GroupIndex;
    CBoast() : ID(0), Cost(0), Reward(0), Evil(false), GroupIndex(0) { }
    CBoast(const CBoast& o) : ID(o.ID), Cost(o.Cost), Reward(o.Reward), Evil(o.Evil), GroupIndex(o.GroupIndex) { ++g_boastCopies; }
};
template <class T1, class T2> struct Pair {
    T1 first;
    T2 second;
    Pair(const T1& a, const T2& b) : first(a), second(b) { }
};
static void run()
{
    CMenuEntry entry = { 1 };
    CBoast boast;
    boast.ID = 11; boast.Cost = 200; boast.Reward = 450; boast.Evil = true; boast.GroupIndex = 3;
    CMenuEntry* const key = &entry;
    Pair<CMenuEntry* const, CBoast> p(key, boast);
    CHECK(p.first == &entry);
    CHECK(p.second.ID == 11 && p.second.Cost == 200 && p.second.Reward == 450 && p.second.Evil && p.second.GroupIndex == 3);
    CHECK(g_boastCopies == 1);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0056c5f0_TEST FAIL\n");
        return 1;
    }
    std::printf("0056c5f0_TEST PASS\n");
    return 0;
}
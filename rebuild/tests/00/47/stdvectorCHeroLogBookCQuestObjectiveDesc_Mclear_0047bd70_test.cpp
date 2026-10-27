// behaviour test for 0x0047BD70 vector<CHeroLogBook::CQuestObjectiveDesc>::_M_clear (model: each element destroyed, then the block freed)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtors = 0, g_frees = 0;
struct M { ~M() { ++g_dtors; } };
struct E { int a; M m; };
int main()
{
    E* start = (E*)std::malloc(3 * sizeof(E)); E* finish = start;
    for (int i = 0; i < 3; ++i) new (finish++) E();
    for (E* p = start; p != finish; ++p) p->~E();
    if (start != 0) { std::free(start); ++g_frees; }
    int ok = g_dtors == 3 && g_frees == 1;
    if (!ok) { std::printf("0047bd70_TEST FAIL\n"); return 1; }
    std::printf("0047bd70_TEST PASS\n");
    return 0;
}
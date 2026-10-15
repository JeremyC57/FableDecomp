#include <cstdio>
struct SSortedPair { unsigned long Key, Value; };
struct CPairCompareLess { unsigned char _empty; };
static int g_sorts = 0;
static void SortPairs(SSortedPair* first, SSortedPair* last, CPairCompareLess)
{
    ++g_sorts;
    for (SSortedPair* i = first + 1; i < last; ++i)
        for (SSortedPair* j = i; j > first && j->Key < (j - 1)->Key; --j) { SSortedPair t = *j; *j = *(j - 1); *(j - 1) = t; }
}
struct CPairVectorMap {
    SSortedPair* _M_start; SSortedPair* _M_finish; SSortedPair* _M_end_of_storage; CPairCompareLess Compare; bool NeedsSort;
    void Sort() { if (NeedsSort) { SortPairs(_M_start, _M_finish, Compare); NeedsSort = false; } }
};
struct SProcessedBlock { int processed; };
struct CSortedMapOwner {
    SProcessedBlock m_block; CPairVectorMap m_map;
    void Process(SProcessedBlock* b) { ++b->processed; }
    void SortAndProcess() { m_map.Sort(); Process(&m_block); }
};
int main()
{
    SSortedPair p[2] = { { 5, 0 }, { 2, 1 } };
    CSortedMapOwner o; o.m_block.processed = 0; o.m_map._M_start = p; o.m_map._M_finish = p + 2; o.m_map.NeedsSort = true;
    o.SortAndProcess();
    if (p[0].Key != 2 || o.m_map.NeedsSort || g_sorts != 1 || o.m_block.processed != 1) { std::printf("00ae34d0_TEST FAIL\n"); return 1; }
    o.SortAndProcess();
    if (g_sorts != 1 || o.m_block.processed != 2) { std::printf("00ae34d0_TEST FAIL\n"); return 1; }
    std::printf("00ae34d0_TEST PASS\n");
    return 0;
}

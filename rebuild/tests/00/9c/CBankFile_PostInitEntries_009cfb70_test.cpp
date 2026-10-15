#include <cstdio>
struct SCRCPair { unsigned long Key, Value; };
struct CKeyPairCompareLess { unsigned char _empty; bool operator()(const SCRCPair& a, const SCRCPair& b) const { return a.Key < b.Key; } };
static int g_sorts = 0;
static void SortCRCPairs(SCRCPair* first, SCRCPair* last, CKeyPairCompareLess cmp)
{
    ++g_sorts;
    for (SCRCPair* i = first + 1; i < last; ++i)
        for (SCRCPair* j = i; j > first && cmp(*j, *(j - 1)); --j) { SCRCPair t = *j; *j = *(j - 1); *(j - 1) = t; }
}
struct CCRCVectorMap {
    SCRCPair* _M_start; SCRCPair* _M_finish; SCRCPair* _M_end_of_storage; CKeyPairCompareLess Compare; bool NeedsSort; int trims;
    void Sort() { if (NeedsSort) { SortCRCPairs(_M_start, _M_finish, Compare); NeedsSort = false; } }
    void DeallocateUnusedMemory() { ++trims; }
};
struct CBankFile {
    bool ReadOnly; CCRCVectorMap SymbolCRCIndices; int packs;
    void PackRuntimeData() { ++packs; }
    void PostInitEntries() { SymbolCRCIndices.Sort(); SymbolCRCIndices.DeallocateUnusedMemory(); if (ReadOnly) PackRuntimeData(); }
};
int main()
{
    SCRCPair p[3] = { { 30, 0 }, { 10, 1 }, { 20, 2 } };
    CBankFile b; b.ReadOnly = false; b.packs = 0;
    b.SymbolCRCIndices._M_start = p; b.SymbolCRCIndices._M_finish = p + 3; b.SymbolCRCIndices._M_end_of_storage = p + 3;
    b.SymbolCRCIndices.NeedsSort = true; b.SymbolCRCIndices.trims = 0;
    b.PostInitEntries();
    if (p[0].Key != 10 || p[2].Key != 30 || b.SymbolCRCIndices.NeedsSort || g_sorts != 1 || b.SymbolCRCIndices.trims != 1 || b.packs != 0) { std::printf("009cfb70_TEST FAIL\n"); return 1; }
    b.ReadOnly = true; b.PostInitEntries();
    if (g_sorts != 1 || b.SymbolCRCIndices.trims != 2 || b.packs != 1) { std::printf("009cfb70_TEST FAIL\n"); return 1; }
    std::printf("009cfb70_TEST PASS\n");
    return 0;
}
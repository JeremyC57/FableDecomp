#include <cstdio>
// Behaviour test for the SGI __pop_heap at 0x004E1D20 (calling conventions dropped for the host).
static int g_strings = 0;
class CCharString { public:
    long PStringData;
    CCharString() : PStringData(0) { ++g_strings; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_strings; }
    CCharString& operator=(const CCharString& o) { PStringData = o.PStringData; return *this; }
    ~CCharString() { --g_strings; }
};
struct CRegisteredTCInfo { long m_type; long m_flags; CCharString m_name; };
struct SRegisteredTCPair {
    CCharString first;
    CRegisteredTCInfo second;
    SRegisteredTCPair() {}
    SRegisteredTCPair(const SRegisteredTCPair& o) : first(o.first), second(o.second) {}
    SRegisteredTCPair& operator=(const SRegisteredTCPair& o) { first = o.first; second = o.second; return *this; }
};
struct CKeyPairCompareLess { CKeyPairCompareLess() {} };
static SRegisteredTCPair* g_adjFirst = 0; static long g_hole = -1, g_len = -1, g_valueKey = -1;
static void __adjust_heap(SRegisteredTCPair* first, long holeIndex, long len, SRegisteredTCPair value, CKeyPairCompareLess)
{
    g_adjFirst = first; g_hole = holeIndex; g_len = len; g_valueKey = value.first.PStringData;
}
static void __pop_heap(SRegisteredTCPair* first, SRegisteredTCPair* last, SRegisteredTCPair* result,
                       SRegisteredTCPair value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, last - first, value, comp);
}
int main()
{
    {
        SRegisteredTCPair heap[5];
        for (int i = 0; i < 5; ++i) { heap[i].first.PStringData = 100 + i; heap[i].second.m_type = i; }
        // pop_heap_aux shape: result = last - 1, value = copy of *(last - 1)
        __pop_heap(heap, heap + 4, heap + 4, heap[4], CKeyPairCompareLess(), (long*)0);
        if (heap[4].first.PStringData != 100 || heap[4].second.m_type != 0 || g_adjFirst != heap || g_hole != 0
            || g_len != 4 || g_valueKey != 104) { std::printf("004e1d20_TEST FAIL\n"); return 1; }
    }
    if (g_strings != 0) { std::printf("004e1d20_TEST FAIL\n"); return 1; }
    std::printf("004e1d20_TEST PASS\n");
    return 0;
}
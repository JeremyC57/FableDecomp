// Behaviour test for __partial_sort<pair<unsigned __int64, CPersistInfo>> @ 0x00CBBEB0
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_live = 0;
class CCharString {
public:
    int m_tag;
    CCharString() : m_tag(0) { ++g_live; }
    CCharString(const CCharString& other) : m_tag(other.m_tag) { ++g_live; }
    ~CCharString() { --g_live; }
    const CCharString& operator=(const CCharString& other) { m_tag = other.m_tag; return *this; }
};
struct CPersistInfo { u64 UID; CCharString PersistScript; };
struct SPersistPair { u64 first; CPersistInfo second; };
struct CKeyPairCompareLess { bool operator()(const SPersistPair& a, const SPersistPair& b) const { return a.first < b.first; } };
// SGI __adjust_heap / __push_heap (reference implementations of the out-of-line callees)
static void PushHeap(SPersistPair* first, long holeIndex, long topIndex, SPersistPair value, CKeyPairCompareLess comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) { *(first + holeIndex) = *(first + parent); holeIndex = parent; parent = (holeIndex - 1) / 2; }
    *(first + holeIndex) = value;
}
static void AdjustHeap(SPersistPair* first, long holeIndex, long len, SPersistPair value, CKeyPairCompareLess comp)
{
    long topIndex = holeIndex, secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1)))) secondChild--;
        *(first + holeIndex) = *(first + secondChild); holeIndex = secondChild; secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { *(first + holeIndex) = *(first + (secondChild - 1)); holeIndex = secondChild - 1; }
    PushHeap(first, holeIndex, topIndex, value, comp);
}
static void PopHeap(SPersistPair* first, SPersistPair* last, SPersistPair* result, SPersistPair value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    AdjustHeap(first, 0, long(last - first), value, comp);
}
static void Fill(SPersistPair* p, int n, const int* keys) { for (int i = 0; i < n; ++i) { p[i].first = (u64)keys[i] << 32; p[i].second.UID = keys[i]; p[i].second.PersistScript.m_tag = keys[i]; } }
static void MakeHeap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp, SPersistPair*, long*)
{
    long len = long(last - first); if (len < 2) return;
    for (long parent = (len - 2) / 2; ; --parent) { AdjustHeap(first, parent, len, SPersistPair(*(first + parent)), comp); if (parent == 0) return; }
}
static void SortHeap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, SPersistPair(*last), comp, (long*)0); }
}
static void PartialSort(SPersistPair* first, SPersistPair* middle, SPersistPair* last, SPersistPair*, CKeyPairCompareLess comp)
{
    MakeHeap(first, middle, comp, (SPersistPair*)0, (long*)0);
    for (SPersistPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, SPersistPair(*i), comp, (long*)0);
    SortHeap(first, middle, comp);
}
int main()
{
    {
        SPersistPair v[8];
        int keys[8] = { 50, 20, 70, 10, 60, 30, 80, 40 };
        Fill(v, 8, keys);
        PartialSort(v, v + 3, v + 8, (SPersistPair*)0, CKeyPairCompareLess());
        if (v[0].second.UID != 10 || v[1].second.UID != 20 || v[2].second.UID != 30 || v[2].second.PersistScript.m_tag != 30) { std::printf("00cbbeb0_TEST FAIL\n"); return 1; }
        int sum = 0; for (int i = 0; i < 8; ++i) sum += (int)v[i].second.UID;
        if (sum != 360) { std::printf("00cbbeb0_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("00cbbeb0_TEST FAIL\n"); return 1; }
    std::printf("00cbbeb0_TEST PASS\n");
    return 0;
}
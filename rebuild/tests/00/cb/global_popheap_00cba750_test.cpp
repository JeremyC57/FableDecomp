// Behaviour test for __pop_heap<pair<unsigned __int64, CPersistInfo>> @ 0x00CBA750
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
    CCharString& operator=(const CCharString& other) { m_tag = other.m_tag; return *this; }
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
int main()
{
    {
        SPersistPair heap[5];
        int keys[5] = { 9, 7, 8, 3, 5 };          // a valid max-heap
        Fill(heap, 5, keys);
        SPersistPair out;
        SPersistPair last = heap[4];
        PopHeap(heap, heap + 4, &out, last, CKeyPairCompareLess(), (long*)0);   // pop into a separate slot, heap shrinks to 4
        if (out.first != ((u64)9 << 32) || out.second.PersistScript.m_tag != 9 || heap[0].second.UID != 8) { std::printf("00cba750_TEST FAIL\n"); return 1; }
        for (int i = 1; i < 4; ++i) if (heap[(i - 1) / 2].first < heap[i].first) { std::printf("00cba750_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("00cba750_TEST FAIL\n"); return 1; }
    std::printf("00cba750_TEST PASS\n");
    return 0;
}
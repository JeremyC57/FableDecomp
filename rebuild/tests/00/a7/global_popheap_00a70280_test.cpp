#include <cstdio>
#include <string.h>
static int g_live = 0;
struct CCharString {
    const char* text;
    CCharString(const char* t) : text(t) { ++g_live; }
    CCharString(const CCharString& o) : text(o.text) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { text = o.text; return *this; }
};
struct C3DDummyObject { float Transform[12]; long BoneIndex; };
struct SDummyPair { CCharString first; C3DDummyObject second; SDummyPair(const char* n, long b) : first(n) { second.BoneIndex = b; } };
struct CCompareString { bool operator()(const SDummyPair& a, const SDummyPair& b) const { return strcmp(a.first.text, b.first.text) < 0; } };
static void __adjust_heap(SDummyPair* first, long holeIndex, long len, SDummyPair value, CCompareString comp)
{
    long topIndex = holeIndex, secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1])) secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild; secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { first[holeIndex] = first[secondChild - 1]; holeIndex = secondChild - 1; }
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) { first[holeIndex] = first[parent]; holeIndex = parent; parent = (holeIndex - 1) / 2; }
    first[holeIndex] = value;
}
static void __pop_heap(SDummyPair* first, SDummyPair* last, SDummyPair* result, SDummyPair value, CCompareString comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}
int main()
{
    // a max-heap by name: d, c, b, a
    SDummyPair h[4] = { SDummyPair("d", 4), SDummyPair("c", 3), SDummyPair("b", 2), SDummyPair("a", 1) };
    int live = g_live;
    // pop: move the root to the last slot (result = last - 1), sift the old last value
    __pop_heap(h, h + 3, h + 3, h[3], CCompareString(), 0);
    if (g_live != live || strcmp(h[3].first.text, "d") != 0 || h[3].second.BoneIndex != 4 || strcmp(h[0].first.text, "c") != 0)
    { std::printf("00a70280_TEST FAIL\n"); return 1; }
    __pop_heap(h, h + 2, h + 2, h[2], CCompareString(), 0);
    if (strcmp(h[2].first.text, "c") != 0 || strcmp(h[0].first.text, "b") != 0 || h[0].second.BoneIndex != 2)
    { std::printf("00a70280_TEST FAIL\n"); return 1; }
    std::printf("00a70280_TEST PASS\n");
    return 0;
}
// behaviour test for 0x004CFE10 _Push_heap<pair<float,CCharString>, CKeyPairCompareLess<float,CCharString>>
#include <cstdio>
#include <cstring>
struct CCharString {
    char text[16];
    CCharString() { text[0] = 0; }
    explicit CCharString(const char* s) { std::strcpy(text, s); }
};
struct TEntry { float first; CCharString second; };
struct TCompare { char m_tag; bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; } };
static void _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
int main()
{
    static const float scores[6] = { 0.5f, 2.0f, -1.0f, 3.5f, 1.0f, 0.25f };
    static const char* names[6] = { "farmer", "guard", "trader", "hero", "bandit", "child" };
    TEntry heap[6];
    TCompare c;
    for (int i = 0; i < 6; ++i) {
        TEntry v; v.first = scores[i]; v.second = CCharString(names[i]);
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("004cfe10_TEST FAIL\n"); return 1; }
    }
    if (heap[0].first != 3.5f || std::strcmp(heap[0].second.text, "hero") != 0) { std::printf("004cfe10_TEST FAIL\n"); return 1; }
    std::printf("004cfe10_TEST PASS\n");
    return 0;
}
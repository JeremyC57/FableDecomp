// behaviour test for 0x00819bc0 _Push_heap (SLongKeyedRecord40, `<` on m_key)
#include <cstdio>
#include <cstring>
struct SLongKeyedRecord40 {
    int m_key;
    int m_words[9];
};
struct SKeyLess {
    char m_tag;
    bool operator()(const SLongKeyedRecord40& a, const SLongKeyedRecord40& b) const { return a.m_key < b.m_key; }
};
static void _Push_heap(SLongKeyedRecord40* first, int hole, int top, SLongKeyedRecord40 value, SKeyLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static int check(SLongKeyedRecord40* h, int n)
{
    SKeyLess c;
    for (int i = 1; i < n; ++i)
        if (c(h[(i - 1) / 2], h[i])) return 0;   // heap property: no parent orders before its child
    return 1;
}
int main()
{
    static const int keys[8] = { 5, 3, 9, 1, 7, 2, 8, 4 };
    SLongKeyedRecord40 heap[8];
    std::memset(heap, 0, sizeof(heap));
    SKeyLess c;
    c.m_tag = 0;
    for (int i = 0; i < 8; ++i) {
        SLongKeyedRecord40 v;
        std::memset(&v, 0, sizeof(v));
        v.m_key = (long)keys[i];
        v.m_words[0] = i + 1;   // payload marker travels with the key
        _Push_heap(heap, i, 0, v, c);
        if (!check(heap, i + 1)) { std::printf("00819bc0_TEST FAIL\n"); return 1; }
    }
    // the top is the greatest key under `<`, the smallest under `>`
    if (heap[0].m_key != (long)(9)) { std::printf("00819bc0_TEST FAIL\n"); return 1; }
    if (heap[0].m_words[0] != 3) { std::printf("00819bc0_TEST FAIL\n"); return 1; }
    // a push with top == hole stores in place
    SLongKeyedRecord40 w;
    std::memset(&w, 0, sizeof(w));
    w.m_key = (long)100;
    _Push_heap(heap, 7, 7, w, c);
    if (heap[7].m_key != (long)100) { std::printf("00819bc0_TEST FAIL\n"); return 1; }
    std::printf("00819bc0_TEST PASS\n");
    return 0;
}
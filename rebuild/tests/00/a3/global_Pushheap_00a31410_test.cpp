// behaviour test for 0x00a31410 _Push_heap (SULongKeyedRecord56, `<` on m_key)
#include <cstdio>
#include <cstring>
struct SULongKeyedRecord56 {
    unsigned int m_key;
    int m_words[13];
};
struct SKeyLess {
    char m_tag;
    bool operator()(const SULongKeyedRecord56& a, const SULongKeyedRecord56& b) const { return a.m_key < b.m_key; }
};
static void _Push_heap(SULongKeyedRecord56* first, int hole, int top, SULongKeyedRecord56 value, SKeyLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static int check(SULongKeyedRecord56* h, int n)
{
    SKeyLess c;
    for (int i = 1; i < n; ++i)
        if (c(h[(i - 1) / 2], h[i])) return 0;   // heap property: no parent orders before its child
    return 1;
}
int main()
{
    static const int keys[8] = { 5, 3, 9, 1, 7, 2, 8, 4 };
    SULongKeyedRecord56 heap[8];
    std::memset(heap, 0, sizeof(heap));
    SKeyLess c;
    c.m_tag = 0;
    for (int i = 0; i < 8; ++i) {
        SULongKeyedRecord56 v;
        std::memset(&v, 0, sizeof(v));
        v.m_key = (unsigned long)keys[i];
        v.m_words[0] = i + 1;   // payload marker travels with the key
        _Push_heap(heap, i, 0, v, c);
        if (!check(heap, i + 1)) { std::printf("00a31410_TEST FAIL\n"); return 1; }
    }
    // the top is the greatest key under `<`, the smallest under `>`
    if (heap[0].m_key != (unsigned long)(9)) { std::printf("00a31410_TEST FAIL\n"); return 1; }
    if (heap[0].m_words[0] != 3) { std::printf("00a31410_TEST FAIL\n"); return 1; }
    // a push with top == hole stores in place
    SULongKeyedRecord56 w;
    std::memset(&w, 0, sizeof(w));
    w.m_key = (unsigned long)100;
    _Push_heap(heap, 7, 7, w, c);
    if (heap[7].m_key != (unsigned long)100) { std::printf("00a31410_TEST FAIL\n"); return 1; }
    std::printf("00a31410_TEST PASS\n");
    return 0;
}
// behaviour test for 0x00497060 _Push_heap<CWideString, CCaseInsensitiveStringCompare> (max-heap, case-insensitive)
#include <cstdio>
typedef unsigned short wch;
static int lower(int c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }
static int my_wcsicmp(const wch* a, const wch* b)
{
    for (;; ++a, ++b) {
        int x = lower(*a), y = lower(*b);
        if (x != y) return x - y;
        if (!x) return 0;
    }
}
class CWideString { public:
    wch text[16];
    CWideString() { text[0] = 0; }
    CWideString(const char* s) { int i = 0; for (; s[i]; ++i) text[i] = (wch)s[i]; text[i] = 0; }
    const wch* GetAsCharArray() const { return text; }
};
struct CCaseInsensitiveStringCompare {
    char m_tag;
    bool operator()(const CWideString& a, const CWideString& b) const { return my_wcsicmp(a.GetAsCharArray(), b.GetAsCharArray()) < 0; }
};
static void _Push_heap(CWideString* first, long hole, long top, CWideString value, CCaseInsensitiveStringCompare comp)
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
    static const char* words[6] = { "bob", "Alice", "dave", "Carol", "EVE", "ann" };
    CWideString heap[6];
    CCaseInsensitiveStringCompare c; c.m_tag = 0;
    for (int i = 0; i < 6; ++i) {
        _Push_heap(heap, i, 0, CWideString(words[i]), c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("00497060_TEST FAIL\n"); return 1; }
    }
    // case-insensitive maximum is "EVE"
    if (my_wcsicmp(heap[0].GetAsCharArray(), CWideString("eve").GetAsCharArray()) != 0) { std::printf("00497060_TEST FAIL\n"); return 1; }
    std::printf("00497060_TEST PASS\n");
    return 0;
}
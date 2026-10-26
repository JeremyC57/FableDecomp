#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    char m_text[12];
    CCharString(const char* s) { std::strcpy(m_text, s); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
    bool operator<(const CCharString& o) const { return std::strcmp(m_text, o.m_text) < 0; }
};
struct SEntry {
    CCharString first; long value;
    SEntry(const char* k, long v) : first(k), value(v) { }
    SEntry(const SEntry& p) : first(p.first), value(p.value) { }
};
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static void PushHeap(SEntry* first, int hole, int top, SEntry val, CLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(SEntry* first, int hole, int len, SEntry val, CLess comp)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val, comp);
}
static int g_adjusts = 0;
static void MakeHeap(SEntry* first, SEntry* last, CLess comp, SEntry*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    while (true) {
        ++g_adjusts;
        AdjustHeap(first, parent, len, SEntry(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
int main()
{
    bool ok = true;
    {
        const char* keys[7] = { "delta", "alpha", "golf", "bravo", "foxtrot", "echo", "charlie" };
        SEntry a[7] = { SEntry(keys[0], 0), SEntry(keys[1], 1), SEntry(keys[2], 2), SEntry(keys[3], 3), SEntry(keys[4], 4), SEntry(keys[5], 5), SEntry(keys[6], 6) };
        MakeHeap(a, a + 7, CLess(), 0, 0);
        for (int i = 1; i < 7; ++i) if (a[(i - 1) / 2].first < a[i].first) ok = false;
        if (std::strcmp(a[0].first.m_text, "golf") || a[0].value != 2 || g_adjusts != 3) ok = false;
        g_adjusts = 0;
        MakeHeap(a, a + 1, CLess(), 0, 0);                   // one element: nothing to do
        if (g_adjusts != 0) ok = false;
        if (g_live != 7) ok = false;
    }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("004e1d68_TEST FAIL\n"); return 1; }
    std::printf("004e1d68_TEST PASS\n");
    return 0;
}
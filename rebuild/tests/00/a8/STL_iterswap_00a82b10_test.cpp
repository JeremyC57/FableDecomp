// Behaviour test for 0x00A82B10 _STL::iter_swap over (unsigned __int64 ID, list) entries: the two entries exchange keys
// and lists (deep copies through a temporary that is destroyed again); swapping an entry with itself keeps it.
#include <cstdio>
static int g_lists = 0;
struct SList {                                   // stands in for list<C2DLineF>: a counted, deep-copied payload
    int* items; int count;
    SList() : items(0), count(0) { ++g_lists; }
    SList(const SList& o) : items(0), count(0) { ++g_lists; Assign(o); }
    ~SList() { delete[] items; --g_lists; }
    SList& operator=(const SList& o) { if (this != &o) Assign(o); return *this; }
    void Assign(const SList& o) { delete[] items; count = o.count; items = count ? new int[count] : 0; for (int i = 0; i < count; ++i) items[i] = o.items[i]; }
    void Set(int a, int n) { delete[] items; count = n; items = new int[n]; for (int i = 0; i < n; ++i) items[i] = a + i; }
};
struct SEntry {
    unsigned long long first; SList second;
    SEntry() : first(0) { }
    SEntry(const SEntry& o) : first(o.first), second(o.second) { }
};
static bool Less(const SEntry& a, const SEntry& b) { return a.first < b.first; }
static void UnguardedLinearInsert(SEntry* last, SEntry val)
{
    SEntry* next = last;
    --next;
    while (Less(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static bool PayloadOk(const SEntry& e) { return e.second.count == 2 && e.second.items[0] == (int)(e.first & 0xFFFF) && e.second.items[1] == (int)(e.first & 0xFFFF) + 1; }
// the function under test (same shape as the source)
static void IterSwap(SEntry* a, SEntry* b)
{
    SEntry tmp = *a;
    *a = *b;
    *b = tmp;
}
int main()
{
    {
        SEntry a, b;
        a.first = 0x100000005ULL; a.second.Set(5, 2);
        b.first = 0x900000009ULL; b.second.Set(9, 2);
        IterSwap(&a, &b);
        if (a.first != 0x900000009ULL || b.first != 0x100000005ULL || !PayloadOk(a) || !PayloadOk(b) || a.second.items == b.second.items) { std::printf("00a82b10_TEST FAIL\n"); return 1; }
        IterSwap(&a, &a);
        if (a.first != 0x900000009ULL || !PayloadOk(a)) { std::printf("00a82b10_TEST FAIL\n"); return 1; }
        if (g_lists != 2) { std::printf("00a82b10_TEST FAIL\n"); return 1; }
    }
    if (g_lists != 0) { std::printf("00a82b10_TEST FAIL\n"); return 1; }
    std::printf("00a82b10_TEST PASS\n");
    return 0;
}
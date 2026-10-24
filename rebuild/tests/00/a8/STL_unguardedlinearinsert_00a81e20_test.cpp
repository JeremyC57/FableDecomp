// Behaviour test for 0x00A81E20 _STL::__unguarded_linear_insert over (unsigned __int64 ID, list) entries: the value is
// inserted into the sorted prefix ending at last (larger entries shift up, lists move with their keys through the
// list's operator=), and the by-value copy is destroyed (no list leaks).
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
int main()
{
    {
        unsigned long long keys[5] = { 0x100000001ULL, 0x100000003ULL, 0x200000000ULL, 0x200000007ULL, 0x100000002ULL };
        SEntry v[5];
        for (int i = 0; i < 5; ++i) { v[i].first = keys[i]; v[i].second.Set((int)(keys[i] & 0xFFFF), 2); }
        UnguardedLinearInsert(v + 4, v[4]);
        unsigned long long expect[5] = { 0x100000001ULL, 0x100000002ULL, 0x100000003ULL, 0x200000000ULL, 0x200000007ULL };
        for (int i = 0; i < 5; ++i)
            if (v[i].first != expect[i] || !PayloadOk(v[i])) { std::printf("00a81e20_TEST FAIL\n"); return 1; }
        UnguardedLinearInsert(v + 4, v[4]);             // already in place: unchanged
        if (v[4].first != 0x200000007ULL || !PayloadOk(v[4])) { std::printf("00a81e20_TEST FAIL\n"); return 1; }
    }
    if (g_lists != 0) { std::printf("00a81e20_TEST FAIL\n"); return 1; }
    std::printf("00a81e20_TEST PASS\n");
    return 0;
}
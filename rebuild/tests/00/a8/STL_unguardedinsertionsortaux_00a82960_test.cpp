// Behaviour test for 0x00A82960 std::__unguarded_insertion_sort_aux over (unsigned __int64 ID, list) entries: with a
// smaller sentinel entry before first, [first, last) ends sorted on the 64-bit key, each list still paired with its
// key, and every by-value copy is destroyed.
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
static void UnguardedInsertionSortAux(SEntry* first, SEntry* last)
{
    for (SEntry* i = first; i != last; ++i)
        UnguardedLinearInsert(i, SEntry(*i));
}
int main()
{
    {
        unsigned long long keys[7] = { 0ULL, 0x300000000ULL, 5ULL, 0x100000000ULL, 0xFFFFFFFFULL, 5ULL, 0x100000004ULL };
        SEntry v[7];
        for (int i = 0; i < 7; ++i) { v[i].first = keys[i]; v[i].second.Set((int)(keys[i] & 0xFFFF), 2); }
        UnguardedInsertionSortAux(v + 1, v + 7);       // v[0] = 0 is the unguarded sentinel
        unsigned long long expect[7] = { 0ULL, 5ULL, 5ULL, 0xFFFFFFFFULL, 0x100000000ULL, 0x100000004ULL, 0x300000000ULL };
        for (int i = 0; i < 7; ++i)
            if (v[i].first != expect[i] || !PayloadOk(v[i])) { std::printf("00a82960_TEST FAIL\n"); return 1; }
    }
    if (g_lists != 0) { std::printf("00a82960_TEST FAIL\n"); return 1; }
    std::printf("00a82960_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00A7F060 _STL::__unguarded_insertion_sort_aux over (unsigned __int64 key, value) entries: with a
// sorted prefix that holds the minimum (the unguarded sentinel), inserting [first, last) leaves the whole array sorted.
#include <cstdio>
struct SEntry { unsigned long long first; void* second; };
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static void UnguardedLinearInsert(SEntry* last, SEntry val, CLess comp)
{
    SEntry* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void UnguardedInsertionSortAux(SEntry* first, SEntry* last, SEntry*, CLess comp)
{
    for (SEntry* i = first; i != last; ++i)
        UnguardedLinearInsert(i, SEntry(*i), comp);
}
int main()
{
    unsigned long long keys[8] = { 0ULL, 5ULL, 9ULL, 0x300000001ULL, 7ULL, 0x100000000ULL, 1ULL, 9ULL };
    SEntry v[8];
    for (int i = 0; i < 8; ++i) { v[i].first = keys[i]; v[i].second = (void*)0; }
    UnguardedInsertionSortAux(v + 3, v + 8, (SEntry*)0, CLess());
    for (int i = 1; i < 8; ++i)
        if (v[i].first < v[i - 1].first) { std::printf("00a7f060_TEST FAIL\n"); return 1; }
    if (v[7].first != 0x300000001ULL || v[1].first != 1ULL) { std::printf("00a7f060_TEST FAIL\n"); return 1; }
    std::printf("00a7f060_TEST PASS\n");
    return 0;
}
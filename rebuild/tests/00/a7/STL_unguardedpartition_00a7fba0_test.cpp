// Behaviour test for 0x00A7FBA0 _STL::__unguarded_partition over (unsigned __int64 key, value) entries: every entry before
// the returned cut has a key <= pivot, every entry from the cut on has a key >= pivot; the multiset of keys is kept.
#include <cstdio>
struct SEntry { unsigned long long first; bool second; };
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static SEntry* UnguardedPartition(SEntry* first, SEntry* last, SEntry pivot, CLess comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        SEntry tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}
int main()
{
    unsigned long long keys[9] = { 8ULL, 0x100000000ULL, 3ULL, 5ULL, 0x100000002ULL, 1ULL, 5ULL, 9ULL, 2ULL };
    SEntry v[9];
    unsigned long long sum = 0;
    for (int i = 0; i < 9; ++i) { v[i].first = keys[i]; v[i].second = (i & 1) != 0; sum += keys[i]; }
    SEntry pivot = v[3];                     // key 5 (a median-of-three style pivot inside the range)
    SEntry* cut = UnguardedPartition(v, v + 9, pivot, CLess());
    if (cut <= v || cut >= v + 9) { std::printf("00a7fba0_TEST FAIL\n"); return 1; }
    for (SEntry* p = v; p < cut; ++p) if (p->first > 5ULL) { std::printf("00a7fba0_TEST FAIL\n"); return 1; }
    for (SEntry* p = cut; p < v + 9; ++p) if (p->first < 5ULL) { std::printf("00a7fba0_TEST FAIL\n"); return 1; }
    unsigned long long sum2 = 0;
    for (int i = 0; i < 9; ++i) sum2 += v[i].first;
    if (sum2 != sum) { std::printf("00a7fba0_TEST FAIL\n"); return 1; }
    std::printf("00a7fba0_TEST PASS\n");
    return 0;
}
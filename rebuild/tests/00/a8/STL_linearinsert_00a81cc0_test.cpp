// Behaviour test for 0x00A81CC0 _STL::__linear_insert over (unsigned __int64 key, value) entries: inserting the entry at
// last into the sorted [first, last) keeps the range sorted on the 64-bit key and carries the value along; a new
// minimum shifts the whole prefix up, anything else is an unguarded backward insert.
#include <cstdio>
struct SEntry { unsigned long long first; void* second; };
static bool Less(const SEntry& a, const SEntry& b) { return a.first < b.first; }
static void UnguardedLinearInsert(SEntry* last, SEntry val)
{
    SEntry* next = last - 1;
    while (Less(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void CopyBackward(SEntry* first, SEntry* last, SEntry* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
}
// the function under test (same shape as the source)
static void LinearInsert(SEntry* first, SEntry* last, SEntry val)
{
    if (Less(val, *first)) { CopyBackward(first, last, last + 1); *first = val; }
    else UnguardedLinearInsert(last, val);
}
int main()
{
    unsigned long long keys[8] = { 0x500000000ULL, 3ULL, 0x100000000ULL, 1ULL, 0x100000000ULL, 0ULL, 0xFFFFFFFFULL, 2ULL };
    SEntry v[8];
    for (int i = 0; i < 8; ++i) { v[i].first = keys[i]; v[i].second = (void*)(keys + i); }
    for (int i = 1; i < 8; ++i) LinearInsert(v, v + i, v[i]);
    unsigned long long expect[8] = { 0ULL, 1ULL, 2ULL, 3ULL, 0xFFFFFFFFULL, 0x100000000ULL, 0x100000000ULL, 0x500000000ULL };
    for (int i = 0; i < 8; ++i) {
        if (v[i].first != expect[i]) { std::printf("00a81cc0_TEST FAIL\n"); return 1; }
        int j = 0; while (keys[j] != v[i].first) ++j;
        SEntry w; w.first = keys[j]; w.second = (void*)(keys + j);
        if (v[i].first != 0x100000000ULL && v[i].second != w.second) { std::printf("00a81cc0_TEST FAIL\n"); return 1; }
    }
    std::printf("00a81cc0_TEST PASS\n");
    return 0;
}
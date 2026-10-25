// Behaviour test for 0x00A7D570 std::__unguarded_linear_insert over (unsigned __int64 key, value) entries: larger keys
// shift up one slot (64-bit compare: high word first), the value lands in the hole; equal keys stop the shift.
#include <cstdio>
struct SEntry { unsigned long long first; bool second; };
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static void UnguardedLinearInsert(SEntry* last, SEntry val, CLess comp)
{
    SEntry* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
int main()
{
    SEntry v[5];
    unsigned long long keys[5] = { 1ULL, 0x100000000ULL, 0x100000005ULL, 0x200000000ULL, 0x100000003ULL };
    for (int i = 0; i < 5; ++i) { v[i].first = keys[i]; v[i].second = true; }
    UnguardedLinearInsert(v + 4, v[4], CLess());
    if (v[0].first != 1ULL || v[1].first != 0x100000000ULL || v[2].first != 0x100000003ULL || v[3].first != 0x100000005ULL || v[4].first != 0x200000000ULL) {
        std::printf("00a7d570_TEST FAIL\n"); return 1;
    }
    SEntry eq = v[2];
    v[3] = eq;            // insert an equal key at the end of a run: nothing moves
    SEntry w[2]; w[0] = eq; w[1] = eq;
    UnguardedLinearInsert(w + 1, w[1], CLess());
    if (w[0].first != eq.first || w[1].first != eq.first) { std::printf("00a7d570_TEST FAIL\n"); return 1; }
    std::printf("00a7d570_TEST PASS\n");
    return 0;
}
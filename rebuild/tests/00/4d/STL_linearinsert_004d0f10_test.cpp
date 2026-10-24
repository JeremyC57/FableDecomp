#include <cstdio>
struct SEntry {
    float first; long second;
    SEntry(float f, long s) : first(f), second(s) { }
    SEntry(const SEntry& p) : first(p.first), second(p.second) { }
};
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static SEntry* CopyBackward(SEntry* first, SEntry* last, SEntry* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void UnguardedLinearInsert(SEntry* last, SEntry val, CLess comp)
{
    SEntry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(SEntry* first, SEntry* last, SEntry val, CLess comp)
{
    if (comp(val, *first)) { CopyBackward(first, last, last + 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}
int main()
{
    bool ok = true;
    SEntry a[5] = { SEntry(2.0f, 20), SEntry(4.0f, 40), SEntry(6.0f, 60), SEntry(1.0f, 10), SEntry(0.0f, 0) };
    LinearInsert(a, a + 3, SEntry(a[3]), CLess());          // new minimum: shift all three, insert first
    if (a[0].second != 10 || a[1].second != 20 || a[2].second != 40 || a[3].second != 60) ok = false;
    a[4] = SEntry(5.0f, 50);
    LinearInsert(a, a + 4, SEntry(a[4]), CLess());          // middle: unguarded path
    if (a[3].second != 50 || a[4].second != 60 || a[2].second != 40 || a[0].first != 1.0f) ok = false;
    SEntry b[2] = { SEntry(3.0f, 3), SEntry(3.0f, 4) };
    LinearInsert(b, b + 1, SEntry(b[1]), CLess());          // equal key: stays after
    if (b[0].second != 3 || b[1].second != 4) ok = false;
    if (!ok) { std::printf("004d0f10_TEST FAIL\n"); return 1; }
    std::printf("004d0f10_TEST PASS\n");
    return 0;
}
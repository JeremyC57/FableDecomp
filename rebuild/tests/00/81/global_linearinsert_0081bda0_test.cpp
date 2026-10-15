// Behaviour test for 0x0081bda0 SGI __linear_insert: a value below the first element shifts [first, last) up and lands first;
// otherwise the unguarded insert sinks it from last into place.
#include <cstdio>
typedef long TKey;
struct SRecord { TKey first; unsigned int second[5]; };
struct SKeyLess { bool operator()(const SRecord& a, const SRecord& b) const { return a.first < b.first; } };
static SRecord* __copy_backward(SRecord* first, SRecord* last, SRecord* result)
{ for (long n = last - first; n > 0; --n) *--result = *--last; return result; }
static void __unguarded_linear_insert(SRecord* last, SRecord value, SKeyLess comp)
{ SRecord* next = last; --next; while (comp(value, *next)) { *last = *next; last = next; --next; } *last = value; }
static void __linear_insert(SRecord* first, SRecord* last, SRecord value, SKeyLess comp)
{
    if (comp(value, *first)) { __copy_backward(first, last, last + 1); *first = value; }
    else __unguarded_linear_insert(last, value, comp);
}
static SRecord R(TKey k) { SRecord r; r.first = k; for (int i = 0; i < 5; ++i) r.second[i] = (unsigned int)k * 10 + i; return r; }
int main()
{
    SRecord a[4] = { R(3), R(5), R(8), R(1) };
    __linear_insert(a, a + 3, a[3], SKeyLess());
    if (a[0].first != 1 || a[1].first != 3 || a[2].first != 5 || a[3].first != 8 || a[0].second[1] != 11) { std::printf("0081bda0_TEST FAIL\n"); return 1; }
    SRecord b[4] = { R(3), R(5), R(8), R(6) };
    __linear_insert(b, b + 3, b[3], SKeyLess());
    if (b[0].first != 3 || b[1].first != 5 || b[2].first != 6 || b[3].first != 8 || b[2].second[0] != 60) { std::printf("0081bda0_TEST FAIL\n"); return 1; }
    std::printf("0081bda0_TEST PASS\n");
    return 0;
}
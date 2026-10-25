// Behaviour test for 004711f5: std::__final_insertion_sort over pair<unsigned long,CDefString> -- short and long (> 16, block-presorted) ranges end sorted
#include <cstdio>
class CDefString { public: long TablePos; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair() { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}
template <class Key, class Value> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned long, CDefString> Entry;
typedef CKeyPairCompareLess<unsigned long, CDefString> Less;
static void unguarded_linear_insert(Entry* last, Entry val, Less comp)
{
    Entry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void unguarded_insertion_sort_aux(Entry* first, Entry* last, Entry*, Less comp)
{
    for (Entry* i = first; i != last; ++i) unguarded_linear_insert(i, Entry(*i), comp);
}
static void insertion_sort(Entry* first, Entry* last, Less comp)
{
    if (first == last) return;
    for (Entry* i = first + 1; i != last; ++i) {
        Entry val(*i);
        if (comp(val, *first)) { for (Entry* p = i; p != first; --p) *p = *(p - 1); *first = val; }
        else unguarded_linear_insert(i, val, comp);
    }
}
static Entry* copy_backward_ptrs(Entry* first, Entry* last, Entry* result)
{
    for (int n = int(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void final_insertion_sort(Entry* first, Entry* last, Less comp)
{
    if (last - first > 16) { insertion_sort(first, first + 16, comp); unguarded_insertion_sort_aux(first + 16, last, (Entry*)0, comp); }
    else insertion_sort(first, last, comp);
}
static void fill(Entry* e, int n, unsigned long seed)
{
    for (int i = 0; i < n; ++i) { e[i].first = (unsigned long)((i * 7 + seed) % n); e[i].second.TablePos = 1000 + (long)e[i].first; }
}
static int sorted(const Entry* e, int n)
{
    int ok = 1;
    for (int i = 0; i < n; ++i) ok &= e[i].first == (unsigned long)i && e[i].second.TablePos == 1000 + i;
    return ok;
}

int main()
{
    Less comp;
    Entry a[9]; fill(a, 9, 4); final_insertion_sort(a, a + 9, comp);
    int ok = sorted(a, 9);
    // > 16 entries: as after introsort, every entry is already within its 16-block (first block holds the 16 smallest)
    Entry b[40];
    for (int i = 0; i < 40; ++i) { int blk = i / 16, off = i % 16; int len = (blk == 2) ? 8 : 16; int k = blk * 16 + (len - 1 - off); b[i].first = (unsigned long)k; b[i].second.TablePos = 1000 + k; }
    final_insertion_sort(b, b + 40, comp);
    ok &= sorted(b, 40);
    std::printf(ok ? "004711f5_TEST PASS\n" : "004711f5_TEST FAIL\n");
    return ok ? 0 : 1;
}

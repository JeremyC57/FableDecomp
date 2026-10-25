// Behaviour test for 0047130e: std::__copy_backward_ptrs over pair<unsigned long,CDefString> -- overlapping backward shift by two, returns the new start
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
    Entry e[6]; fill(e, 6, 0);
    unsigned long k[6]; long tp[6];
    for (int i = 0; i < 6; ++i) { k[i] = e[i].first; tp[i] = e[i].second.TablePos; }
    Entry* r = copy_backward_ptrs(e + 1, e + 4, e + 6);      // old e[1..3] -> e[3..5]
    int ok = r == e + 3;
    for (int i = 0; i < 3; ++i) ok &= e[3 + i].first == k[1 + i] && e[3 + i].second.TablePos == tp[1 + i];
    ok &= e[0].first == k[0] && e[1].first == k[1] && e[2].first == k[2];
    Entry* q = copy_backward_ptrs(e, e, e + 3);
    ok &= q == e + 3;
    std::printf(ok ? "0047130e_TEST PASS\n" : "0047130e_TEST FAIL\n");
    return ok ? 0 : 1;
}

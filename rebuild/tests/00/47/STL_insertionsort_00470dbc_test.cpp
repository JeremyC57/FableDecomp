// Behaviour test for 00470dbc: std::__insertion_sort over pair<unsigned long,CSoundPair> -- the sound entries end in key order with their sound pairs; empty and one-entry ranges are left alone
#include <cstdio>
class CSoundPair { public: long FirstSound; long LastSound; };
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
typedef std::pair<unsigned long, CSoundPair> Entry;
typedef CKeyPairCompareLess<unsigned long, CSoundPair> Less;
// __linear_insert 0x00470E08 (STLport): front shift when below the first entry, else the unguarded walk
static void linear_insert(Entry* first, Entry* last, Entry val, Less comp)
{
    if (comp(val, *first)) { for (Entry* p = last; p != first; --p) *p = *(p - 1); *first = val; }
    else { Entry* next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val; }
}
// the row
static void insertion_sort(Entry* first, Entry* last, Less comp)
{
    if (first == last) return;
    for (Entry* i = first + 1; i != last; ++i) linear_insert(first, i, *i, comp);
}
int main()
{
    static const unsigned long keys[10] = { 9, 4, 0x80000001UL, 7, 1, 8, 0, 3, 2, 5 };
    Entry e[10];
    for (int i = 0; i < 10; ++i) { e[i].first = keys[i]; e[i].second.FirstSound = (long)(keys[i] & 0xFF) * 2; e[i].second.LastSound = (long)(keys[i] & 0xFF) * 2 + 1; }
    Less comp;
    insertion_sort(e, e, comp);
    insertion_sort(e, e + 1, comp);
    int ok = e[0].first == 9;
    insertion_sort(e, e + 10, comp);
    for (int i = 0; i < 10; ++i) {
        ok &= e[i].second.FirstSound == (long)(e[i].first & 0xFF) * 2 && e[i].second.LastSound == (long)(e[i].first & 0xFF) * 2 + 1;
        if (i) ok &= e[i - 1].first < e[i].first;
    }
    ok &= e[9].first == 0x80000001UL;                      // the key compares unsigned
    std::printf(ok ? "00470dbc_TEST PASS\n" : "00470dbc_TEST FAIL\n");
    return ok ? 0 : 1;
}

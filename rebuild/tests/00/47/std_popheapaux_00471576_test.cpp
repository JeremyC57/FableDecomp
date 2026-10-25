// Behaviour test for 00471576: std::__pop_heap_aux over pair<unsigned long,CDefString> -- heap-sort seven criteria entries, every def string stays with its key
#include <cstdio>
class CSoundPair { public: long FirstSound; long LastSound; };
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
template <class T, class C> void push_impl(T* first, int holeIndex, int topIndex, T value, C comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) { first[holeIndex] = first[parent]; holeIndex = parent; parent = (holeIndex - 1) / 2; }
    first[holeIndex] = value;
}
template <class T, class C> void adjust_impl(T* first, int holeIndex, int len, T value, C comp)
{
    int topIndex = holeIndex, secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1])) secondChild--;
        first[holeIndex] = first[secondChild]; holeIndex = secondChild; secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { first[holeIndex] = first[secondChild - 1]; holeIndex = secondChild - 1; }
    push_impl(first, holeIndex, topIndex, value, comp);
}
// the row: __pop_heap_aux(first, last, T*, comp) = __pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp)
template <class T, class C> void pop_heap_aux(T* first, T* last, T*, C comp)
{
    T val(*(last - 1));
    *(last - 1) = *first;
    adjust_impl(first, 0, int((last - 1) - first), val, comp);
}
template <class T, class C> void make_heap_impl(T* first, T* last, C comp)
{
    int len = int(last - first); if (len < 2) return;
    for (int parent = (len - 2) / 2; ; --parent) { adjust_impl(first, parent, len, T(first[parent]), comp); if (parent == 0) return; }
}

typedef std::pair<unsigned long, CDefString> Entry;
int main()
{
    static const unsigned long keys[7] = { 5, 1, 6, 3, 0, 4, 2 };
    Entry e[7];
    for (int i = 0; i < 7; ++i) { e[i].first = keys[i]; e[i].second.TablePos = 100 + (long)keys[i]; }
    CKeyPairCompareLess<unsigned long, CDefString> comp;
    make_heap_impl(e, e + 7, comp);
    for (Entry* last = e + 7; last - e > 1; --last) pop_heap_aux(e, last, e, comp);
    int ok = 1;
    for (int i = 0; i < 7; ++i) { const Entry& x = e[i]; ok &= x.first == (unsigned long)i && x.second.TablePos == 100 + i; }
    std::printf(ok ? "00471576_TEST PASS\n" : "00471576_TEST FAIL\n");
    return ok ? 0 : 1;
}

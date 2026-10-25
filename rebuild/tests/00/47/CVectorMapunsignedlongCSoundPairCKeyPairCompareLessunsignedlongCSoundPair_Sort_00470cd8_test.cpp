// Behaviour test for 00470cd8: CVectorMap<unsigned long,CSoundPair,CKeyPairCompareLess<..> >::Sort -- entries end in key order with their values, Dirty cleared
#include <cstdio>
#include <algorithm>
class CSoundPair { public: long FirstSound; long LastSound; };
template <class K, class V> struct Pair { K first; V second; };
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const Pair<K, V>& a, const Pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef Pair<K, V> value_type;
    value_type* First; value_type* Last; value_type* EndOfStorage; C Compare; bool Dirty;
    void Sort() { std::sort(First, Last, Compare); Dirty = false; }
};
typedef Pair<unsigned long, CSoundPair> Entry;
int main()
{
    static const unsigned long keys[9] = { 40, 7, 19, 3, 88, 0, 61, 25, 12 };
    Entry e[9];
    for (int i = 0; i < 9; ++i) { e[i].first = keys[i]; e[i].second.FirstSound = (long)keys[i] + 1; e[i].second.LastSound = (long)keys[i] + 2; }
    CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> > m;
    m.First = e; m.Last = e + 9; m.EndOfStorage = e + 9; m.Dirty = true;
    m.Sort();
    int ok = !m.Dirty;
    for (int i = 0; i < 9; ++i) { const Entry& x = e[i]; ok &= (i == 0 || e[i - 1].first < x.first) && x.second.FirstSound == (long)x.first + 1 && x.second.LastSound == (long)x.first + 2; }
    std::printf(ok ? "00470cd8_TEST PASS\n" : "00470cd8_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for 0047119a: CVectorMap<unsigned long,CDefString,CKeyPairCompareLess<..> >::Sort -- entries end in key order with their values, Dirty cleared
#include <cstdio>
#include <algorithm>
class CDefString { public: long TablePos; };
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
typedef Pair<unsigned long, CDefString> Entry;
int main()
{
    static const unsigned long keys[9] = { 40, 7, 19, 3, 88, 0, 61, 25, 12 };
    Entry e[9];
    for (int i = 0; i < 9; ++i) { e[i].first = keys[i]; e[i].second.TablePos = (long)keys[i] * 3; }
    CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> > m;
    m.First = e; m.Last = e + 9; m.EndOfStorage = e + 9; m.Dirty = true;
    m.Sort();
    int ok = !m.Dirty;
    for (int i = 0; i < 9; ++i) { const Entry& x = e[i]; ok &= (i == 0 || e[i - 1].first < x.first) && x.second.TablePos == (long)x.first * 3; }
    std::printf(ok ? "0047119a_TEST PASS\n" : "0047119a_TEST FAIL\n");
    return ok ? 0 : 1;
}

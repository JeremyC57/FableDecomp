// Behaviour test for 0081e190: CVectorMap<unsigned long,CSkeletalMorphs::CEntry,CKeyPairCompareLess<..> >::Sort -- entries end in key order with their values, Dirty cleared, clean or not
#include <cstdio>
#include <algorithm>
class CEntry { public: long MorphName; short Priority; bool SupressesLowerMorphs; };   // CSkeletalMorphs::CEntry
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
    void Update() { if (Dirty) Sort(); }
};
typedef Pair<unsigned long, CEntry> Entry;
static int Run(bool dirty)
{
    static const unsigned long keys[9] = { 40, 7, 19, 3, 88, 0, 61, 25, 12 };
    Entry e[9];
    for (int i = 0; i < 9; ++i) { e[i].first = keys[i]; e[i].second.MorphName = (long)keys[i] + 1; e[i].second.Priority = (short)(keys[i] + 2); e[i].second.SupressesLowerMorphs = (keys[i] & 1) != 0; }
    CVectorMap<unsigned long, CEntry, CKeyPairCompareLess<unsigned long, CEntry> > m;
    m.First = e; m.Last = e + 9; m.EndOfStorage = e + 9; m.Dirty = dirty;
    m.Sort();
    int ok = !m.Dirty;
    int sorted = 1;
    for (int i = 0; i < 9; ++i) { const Entry& x = e[i]; sorted &= (i == 0 || e[i - 1].first < x.first); ok &= x.second.MorphName == (long)x.first + 1 && x.second.Priority == (short)(x.first + 2) && x.second.SupressesLowerMorphs == ((x.first & 1) != 0); }
    return ok && (sorted);
}
int main()
{
    int ok = Run(true) && Run(false);
    std::printf(ok ? "0081e190_TEST PASS\n" : "0081e190_TEST FAIL\n");
    return ok ? 0 : 1;
}

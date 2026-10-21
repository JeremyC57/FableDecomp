#include <cstdio>
class CThing;
template <class T> class CIntelligentPointer {
public:
    T*   Data;
    long Handle;
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    value_type* LowerBound(const K& key);
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0)
    {
        int half = count / 2;
        value_type* middle = first + half;
        if (comp(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
template class CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >;

typedef CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > > TMap;
int main()
{
    static TMap::value_type e[5];
    long keys[5] = { 2, 4, 4, 9, 15 };
    for (int i = 0; i < 5; ++i) { e[i].first = keys[i]; e[i].second.Data = 0; e[i].second.Handle = i; }
    TMap m; m.First = e; m.Last = e + 5; m.EndOfStorage = e + 5;
    long k;
    bool ok = sizeof(TMap::value_type) == 12;
    k = 1;  ok = ok && m.LowerBound(k) == e;
    k = 2;  ok = ok && m.LowerBound(k) == e;
    k = 3;  ok = ok && m.LowerBound(k) == e + 1;
    k = 4;  ok = ok && m.LowerBound(k) == e + 1;
    k = 5;  ok = ok && m.LowerBound(k) == e + 3;
    k = 15; ok = ok && m.LowerBound(k) == e + 4;
    k = 99; ok = ok && m.LowerBound(k) == e + 5;
    m.Last = e; k = 3; ok = ok && m.LowerBound(k) == e;
    std::printf(ok ? "00447c80_TEST PASS\n" : "00447c80_TEST FAIL\n");
    return ok ? 0 : 1;
}
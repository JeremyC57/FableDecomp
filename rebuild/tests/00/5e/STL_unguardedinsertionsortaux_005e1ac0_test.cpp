// Behaviour test for 0x005E1AC0 _STL::__unguarded_insertion_sort_aux<pair<unsigned long,CAnimationEntry>*>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("005e1ac0_TEST FAIL\n"); return 1; } } while (0)

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_linear_insert_model(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
// the function under test: STLport __unguarded_insertion_sort_aux
template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_insertion_sort_aux_under_test(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    _RandomAccessIter __i;
    for (__i = __first; __i != __last; ++__i)
        unguarded_linear_insert_model(__i, _Tp(*__i), __comp);
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};

struct CAnimComponentMap { long IVRefCount; CAnimComponentMap() : IVRefCount(0) { } };
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer(T* o) : Object(o) { if (Object) ++Object->IVRefCount; }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { if (Object) --Object->IVRefCount; }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (other.Object) ++other.Object->IVRefCount;
        if (Object) --Object->IVRefCount;
        Object = other.Object;
        return *this;
    }
};
struct CDefString { long TablePos; };
struct CAnimationEntry {
    long                                 BankIndex;
    CDefString                           AnimName;
    CDefString                           GroupName;
    CIVCountedPointer<CAnimComponentMap> Components;
    CAnimationEntry(long b, CAnimComponentMap* m) : BankIndex(b), Components(m) { AnimName.TablePos = b * 2; GroupName.TablePos = b * 3; }
};
typedef pair<unsigned long, CAnimationEntry> TAnimationEntryPair;

int main()
{
    int i;
    static CAnimComponentMap maps[5];
    {
        unsigned long keys[5] = { 0, 9, 3, 7, 1 };
        TAnimationEntryPair v[5] = { TAnimationEntryPair(keys[0], CAnimationEntry(0, &maps[0])), TAnimationEntryPair(keys[1], CAnimationEntry(1, &maps[1])),
            TAnimationEntryPair(keys[2], CAnimationEntry(2, &maps[2])), TAnimationEntryPair(keys[3], CAnimationEntry(3, &maps[3])),
            TAnimationEntryPair(keys[4], CAnimationEntry(4, &maps[4])) };
        unguarded_insertion_sort_aux_under_test(v + 1, v + 5, (TAnimationEntryPair*)0, CKeyPairCompareLess<unsigned long, CAnimationEntry>());
        unsigned long expect[5] = { 0, 1, 3, 7, 9 };
        long bank[5] = { 0, 4, 2, 3, 1 };
        for (i = 0; i < 5; ++i) CHECK(v[i].first == expect[i] && v[i].second.BankIndex == bank[i] && v[i].second.AnimName.TablePos == bank[i] * 2 && v[i].second.Components.Object == &maps[bank[i]]);
        for (i = 0; i < 5; ++i) CHECK(maps[i].IVRefCount == 1);
    }
    for (i = 0; i < 5; ++i) CHECK(maps[i].IVRefCount == 0);
    std::printf("005e1ac0_TEST PASS\n");
    return 0;
}
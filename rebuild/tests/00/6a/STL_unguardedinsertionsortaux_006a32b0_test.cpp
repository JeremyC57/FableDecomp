// Behaviour test for 0x006A32B0 _STL::__unguarded_insertion_sort_aux<pair<long,CIntelligentPointer<CThing>>*>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("006a32b0_TEST FAIL\n"); return 1; } } while (0)

class CBaseObject { public: int Id; };
static int g_ptrSets = 0, g_ptrDtors = 0;
class CBaseIntelligentPointer {                         // test model: vptr + item
public:
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    virtual ~CBaseIntelligentPointer() { ++g_ptrDtors; PItem = 0; }
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* item) { PItem = item; ++g_ptrSets; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

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

class CThing;
typedef pair<long, CIntelligentPointer<CThing> > TThingEntry;

int main()
{
    int i;
    CBaseObject things[7];
    for (i = 0; i < 7; ++i) things[i].Id = 100 + i;
    long keys[7] = { -1, 30, 10, 50, 20, 40, 0 };   // v[0] is the sentinel minimum of the guarded prefix
    TThingEntry v[7] = { TThingEntry(keys[0], CIntelligentPointer<CThing>(&things[0])), TThingEntry(keys[1], CIntelligentPointer<CThing>(&things[1])),
        TThingEntry(keys[2], CIntelligentPointer<CThing>(&things[2])), TThingEntry(keys[3], CIntelligentPointer<CThing>(&things[3])),
        TThingEntry(keys[4], CIntelligentPointer<CThing>(&things[4])), TThingEntry(keys[5], CIntelligentPointer<CThing>(&things[5])),
        TThingEntry(keys[6], CIntelligentPointer<CThing>(&things[6])) };
    unguarded_insertion_sort_aux_under_test(v + 1, v + 7, (TThingEntry*)0, CKeyPairCompareLess<long, CIntelligentPointer<CThing> >());
    long expect[7] = { -1, 0, 10, 20, 30, 40, 50 };
    int owner[7] = { 0, 6, 2, 4, 1, 5, 3 };
    for (i = 0; i < 7; ++i) CHECK(v[i].first == expect[i] && v[i].second.GetPItem() == &things[owner[i]]);
    std::printf("006a32b0_TEST PASS\n");
    return 0;
}
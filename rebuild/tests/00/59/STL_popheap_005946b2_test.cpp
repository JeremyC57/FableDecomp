// Behaviour test for 0x005946B2 std::__pop_heap<pair<long,CDefPointer<const CContextSensitiveItemDef>>*, ..., CContextSensitivePriorityCompare>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("005946b2_TEST FAIL\n"); return 1; } } while (0)

static int g_deletes = 0;
class CDefPointeeBase {                                 // test model: intrusive count + Delete()
public:
    unsigned long IVRefCount;
    long          Priority;
    CDefPointeeBase() : IVRefCount(0), Priority(0) { }
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { ++g_deletes; }
};
template <class T>
class CDefPointer {
public:
    T* Object;
    CDefPointer(T* o) : Object(o) { if (Object) ++((CDefPointeeBase*)Object)->IVRefCount; }
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++((CDefPointeeBase*)Object)->IVRefCount; }
    ~CDefPointer() { if (Object) { if (--((CDefPointeeBase*)Object)->IVRefCount == 0) ((CDefPointeeBase*)Object)->Delete(); Object = 0; } }
    CDefPointer& operator=(const CDefPointer& p)
    {
        if (Object != p.Object) {
            if (Object) { if (--((CDefPointeeBase*)Object)->IVRefCount == 0) ((CDefPointeeBase*)Object)->Delete(); Object = 0; }
            Object = p.Object;
            if (Object) ++((CDefPointeeBase*)Object)->IVRefCount;
        }
        return *this;
    }
};
class CContextSensitiveItemDef : public CDefPointeeBase { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

typedef pair<long, CDefPointer<CContextSensitiveItemDef> > TContextItemPair;
class CContextSensitivePriorityCompare {                // test model: order by the pair's priority key
public:
    bool operator()(const TContextItemPair& a, const TContextItemPair& b) const { return a.first < b.first; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void push_heap_model(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void adjust_heap_model(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    push_heap_model(__first, __holeIndex, __topIndex, __val, __comp);
}
// the function under test: STLport __pop_heap
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void pop_heap_under_test(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    adjust_heap_model(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _Tp, class _Compare>
void push_heap_all(_Tp* __first, _Tp* __last, _Compare __comp)
{
    push_heap_model(__first, int((__last - __first) - 1), int(0), _Tp(*(__last - 1)), __comp);
}
template <class _Tp, class _Compare>
void pop_heap_all(_Tp* __first, _Tp* __last, _Compare __comp)
{
    pop_heap_under_test(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

int main()
{
    int i;
    static CContextSensitiveItemDef defs[6];
    {
        long keys[6] = { 40, 10, 60, 20, 50, 30 };
        TContextItemPair v[6] = {
            TContextItemPair(keys[0], CDefPointer<CContextSensitiveItemDef>(&defs[0])),
            TContextItemPair(keys[1], CDefPointer<CContextSensitiveItemDef>(&defs[1])),
            TContextItemPair(keys[2], CDefPointer<CContextSensitiveItemDef>(&defs[2])),
            TContextItemPair(keys[3], CDefPointer<CContextSensitiveItemDef>(&defs[3])),
            TContextItemPair(keys[4], CDefPointer<CContextSensitiveItemDef>(&defs[4])),
            TContextItemPair(keys[5], CDefPointer<CContextSensitiveItemDef>(&defs[5])) };
        for (i = 1; i <= 6; ++i) push_heap_all(v, v + i, CContextSensitivePriorityCompare());
        for (i = 6; i > 1; --i) pop_heap_all(v, v + i, CContextSensitivePriorityCompare());
        for (i = 0; i < 6; ++i) CHECK(v[i].first == 10 * (i + 1));
        for (i = 0; i < 6; ++i) CHECK(v[i].second.Object == &defs[v[i].first / 10 == 4 ? 0 : v[i].first / 10 == 1 ? 1 : v[i].first / 10 == 6 ? 2 : v[i].first / 10 == 2 ? 3 : v[i].first / 10 == 5 ? 4 : 5]);
        for (i = 0; i < 6; ++i) CHECK(defs[i].IVRefCount == 1);
        CHECK(g_deletes == 0);
    }
    for (i = 0; i < 6; ++i) CHECK(defs[i].IVRefCount == 0);
    CHECK(g_deletes == 6);
    std::printf("005946b2_TEST PASS\n");
    return 0;
}
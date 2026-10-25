// Behaviour test for 0x007590C0 std::sort_heap<pair<EBodyLocation,CEntry>*, CKeyPairCompareLess<..> >:
// STLport sort_heap (pop_heap / __pop_heap_aux / __pop_heap inline over an out-of-line __adjust_heap) sorts a heap of
// (signed key, 40-byte POD entry) pairs ascending and keeps every payload with its key.
#include <cstdio>
enum EBodyLocation { BODY_LOCATION_NONE = 0 };
struct CEntry { long Words[10]; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<EBodyLocation, CEntry> ElemT;
typedef CKeyPairCompareLess<EBodyLocation, CEntry> CompT;
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
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
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
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
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
}
static void Fill(ElemT& e, int k) { e.first = (EBodyLocation)k; for (int j = 0; j < 10; ++j) e.second.Words[j] = k * 100 + j; }
static bool Check(const ElemT& e) { for (int j = 0; j < 10; ++j) if (e.second.Words[j] != (long)e.first * 100 + j) return false; return true; }
int main()
{
    static const int keys[9] = { 5, 31, 12, 7, 64, 0, 99, 40, 2 };
    const int n = 9;
    int ok = 1, i;
    ElemT a[9];
    CompT comp;
    for (i = 0; i < n; ++i) Fill(a[i], keys[i]);
    for (i = n / 2; 0 < i; ) { --i; std::__adjust_heap(a, i, n, ElemT(a[i]), comp); }
    std::sort_heap(a, a + n, comp);
    for (i = 1; i < n; ++i) if (!(a[i - 1].first < a[i].first)) ok = 0;
    for (i = 0; i < n; ++i) if (!Check(a[i])) ok = 0;
    if (a[0].first != 0 || a[n - 1].first != 99) ok = 0;
    std::sort_heap(a, a + 1, comp);
    if (a[0].first != 0 || !Check(a[0])) ok = 0;
    if (!ok) { std::printf("007590c0_TEST FAIL\n"); return 1; }
    std::printf("007590c0_TEST PASS\n");
    return 0;
}

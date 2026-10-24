#include <cstdio>
#include <string.h>
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

class CThing {
public:
    C3DVector Position;
    const C3DVector& GetPosition() const { return Position; }
};

// CThingCompare_Nearest: operator() returns the nearer thing (a null loses).
class CThingCompare_Nearest {
public:
    C3DVector Pos;
    template <class T> T* operator()(T* a, T* b) const
    {
        if (!a)
            return b;
        if (!b)
            return a;
        C3DVector deltaA = a->GetPosition() - Pos;
        C3DVector deltaB = b->GetPosition() - Pos;
        return deltaA.GetLengthSquared() < deltaB.GetLengthSquared() ? a : b;
    }
};
template <class TCompare>
class CAdaptThingCompareToSTLComparitor {
public:
    TCompare* Functor;
    template <class T> bool operator()(T* a, T* b) const { return (*Functor)(a, b) == a; }
};
static CThingCompare_Nearest g_Nearest;

typedef CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> TComp;
static TComp MakeComp() { TComp c; c.Functor = &g_Nearest; return c; }

namespace _STL {
template <class _Iter> inline void iter_swap(_Iter __i1, _Iter __i2) { CThing* __tmp = *__i1; *__i1 = *__i2; *__i2 = __tmp; }

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        int __num = (int)((char*)__last - (char*)__first);
        if (__num > 0)
            memmove((char*)(__last + 1) - __num, __first, __num);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort_aux(__first + 16, __last, (CThing**)0, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template <class _Tp, class _Compare>
const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = (_Distance)(__last - __first);
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template <class _RandomAccessIterator, class _Compare>
void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CThing**)0, (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, (CThing*)(*(__last - 1)), __comp, (int*)0);
}
template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template <class _Size>
_Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            __partial_sort(__first, __last, __last, (_Tp*)0, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut = __unguarded_partition(__first, __last,
            _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)), __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
template <class _RandomAccessIter, class _Compare>
void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CThing**)0, __lg((int)(__last - __first)) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
} // namespace _STL

// ---- checks ----
template <class C> static bool IsSorted(CThing** v, int n, C comp)
{
    for (int i = 0; i + 1 < n; ++i)
        if (comp(v[i + 1], v[i]))
            return false;
    return true;
}
template <class C> static bool IsHeap(CThing** v, int n, C comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(v[(i - 1) / 2], v[i]))
            return false;
    return true;
}
static bool SameSet(CThing** a, CThing** b, int n)
{
    for (int i = 0; i < n; ++i) {
        int countA = 0, countB = 0;
        for (int j = 0; j < n; ++j) {
            if (a[j] == a[i]) ++countA;
            if (b[j] == a[i]) ++countB;
        }
        if (countA != countB)
            return false;
    }
    return true;
}
static const int N = 40;
static CThing g_things[N];
static void MakeThings()
{
    for (int i = 0; i < N; ++i) {
        // scattered positions, all away from the origin, no two equal
        float angle = (float)((i * 37) % 360) * 0.0174533f;
        float radius = 2.0f + (float)((i * 13) % 41) * 0.5f;    // 41 is prime: every radius differs
        g_things[i].Position = C3DVector(radius * (float)cos((double)angle), radius * (float)sin((double)angle), 0.0f);
    }
}
static void Fill(CThing** v, int n)
{
    for (int i = 0; i < n; ++i)
        v[i] = &g_things[(i * 7 + 3) % N];
}
static int Fail(const char* addr, const char* what)
{
    std::printf("%s_TEST FAIL (%s)\n", addr, what);
    return 1;
}


int main()
{
    static const char* A = "0096d660";
    MakeThings();
    TComp comp = MakeComp();

    for (int i = 0; i + 2 < N; ++i) {
        CThing* a = &g_things[i]; CThing* b = &g_things[(i * 5 + 1) % N]; CThing* c = &g_things[(i * 11 + 2) % N];
        CThing* const& m = _STL::__median(a, b, c, comp);
        int below = (a != m && comp(a, m) ? 1 : 0) + (b != m && comp(b, m) ? 1 : 0) + (c != m && comp(c, m) ? 1 : 0);
        int above = (a != m && comp(m, a) ? 1 : 0) + (b != m && comp(m, b) ? 1 : 0) + (c != m && comp(m, c) ? 1 : 0);
        if (&m != &a && &m != &b && &m != &c) return Fail(A, "not one of the arguments");
        if (below > 1 || above > 1) return Fail(A, "not the median");
    }

    std::printf("0096d660_TEST PASS\n");
    return 0;
}
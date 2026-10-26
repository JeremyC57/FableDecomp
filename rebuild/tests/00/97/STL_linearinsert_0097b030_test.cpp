#include <cstdio>
#include <string.h>
#include <math.h>

class C3DVector { public:
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

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};

// The in-direction-and-nearest heuristic: a sorts before b when its score is higher.
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;
    C2DVector Dir;
    CThingCompare_InDirectionOfAndNearestToHeuristicBitch(const C2DVector& pos, const C2DVector& dir) : Pos(pos), Dir(dir) { }
    float GetScore(CThing* thing) const
    {
        const C3DVector& p = thing->GetPosition();
        float dx = p.X - Pos.X;
        float dy = p.Y - Pos.Y;
        float dist = (float)sqrt((double)(dx * dx + dy * dy));
        float inv = 1.0f / dist;
        dx *= inv;
        dy *= inv;
        float along = dy * Dir.Y + dx * Dir.X;
        return (float)pow((double)along, (double)(float)((int)dist / 4 * 2 + 1));
    }
    bool operator()(CThing* a, CThing* b) const
    {
        float scoreA = GetScore(a);
        if (GetScore(b) < scoreA)
            return true;
        return false;
    }
};
typedef CThingCompare_InDirectionOfAndNearestToHeuristicBitch TComp;
static TComp MakeComp() { return TComp(C2DVector(0.0f, 0.0f), C2DVector(1.0f, 0.0f)); }

namespace std {
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
} // namespace std

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
    static const char* A = "0097b030";
    MakeThings();
    TComp comp = MakeComp();

    for (int k = 2; k <= N; k += 5) {
        CThing* v[N]; Fill(v, k);
        std::__insertion_sort(v, v + k - 1, comp);
        CThing* before[N]; for (int i = 0; i < k; ++i) before[i] = v[i];
        std::__linear_insert(v, v + k - 1, v[k - 1], comp);
        if (!IsSorted(v, k, comp)) return Fail(A, "not sorted");
        if (!SameSet(before, v, k)) return Fail(A, "permutation");
    }

    std::printf("0097b030_TEST PASS\n");
    return 0;
}
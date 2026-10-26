#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

// Model: things with positions, the nearest-to-position comparator and STLport's push / adjust heap; the heap is built
// by repeated pushes, then sort_heap must leave the things nearest first.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CThing {
public:
    C3DVector Pos;
    int       Id;
};

class CThingCompare_NearestToPos { public:
    C3DVector Pos;
    bool operator()(CThing* a, CThing* b) const
    {
        if (!a)
            return false;
        if (!b)
            return true;
        float ax = a->Pos.X - Pos.X, ay = a->Pos.Y - Pos.Y, az = a->Pos.Z - Pos.Z;
        float bx = b->Pos.X - Pos.X, by = b->Pos.Y - Pos.Y, bz = b->Pos.Z - Pos.Z;
        return ax * ax + ay * ay + az * az < bx * bx + by * by + bz * bz;
    }
};

static int g_adjustCalls = 0;

void __fastcall __push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CThingCompare_NearestToPos comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}

void __fastcall __adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CThingCompare_NearestToPos comp)
{
    ++g_adjustCalls;
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
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

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, (CThing**)0, __comp);
}

void __fastcall sort_heap(CThing** first, CThing** last, CThingCompare_NearestToPos comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}

int main()
{
    const int count = 7;
    static const float xs[count] = { 5.0f, -1.0f, 9.0f, 3.0f, -7.0f, 0.5f, 2.0f };
    CThing things[count];
    CThing* heap[count];
    CThingCompare_NearestToPos comp;
    comp.Pos = C3DVector(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < count; ++i) {
        things[i].Pos = C3DVector(xs[i], 1.0f, 0.0f);
        things[i].Id = i;
        heap[i] = &things[i];
        __push_heap(heap, i, 0, heap[i], comp);
    }

    sort_heap(heap, heap + count, comp);

    static const int expected[count] = { 5, 1, 6, 3, 0, 4, 2 };   // |x| 0.5, 1, 2, 3, 5, 7, 9
    bool ok = g_adjustCalls == count - 1;
    for (int i = 0; i < count; ++i)
        ok = ok && heap[i]->Id == expected[i];

    CThing* one[1] = { &things[0] };                     // a single element is left alone
    g_adjustCalls = 0;
    sort_heap(one, one + 1, comp);
    ok = ok && g_adjustCalls == 0 && one[0] == &things[0];

    if (!ok) {
        std::printf("007a3b60_TEST FAIL\n");
        return 1;
    }
    std::printf("007a3b60_TEST PASS\n");
    return 0;
}
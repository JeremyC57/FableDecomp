#include <cstdio>
// Behaviour test for 0x0080B9F0: STLport sort_heap over 28-byte CTargetPoints with a 52-byte by-value comparator (here: nearest-to-anchor first via a heap on "farther").
class CThing { public: int id; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* t) { m_pItem = t; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CTargetPoint { CIntelligentPointer PThing; long Index; C3DVector TargetingPosition; long FrameRejected; };
class CVectorCompare_BestForTargeting { public:
    C3DVector AnchorPos; C3DVector PreferredLine; float AngleCompare; float SqrDistCompare;
    float OverrideShortDistanceFactor; float OverrideShortDistanceFalloff; bool CompareInXYPlane;
};
static int g_compCopies = 0;
class CThingCompare_BestForTargeting2 { public:
    CThing* Hero; CVectorCompare_BestForTargeting VectorCompare; float EnemyPreferenceFactor;
    CThingCompare_BestForTargeting2() { }
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& o) : Hero(o.Hero), VectorCompare(o.VectorCompare), EnemyPreferenceFactor(o.EnemyPreferenceFactor) { ++g_compCopies; }
    float SqDist(const CTargetPoint& p) const
    {
        float dx = p.TargetingPosition.X - VectorCompare.AnchorPos.X, dy = p.TargetingPosition.Y - VectorCompare.AnchorPos.Y;
        return dx * dx + dy * dy;
    }
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const { return SqDist(a) < SqDist(b); }
};

// STLport heap chain (behaviour model of the retail row and its out-of-line callees)
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
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp)
{
    *__result = *__first;
    __adjust_heap(__first, 0, int(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, *(__last - 1), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void make_heap_by_push(_RandomAccessIterator __first, int __n, _Compare __comp)
{
    for (int i = 1; i < __n; ++i)
        __push_heap(__first, i, 0, *(__first + i), __comp);
}

int main()
{
    CThing things[5];
    CTargetPoint v[5];
    float xs[5] = { 3.0f, -1.0f, 5.0f, 0.5f, -4.0f };
    for (int i = 0; i < 5; ++i) {
        v[i].PThing.SetPItem(&things[i]); v[i].Index = i; v[i].FrameRejected = 0;
        v[i].TargetingPosition.X = xs[i]; v[i].TargetingPosition.Y = 0.0f; v[i].TargetingPosition.Z = 0.0f;
    }
    CThingCompare_BestForTargeting2 comp; comp.Hero = &things[0]; comp.EnemyPreferenceFactor = 1.0f;
    comp.VectorCompare.AnchorPos.X = 0.0f; comp.VectorCompare.AnchorPos.Y = 0.0f; comp.VectorCompare.CompareInXYPlane = true;
    make_heap_by_push(v, 5, comp);
    g_compCopies = 0;
    sort_heap(v, v + 5, comp);
    if (g_compCopies == 0) { std::printf("0080b9f0_TEST FAIL\n"); return 1; }   // the comparator travels by value
    int want[5] = { 3, 1, 0, 4, 2 };     // |0.5| < |-1| < |3| < |-4| < |5|
    for (int i = 0; i < 5; ++i)
        if (v[i].Index != want[i] || v[i].PThing.GetPItem() != &things[want[i]]) { std::printf("0080b9f0_TEST FAIL\n"); return 1; }
    std::printf("0080b9f0_TEST PASS\n");
    return 0;
}
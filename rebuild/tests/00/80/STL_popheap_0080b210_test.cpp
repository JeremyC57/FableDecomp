// 0080b210 behaviour test: __pop_heap over target points: repeated pops after a push-built heap leave the points sorted with their payloads intact
#include <cstdio>
#include <string.h>

// CBaseIntelligentPointer model: the tracked item pointer behind GetPItem / SetPItem
class CBaseObject { public: int id; };
class CThing : public CBaseObject { public: };
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;
    long Index;
    C3DVector TargetingPosition;
    long FrameRejected;
    CTargetPoint() : Index(0), FrameRejected(0) {}
    CTargetPoint(const CTargetPoint& o) : PThing(o.PThing), Index(o.Index), TargetingPosition(o.TargetingPosition), FrameRejected(o.FrameRejected) {}
    CTargetPoint& operator=(const CTargetPoint& o)
    {
        PThing = o.PThing; Index = o.Index; TargetingPosition = o.TargetingPosition; FrameRejected = o.FrameRejected;
        return *this;
    }
};
// comparator with state (the 0x34-byte CThingCompare_BestForTargeting2 in retail): ranks points by a weighted score
class CThingCompare_BestForTargeting2 { public:
    CThing* Hero;
    float EnemyPreferenceFactor;
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const
    {
        float va = a.TargetingPosition.X * EnemyPreferenceFactor;
        float vb = b.TargetingPosition.X * EnemyPreferenceFactor;
        return va - vb > 0.0001f;
    }
};
typedef CTargetPoint Elem;
typedef CThingCompare_BestForTargeting2 Comp;
// STLport 4.6 heap primitives, as in the source
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
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

static CThing g_things[9];
static const float kScore[9] = { 3.0f, -1.0f, 7.5f, 0.0f, 2.25f, 7.5f, -4.0f, 10.0f, 1.0f };

static int Fail(const char* why) { std::printf("0080b210_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    const int N = 9;
    Comp comp; comp.Hero = &g_things[0]; comp.EnemyPreferenceFactor = 2.0f;
    Elem a[9];
    for (int i = 0; i < N; ++i) {
        g_things[i].id = i;
        a[i].PThing.SetPItem(&g_things[i]); a[i].Index = i; a[i].TargetingPosition.X = kScore[i];
        a[i].TargetingPosition.Y = (float)i; a[i].FrameRejected = 100 + i;
    }
    for (int n = 0; n < N; ++n) { Elem v(a[n]); __push_heap(a, n, 0, v, comp); }
    for (int i = 1; i < N; ++i)
        if (comp(a[(i - 1) / 2], a[i])) return Fail("heap property");
    for (int n = N; n > 1; --n) {
        __pop_heap(a, a + (n - 1), a + (n - 1), Elem(a[n - 1]), comp, (int*)0);
    }
    for (int i = 1; i < N; ++i)
        if (comp(a[i], a[i - 1])) return Fail("order");
    int seen = 0;
    for (int i = 0; i < N; ++i) {
        const Elem& e = a[i];
        if (e.PThing.GetPItem() != &g_things[e.Index] || e.FrameRejected != 100 + e.Index || e.TargetingPosition.Y != (float)e.Index
            || e.TargetingPosition.X != kScore[e.Index]) return Fail("payload");
        seen |= 1 << e.Index;
    }
    if (seen != (1 << N) - 1) return Fail("permutation");
    // descending score order under this comparator ("a beats b" = higher score sorts later in the heap's max-first sense)
    if (a[0].TargetingPosition.X != 10.0f || a[N - 1].TargetingPosition.X != -4.0f) return Fail("ends");
    std::printf("0080b210_TEST PASS\n");
    return 0;
}
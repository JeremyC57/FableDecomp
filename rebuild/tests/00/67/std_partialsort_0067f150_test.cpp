// Behaviour test for 0x0067F150 std::__partial_sort<CDegradableInfo*, CDegradableInfo, CIsHealth1GTHealth2>: the first middle - first stages
// end up as the highest-health stages in descending order; the tail keeps the rest.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#pragma pack(push, 1)
class CEngineGraphic {
public:
    unsigned int  BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
};
#pragma pack(pop)

class CDegradableInfo {
public:
    float          HealthPercentage;
    CEngineGraphic EngineGraphic;
    int            SmashParticleEmitter;
    bool           BlocksNavigation;
};

class CDegradableDef {
public:
    class CIsHealth1GTHealth2 {
    public:
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const
        {
            return a.HealthPercentage > b.HealthPercentage;
        }
    };
};

static void Set(CDegradableInfo& d, float health, int tag)
{
    d.HealthPercentage = health; d.EngineGraphic.BankIndex = tag; d.EngineGraphic.AnimStep = 0.0f;
    d.EngineGraphic.RenderSizeX = 0.0f; d.EngineGraphic.AdditiveAlpha = false; d.EngineGraphic.Type = 0;
    d.SmashParticleEmitter = tag * 2; d.BlocksNavigation = (tag & 1) != 0;
}
static bool Is(const CDegradableInfo& d, float health, int tag)
{
    return d.HealthPercentage == health && (int)d.EngineGraphic.BankIndex == tag && d.SmashParticleEmitter == tag * 2 &&
           d.BlocksNavigation == ((tag & 1) != 0);
}

namespace nav {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)                                    // models 0x0067E650
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
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp)
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

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*)                                         // models 0x0067EAD0
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)   // models 0x0067F040
{
    while (__last - __first > 1)
        __pop_heap_aux(__first, __last--, __value_type(__first), __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last,
                               _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIterator __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

typedef CDegradableDef::CIsHealth1GTHealth2 Cmp;
// heap property under the comparator: no child orders before (comp-greater than) its parent
static bool IsHeap(const CDegradableInfo* a, int n)
{
    for (int i = 1; i < n; ++i) if (Cmp()(a[(i - 1) / 2], a[i])) return false;
    return true;
}

int main()
{
    CDegradableInfo a[7];
    Set(a[0], 0.3f, 1); Set(a[1], 0.9f, 2); Set(a[2], 0.1f, 3); Set(a[3], 0.7f, 4); Set(a[4], 0.5f, 5);
    Set(a[5], 1.0f, 6); Set(a[6], 0.2f, 7);
    nav::__partial_sort(a, a + 3, a + 7, (CDegradableInfo*)0, Cmp());
    if (!Is(a[0], 1.0f, 6) || !Is(a[1], 0.9f, 2) || !Is(a[2], 0.7f, 4)) { std::printf("0067f150_TEST FAIL\n"); return 1; }
    int tags = 0;
    for (int i = 3; i < 7; ++i) { tags += (int)a[i].EngineGraphic.BankIndex; if (a[i].HealthPercentage > 0.7f) { std::printf("0067f150_TEST FAIL\n"); return 1; } }
    if (tags != 1 + 3 + 5 + 7) { std::printf("0067f150_TEST FAIL\n"); return 1; }
    std::printf("0067f150_TEST PASS\n");
    return 0;
}
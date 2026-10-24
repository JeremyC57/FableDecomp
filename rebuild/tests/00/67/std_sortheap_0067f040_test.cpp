#include <cstdio>
// Behaviour test for 0x0067F040: STLport sort_heap over 28-byte CDegradableInfo with CIsHealth1GTHealth2 (descending health).
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
struct CDegradableInfo { float HealthPercentage; CEngineGraphic EngineGraphic; long SmashParticleEmitter; bool BlocksNavigation; };
struct CIsHealth1GTHealth2 { bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; } };

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
    CDegradableInfo v[5];
    float h[5] = { 0.5f, 1.0f, 0.0f, 0.75f, 0.25f };
    for (int i = 0; i < 5; ++i) {
        v[i].HealthPercentage = h[i]; v[i].EngineGraphic.BankIndex = 100 + i; v[i].EngineGraphic.AnimStep = 0.0f;
        v[i].EngineGraphic.RenderSizeX = 1.0f; v[i].EngineGraphic.AdditiveAlpha = false; v[i].EngineGraphic.Type = (unsigned char)i;
        v[i].SmashParticleEmitter = 10 * i; v[i].BlocksNavigation = (i & 1) != 0;
    }
    make_heap_by_push(v, 5, CIsHealth1GTHealth2());
    sort_heap(v, v + 5, CIsHealth1GTHealth2());
    float wh[5] = { 1.0f, 0.75f, 0.5f, 0.25f, 0.0f };
    unsigned long wb[5] = { 101, 103, 100, 104, 102 };
    for (int i = 0; i < 5; ++i)
        if (v[i].HealthPercentage != wh[i] || v[i].EngineGraphic.BankIndex != wb[i] || v[i].SmashParticleEmitter != (long)(10 * (wb[i] - 100))) {
            std::printf("0067f040_TEST FAIL\n"); return 1;
        }
    std::printf("0067f040_TEST PASS\n");
    return 0;
}
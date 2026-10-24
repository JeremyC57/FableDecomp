#include <cstdio>
#include <stdlib.h>
#include <new>

// Copy of the source's polymorphic model (the test defines the virtuals).
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;
};
class C3DAnimationState : public CIVCountedPointeeBase {
public:
    virtual ~C3DAnimationState() { }
    virtual long GetType() const { return 1; }
    virtual void Validate() { }
    long  AnimationIndex;
    float AnimationTime;
};
class C3DAnimationStateAlpha : public C3DAnimationState {
public:
    virtual long GetType() const { return 2; }
    virtual void Validate() { }
    float AlphaWeight;
};

static void Set(C3DAnimationStateAlpha& a, long index, float time, float weight)
{
    a.IVRefCount = 0;
    a.AnimationIndex = index;
    a.AnimationTime = time;
    a.AlphaWeight = weight;
}
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
static bool Is(const C3DAnimationStateAlpha& a, long index, float time, float weight)
{
    return a.GetType() == 2 && a.AnimationIndex == index && Near(a.AnimationTime, time) && Near(a.AlphaWeight, weight);
}

namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

int main()
{
    C3DAnimationStateAlpha x;
    Set(x, 7, 3.0f, 0.125f);
    C3DAnimationStateAlpha* raw = (C3DAnimationStateAlpha*)malloc(5 * sizeof(C3DAnimationStateAlpha));
    C3DAnimationStateAlpha* end = _STL::__uninitialized_fill_n(raw, 4u, x, _STL::__false_type());
    if (end != raw + 4)
        { std::printf("00a3cbe0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (!Is(raw[i], 7, 3.0f, 0.125f))
            { std::printf("00a3cbe0_TEST FAIL\n"); return 1; }
    if (_STL::__uninitialized_fill_n(raw + 4, 0u, x, _STL::__false_type()) != raw + 4)
        { std::printf("00a3cbe0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        raw[i].~C3DAnimationStateAlpha();
    free(raw);
    std::printf("00a3cbe0_TEST PASS\n");
    return 0;
}
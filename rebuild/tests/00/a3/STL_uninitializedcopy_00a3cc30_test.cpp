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
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

int main()
{
    C3DAnimationStateAlpha src[3];
    Set(src[0], 1, 0.5f, 0.25f);
    Set(src[1], 2, 1.5f, 0.5f);
    Set(src[2], 3, 2.5f, 0.75f);
    C3DAnimationStateAlpha* raw = (C3DAnimationStateAlpha*)malloc(4 * sizeof(C3DAnimationStateAlpha));
    const C3DAnimationStateAlpha* first = src;
    C3DAnimationStateAlpha* end = _STL::__uninitialized_copy(first, first + 3, raw, _STL::__false_type());
    if (end != raw + 3 || !Is(raw[0], 1, 0.5f, 0.25f) || !Is(raw[1], 2, 1.5f, 0.5f) || !Is(raw[2], 3, 2.5f, 0.75f))
        { std::printf("00a3cc30_TEST FAIL\n"); return 1; }
    if (_STL::__uninitialized_copy(first, first, raw + 3, _STL::__false_type()) != raw + 3)
        { std::printf("00a3cc30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        raw[i].~C3DAnimationStateAlpha();
    free(raw);
    std::printf("00a3cc30_TEST PASS\n");
    return 0;
}
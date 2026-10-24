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
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

int main()
{
    C3DAnimationStateAlpha a[5];
    for (int i = 0; i < 5; ++i)
        Set(a[i], i, (float)i, (float)i * 0.25f);
    // close a hole at [1,3) (overlapping, as vector::erase does)
    C3DAnimationStateAlpha* r = _STL::__copy(a + 3, a + 5, a + 1, _STL::random_access_iterator_tag(), (int*)0);
    if (r != a + 3 || !Is(a[0], 0, 0.0f, 0.0f) || !Is(a[1], 3, 3.0f, 0.75f) || !Is(a[2], 4, 4.0f, 1.0f)
        || !Is(a[3], 3, 3.0f, 0.75f) || !Is(a[4], 4, 4.0f, 1.0f))
        { std::printf("00a3cf20_TEST FAIL\n"); return 1; }
    std::printf("00a3cf20_TEST PASS\n");
    return 0;
}
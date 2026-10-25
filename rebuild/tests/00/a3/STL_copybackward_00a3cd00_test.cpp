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

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}

int main()
{
    C3DAnimationStateAlpha a[5];
    for (int i = 0; i < 5; ++i)
        Set(a[i], i, (float)i, (float)i * 0.5f);
    // shift [0,3) up by one (overlapping, as vector::insert does)
    C3DAnimationStateAlpha* r = std::__copy_backward(a, a + 3, a + 4, std::random_access_iterator_tag(), (int*)0);
    if (r != a + 1 || !Is(a[0], 0, 0.0f, 0.0f) || !Is(a[1], 0, 0.0f, 0.0f) || !Is(a[2], 1, 1.0f, 0.5f)
        || !Is(a[3], 2, 2.0f, 1.0f) || !Is(a[4], 4, 4.0f, 2.0f))
        { std::printf("00a3cd00_TEST FAIL\n"); return 1; }
    std::printf("00a3cd00_TEST PASS\n");
    return 0;
}
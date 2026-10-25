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
template <class _ForwardIter, class _Tp>
void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
}

int main()
{
    C3DAnimationStateAlpha a[4];
    for (int i = 0; i < 4; ++i)
        Set(a[i], i, (float)i, 0.0f);
    C3DAnimationStateAlpha v;
    Set(v, 9, 9.5f, 0.5f);
    std::fill(a + 1, a + 3, v);
    if (!Is(a[0], 0, 0.0f, 0.0f) || !Is(a[1], 9, 9.5f, 0.5f) || !Is(a[2], 9, 9.5f, 0.5f) || !Is(a[3], 3, 3.0f, 0.0f))
        { std::printf("00a3cee0_TEST FAIL\n"); return 1; }
    std::printf("00a3cee0_TEST PASS\n");
    return 0;
}
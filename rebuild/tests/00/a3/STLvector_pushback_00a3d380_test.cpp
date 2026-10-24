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

static int g_overflow = 0;
static long g_overflowIndex = 0;

namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    // test stub for the out-of-line grow path 0x00A3D060: records the call
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        if (__atend && __fill_len == 1 && __position == _M_finish) {
            ++g_overflow;
            g_overflowIndex = __x.AnimationIndex;
        }
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

int main()
{
    _STL::vector<C3DAnimationStateAlpha> v;
    v._M_start = (C3DAnimationStateAlpha*)malloc(2 * sizeof(C3DAnimationStateAlpha));
    v._M_finish = v._M_start;
    v._M_end_of_storage = v._M_start + 2;
    C3DAnimationStateAlpha x;
    Set(x, 1, 1.0f, 0.5f);
    v.push_back(x);
    Set(x, 2, 2.0f, 0.25f);
    v.push_back(x);
    Set(x, 3, 3.0f, 0.125f);
    v.push_back(x);
    if (v._M_finish != v._M_start + 2 || !Is(v._M_start[0], 1, 1.0f, 0.5f) || !Is(v._M_start[1], 2, 2.0f, 0.25f)
        || g_overflow != 1 || g_overflowIndex != 3)
        { std::printf("00a3d380_TEST FAIL\n"); return 1; }
    v._M_start[0].~C3DAnimationStateAlpha();
    v._M_start[1].~C3DAnimationStateAlpha();
    free(v._M_start);
    std::printf("00a3d380_TEST PASS\n");
    return 0;
}
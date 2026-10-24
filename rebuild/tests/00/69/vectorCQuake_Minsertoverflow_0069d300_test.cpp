// Behaviour test for 0x0069D300 vector<CQuake>::_M_insert_overflow: growth to old + max(old, n), the head copied,
// n copies of x placed (the single-element arm and the fill arm), the tail copied unless atend, the old block freed.
#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CGameCameraManager {
public:
    class CQuake {
    public:
        C3DVector Position;
        C3DVector Axis;
        int       Frame;
        float     Amplitude;
        float     Duration;
        float     Frequency;
        float     Phase;
        bool      Global;
    };
};
typedef CGameCameraManager::CQuake CQuake;
typedef char QuakeIs48[sizeof(CQuake) == 0x30 ? 1 : -1];

namespace nav {
struct __false_type { __false_type() {} };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result, const __false_type&)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
}

static CQuake MakeQuake(int i)
{
    CQuake q;
    q.Position = C3DVector(1.0f * i, 2.0f * i, 3.0f * i);
    q.Axis = C3DVector(0.0f, 0.0f, 1.0f + i);
    q.Frame = 100 + i;
    q.Amplitude = 0.5f * i;
    q.Duration = 2.0f + i;
    q.Frequency = 10.0f;
    q.Phase = 0.25f * i;
    q.Global = (i & 1) != 0;
    return q;
}
static bool SameQuake(const CQuake& a, const CQuake& b)
{
    return a.Position.X == b.Position.X && a.Position.Y == b.Position.Y && a.Position.Z == b.Position.Z &&
           a.Axis.X == b.Axis.X && a.Axis.Y == b.Axis.Y && a.Axis.Z == b.Axis.Z && a.Frame == b.Frame &&
           a.Amplitude == b.Amplitude && a.Duration == b.Duration && a.Frequency == b.Frequency &&
           a.Phase == b.Phase && a.Global == b.Global;
}

namespace nav {
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        T* new_start = len != 0 ? (T*)std::malloc(len * sizeof(T)) : 0;
        T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) {
            _Construct(new_finish, x);
            ++new_finish;
        } else
            new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
        if (!atend)
            new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
        if (_M_start != 0) std::free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};
}

int main()
{
    nav::vector<CQuake> v = { 0, 0, 0 };
    // push_back into an empty vector: len = 0 + max(0, 1) = 1
    CQuake q1 = MakeQuake(1);
    v._M_insert_overflow(v._M_finish, q1, nav::__false_type(), 1, true);
    if (v.size() != 1 || v._M_end_of_storage != v._M_start + 1 || !SameQuake(v._M_start[0], q1)) {
        std::printf("0069d300_TEST FAIL\n"); return 1;
    }
    // push_back again: len = 1 + 1 = 2
    CQuake q2 = MakeQuake(2);
    v._M_insert_overflow(v._M_finish, q2, nav::__false_type(), 1, true);
    if (v.size() != 2 || v._M_end_of_storage != v._M_start + 2 || !SameQuake(v._M_start[1], q2)) {
        std::printf("0069d300_TEST FAIL\n"); return 1;
    }
    // insert three copies in the middle (fill arm, tail copied): len = 2 + max(2, 3) = 5
    CQuake q9 = MakeQuake(9);
    v._M_insert_overflow(v._M_start + 1, q9, nav::__false_type(), 3, false);
    if (v.size() != 5 || v._M_end_of_storage != v._M_start + 5) { std::printf("0069d300_TEST FAIL\n"); return 1; }
    if (!SameQuake(v._M_start[0], q1) || !SameQuake(v._M_start[1], q9) || !SameQuake(v._M_start[2], q9) ||
        !SameQuake(v._M_start[3], q9) || !SameQuake(v._M_start[4], q2)) {
        std::printf("0069d300_TEST FAIL\n"); return 1;
    }
    std::free(v._M_start);
    std::printf("0069d300_TEST PASS\n");
    return 0;
}
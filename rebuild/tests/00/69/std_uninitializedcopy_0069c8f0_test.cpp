// Behaviour test for 0x0069C8F0 std::__uninitialized_copy<CQuake*, CQuake*>: every quake of [first, last) is copied
// field by field into raw storage, the return is result + (last - first), an empty range copies nothing.
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

int main()
{
    CQuake src[3] = { MakeQuake(1), MakeQuake(2), MakeQuake(3) };
    unsigned char raw[5 * sizeof(CQuake)];
    for (unsigned int i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CQuake* dst = (CQuake*)raw;
    CQuake* end = nav::__uninitialized_copy(src, src + 3, dst, nav::__false_type());
    if (end != dst + 3) { std::printf("0069c8f0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (!SameQuake(dst[i], src[i])) { std::printf("0069c8f0_TEST FAIL\n"); return 1; }
    if (raw[3 * sizeof(CQuake)] != 0xAB) { std::printf("0069c8f0_TEST FAIL\n"); return 1; }
    if (nav::__uninitialized_copy(src, src, dst + 4, nav::__false_type()) != dst + 4 || raw[4 * sizeof(CQuake)] != 0xAB) {
        std::printf("0069c8f0_TEST FAIL\n"); return 1;
    }
    std::printf("0069c8f0_TEST PASS\n");
    return 0;
}
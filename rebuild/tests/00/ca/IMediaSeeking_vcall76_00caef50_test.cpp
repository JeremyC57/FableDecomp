// Behaviour test for the IMediaSeeking vcall thunk {76} (0x00CAEF50): calling through a pointer to the virtual member GetPreroll
// dispatches to the object's own override (the thunk jumps through vtable slot 19), as CPosPassThru::GetSeekingLongLong does
// with `(pMS->*pMethod)(pll)`; a second implementation of the interface reached through the same member pointer runs its own body.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
struct IMediaSeeking {
    virtual ~IMediaSeeking() {}
    virtual long GetDuration(LONGLONG* p) = 0;
    virtual long GetStopPosition(LONGLONG* p) = 0;
    virtual long GetPreroll(LONGLONG* p) = 0;
};
struct A : IMediaSeeking {
    long GetDuration(LONGLONG* p) { *p = 100; return 0; }
    long GetStopPosition(LONGLONG* p) { *p = 200; return 0; }
    long GetPreroll(LONGLONG* p) { *p = 300; return 0; }
};
struct B : IMediaSeeking {
    long GetDuration(LONGLONG* p) { *p = -1; return 1; }
    long GetStopPosition(LONGLONG* p) { *p = -2; return 1; }
    long GetPreroll(LONGLONG* p) { *p = -3; return 1; }
};
static long GetSeekingLongLong(IMediaSeeking* pMS, long (IMediaSeeking::*pMethod)(LONGLONG*), LONGLONG* pll)
{
    return (pMS->*pMethod)(pll);
}
int main()
{
    A a; B b; LONGLONG v = 0;
    if (GetSeekingLongLong(&a, &IMediaSeeking::GetPreroll, &v) != 0 || v != 300) { std::printf("00caef50_TEST FAIL 1\n"); return 1; }
    if (GetSeekingLongLong(&b, &IMediaSeeking::GetPreroll, &v) != 1 || v != -3) { std::printf("00caef50_TEST FAIL 2\n"); return 1; }
    std::printf("00caef50_TEST PASS\n");
    return 0;
}
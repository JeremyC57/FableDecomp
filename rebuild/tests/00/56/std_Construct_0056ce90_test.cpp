// 0056ce90 behaviour test for std::_Construct<CUnprovenPod8_0056ce90> (placement copy of an 8-byte element)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
#include <new>
struct CUnprovenPod8_0056ce90 { long Word0; long Word4; };
template <class T1, class T2> inline void __fastcall Construct(T1* p, const T2& value) { new (p) T1(value); }
static void run()
{
    CUnprovenPod8_0056ce90 src = { 0x1234, -7 };
    CUnprovenPod8_0056ce90 dst = { 0, 0 };
    Construct(&dst, src);
    CHECK(dst.Word0 == 0x1234 && dst.Word4 == -7);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0056ce90_TEST FAIL\n");
        return 1;
    }
    std::printf("0056ce90_TEST PASS\n");
    return 0;
}
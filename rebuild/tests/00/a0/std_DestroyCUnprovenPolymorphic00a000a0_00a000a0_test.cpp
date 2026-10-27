// Test for 0x00A000A0 std::_Destroy<T>(T*): runs the (virtual) destructor of the pointee in place, dispatching to the dynamic type.
#include <stdio.h>
#include <new>
static int g_base = 0, g_derived = 0;
class CPoly { public: virtual ~CPoly() { ++g_base; } };
class CPolyDerived : public CPoly { public: virtual ~CPolyDerived() { ++g_derived; } };
namespace stdmodel { template <class _Tp> void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); } }
static double g_buf[4];
int main()
{
    CPoly* p = new ((void*)g_buf) CPolyDerived;
    stdmodel::_Destroy(p);
    bool ok = g_derived == 1 && g_base == 1;
    puts(ok ? "00a000a0_TEST PASS" : "00a000a0_TEST FAIL");
    return ok ? 0 : 1;
}
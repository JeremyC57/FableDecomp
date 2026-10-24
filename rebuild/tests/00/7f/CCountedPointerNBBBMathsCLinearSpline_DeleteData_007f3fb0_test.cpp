#include <cstdio>
#include <cmath>
// Behaviour test: CCountedPointer<NBBBMaths::CLinearSpline>::DeleteData -- deletes the spline through the void pointer, running its destructor (which frees the key vector).
#define CHECK(c) if (!(c)) { std::printf("007f3fb0_TEST FAIL\n"); return 1; }
#include <cstdlib>
static int g_freed = 0, g_dtors = 0;
struct CRetailVector { float* _M_start; ~CRetailVector() { if (_M_start) { std::free(_M_start); ++g_freed; } } };
namespace NBBBMaths { struct CLinearSpline { CRetailVector Keys; ~CLinearSpline() { ++g_dtors; } }; }
template <class T> struct CCountedPointer { static void DeleteData(void* data) { delete (T*)data; } };
int main()
{
    NBBBMaths::CLinearSpline* a = new NBBBMaths::CLinearSpline; a->Keys._M_start = (float*)std::malloc(16);
    NBBBMaths::CLinearSpline* b = new NBBBMaths::CLinearSpline; b->Keys._M_start = 0;
    CCountedPointer<NBBBMaths::CLinearSpline>::DeleteData(a);
    CHECK(g_dtors == 1 && g_freed == 1)
    CCountedPointer<NBBBMaths::CLinearSpline>::DeleteData(b);
    CHECK(g_dtors == 2 && g_freed == 1)
    CCountedPointer<NBBBMaths::CLinearSpline>::DeleteData(0);
    CHECK(g_dtors == 2)
    std::printf("007f3fb0_TEST PASS\n");
    return 0;
}
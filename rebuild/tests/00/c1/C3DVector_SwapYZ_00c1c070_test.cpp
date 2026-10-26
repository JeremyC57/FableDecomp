#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 00c1c070: SwapYZ returns (x, z, y) and leaves the source unchanged.
class C3DVector { public:
    float x; float y; float z;
    C3DVector SwapYZ() const;
};
C3DVector C3DVector::SwapYZ() const
{
    C3DVector result;
    result.x = x;
    result.y = z;
    result.z = y;
    return result;
}
int main()
{
    C3DVector v; v.x = 1.0f; v.y = 2.0f; v.z = 3.0f;
    C3DVector r = v.SwapYZ();
    if (r.x != 1.0f || r.y != 3.0f || r.z != 2.0f || v.y != 2.0f) { std::printf("00c1c070_TEST FAIL\n"); return 1; }
    std::printf("00c1c070_TEST PASS\n");
    return 0;
}
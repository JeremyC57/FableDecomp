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
// Behaviour test for 00c1bff0: Copy returns (x, y, z) unchanged.
struct C3DVector {
    float x; float y; float z;
    C3DVector Copy() const;
};
C3DVector C3DVector::Copy() const
{
    C3DVector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}
int main()
{
    C3DVector v; v.x = -1.5f; v.y = 2.25f; v.z = 8.0f;
    C3DVector r = v.Copy();
    if (r.x != -1.5f || r.y != 2.25f || r.z != 8.0f) { std::printf("00c1bff0_TEST FAIL\n"); return 1; }
    std::printf("00c1bff0_TEST PASS\n");
    return 0;
}

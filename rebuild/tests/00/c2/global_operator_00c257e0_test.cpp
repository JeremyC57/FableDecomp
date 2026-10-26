// Behaviour test for operator+(const C3DVector&, const C3DVector&) @ 0x00C257E0
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

C3DVector __fastcall operator+(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}

int main()
{
    C3DVector a(1.0f, 2.0f, 3.0f), b(0.5f, -4.0f, 10.0f);
    C3DVector c = a + b;
    if (c.X != 1.5f || c.Y != -2.0f || c.Z != 13.0f) { std::printf("00c257e0_TEST FAIL\n"); return 1; }
    std::printf("00c257e0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of std::swap over a 12-byte vector with a memberwise copy constructor.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
template <class T>
void __fastcall Swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
int main()
{
    C3DVector a(1.0f, 2.0f, 3.0f);
    C3DVector b(-4.0f, 5.5f, 6.25f);
    Swap(a, b);
    bool ok = a.X == -4.0f && a.Y == 5.5f && a.Z == 6.25f && b.X == 1.0f && b.Y == 2.0f && b.Z == 3.0f;
    Swap(a, a);                                  // self-swap keeps the value
    if (a.X != -4.0f || a.Y != 5.5f || a.Z != 6.25f)
        ok = false;
    if (!ok) { std::printf("008e3150_TEST FAIL\n"); return 1; }
    std::printf("008e3150_TEST PASS\n");
    return 0;
}
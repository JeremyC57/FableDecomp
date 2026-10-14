#include <cstdio>
struct Base { virtual void v00() {} unsigned char st[0x5c]; Base() {} };
struct DI { long i; DI() : i(-1) {} };
struct V { float X, Y, Z; V() : X(0.0f), Y(0.0f), Z(0.0f) {} };
struct D : Base { virtual void v00() {} DI idx; V off; float speed; D() : Base(), idx(), off(), speed(0.0f) {} };
int main()
{
    D d;
    if (d.idx.i != -1 || d.off.X != 0 || d.off.Y != 0 || d.off.Z != 0 || d.speed != 0) { std::printf("00458d3f_TEST FAIL\n"); return 1; }
    std::printf("00458d3f_TEST PASS\n");
    return 0;
}
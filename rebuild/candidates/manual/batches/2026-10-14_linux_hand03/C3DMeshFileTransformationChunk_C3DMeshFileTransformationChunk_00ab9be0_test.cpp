#include <cstdio>

struct V { float X, Y, Z; };
struct M { V XAxis, YAxis, ZAxis, Position; M() { SetIdentity(); } void SetIdentity() {
    XAxis.X = 1; XAxis.Y = 0; XAxis.Z = 0; YAxis.X = 0; YAxis.Y = 1; YAxis.Z = 0;
    ZAxis.X = 0; ZAxis.Y = 0; ZAxis.Z = 1; Position.X = 0; Position.Y = 0; Position.Z = 0; } };
struct T { const char* type; M Transform; T() : type("TRFM") {} };
int main()
{
    T t; if (t.Transform.XAxis.X != 1 || t.Transform.YAxis.Y != 1 || t.Transform.ZAxis.Z != 1 || t.Transform.ZAxis.X != 0 || t.Transform.Position.Z != 0) { std::printf("00ab9be0_TEST FAIL\n"); return 1; }
    std::printf("00ab9be0_TEST PASS\n");
    return 0;
}

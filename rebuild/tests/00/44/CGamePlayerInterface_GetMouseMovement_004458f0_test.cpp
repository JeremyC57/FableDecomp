#include <cstdio>

struct V3 { float X, Y, Z; V3() : X(0), Y(0), Z(0) {} };
struct V2 { float X, Y; V2(float x, float y) : X(x), Y(y) {} };
struct Ev { V3 GetMovement() { V3 v; v.X = 3.0f; v.Y = 2.0f; v.Z = 1.0f; return v; } };
struct PI { V2 Get(Ev* e) { V3 m = e->GetMovement(); return V2(m.X, -m.Y); } };
int main()
{
    Ev e; PI p; V2 r = p.Get(&e);
    if (r.X != 3.0f || r.Y != -2.0f) { std::printf("004458f0_TEST FAIL\n"); return 1; }
    std::printf("004458f0_TEST PASS\n");
    return 0;
}
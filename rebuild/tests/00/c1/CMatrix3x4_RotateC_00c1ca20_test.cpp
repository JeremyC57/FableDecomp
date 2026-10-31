// Behaviour test for 00c1ca20: CMatrix3x4::RotateC -- rotate every row's (X, Y) pair in place: (c*X + s*Y, c*Y - s*X), Z untouched
#include <cstdio>
#include <math.h>

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Position;
    void RotateC(float angle)
    {
        float c = (float)cos(angle), s = (float)sin(angle);
        float sx;
        sx = s * Right.X;    float t = s * Right.Y; Right.X = t + c * Right.X;    Right.Y    = c * Right.Y    - sx;
        sx = s * Up.X;       Up.X       = s * Up.Y       + c * Up.X;       Up.Y       = c * Up.Y       - sx;
        sx = s * Forward.X;  Forward.X  = s * Forward.Y  + c * Forward.X;  Forward.Y  = c * Forward.Y  - sx;
        sx = s * Position.X; Position.X = s * Position.Y + c * Position.X; Position.Y = c * Position.Y - sx;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }
static bool Is(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }

int main()
{
    if (sizeof(CMatrix3x4) != 0x30) { std::printf("00c1ca20_TEST FAIL\n"); return 1; }
    CMatrix3x4 m = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 2, 3, 5 } };
    m.RotateC(1.5707964f);                 // quarter turn: (X, Y) -> (Y, -X)
    if (!Is(m.Right, 0, -1, 0) || !Is(m.Up, 1, 0, 0) || !Is(m.Forward, 0, 0, 1) || !Is(m.Position, 3, -2, 5)) {
        std::printf("00c1ca20_TEST FAIL\n"); return 1;
    }
    CMatrix3x4 n = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 1, 1, 0 } };
    n.RotateC(0.5f);
    float c = (float)cos(0.5f), s = (float)sin(0.5f);
    if (!Is(n.Right, c * 1 + s * 2, c * 2 - s * 1, 3)) { std::printf("00c1ca20_TEST FAIL\n"); return 1; }
    n.RotateC(-0.5f);
    if (!Is(n.Right, 1, 2, 3) || !Is(n.Up, 4, 5, 6) || !Is(n.Forward, 7, 8, 9) || !Is(n.Position, 1, 1, 0)) {
        std::printf("00c1ca20_TEST FAIL\n"); return 1;
    }
    std::printf("00c1ca20_TEST PASS\n");
    return 0;
}
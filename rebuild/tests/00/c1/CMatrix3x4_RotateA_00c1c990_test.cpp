// Behaviour test for 00c1c990: CMatrix3x4::RotateA -- rotate every row's (X, Z) pair in place: (c*X - s*Z, c*Z + s*X), Y untouched
#include <cstdio>
#include <math.h>

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Position;
    void RotateA(float angle)
    {
        float c = (float)cos(angle), s = (float)sin(angle);
        float sx;
        sx = s * Right.X;    Right.X    = c * Right.X    - s * Right.Z;    Right.Z    = c * Right.Z    + sx;
        sx = s * Up.X;       Up.X       = c * Up.X       - s * Up.Z;       Up.Z       = c * Up.Z       + sx;
        sx = s * Forward.X;  Forward.X  = c * Forward.X  - s * Forward.Z;  Forward.Z  = c * Forward.Z  + sx;
        sx = s * Position.X; Position.X = c * Position.X - s * Position.Z; Position.Z = c * Position.Z + sx;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }
static bool Is(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }

int main()
{
    CMatrix3x4 m = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 2, 5, 3 } };
    m.RotateA(1.5707964f);                 // quarter turn: (X, Z) -> (-Z, X)
    if (!Is(m.Right, 0, 0, 1) || !Is(m.Up, 0, 1, 0) || !Is(m.Forward, -1, 0, 0) || !Is(m.Position, -3, 5, 2)) {
        std::printf("00c1c990_TEST FAIL\n"); return 1;
    }
    CMatrix3x4 n = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 1, 0, 1 } };
    n.RotateA(0.25f);
    float c = (float)cos(0.25f), s = (float)sin(0.25f);
    if (!Is(n.Up, c * 4 - s * 6, 5, c * 6 + s * 4) || !Is(n.Position, c - s, 0, c + s)) { std::printf("00c1c990_TEST FAIL\n"); return 1; }
    n.RotateA(-0.25f);
    if (!Is(n.Right, 1, 2, 3) || !Is(n.Up, 4, 5, 6) || !Is(n.Forward, 7, 8, 9) || !Is(n.Position, 1, 0, 1)) {
        std::printf("00c1c990_TEST FAIL\n"); return 1;
    }
    std::printf("00c1c990_TEST PASS\n");
    return 0;
}
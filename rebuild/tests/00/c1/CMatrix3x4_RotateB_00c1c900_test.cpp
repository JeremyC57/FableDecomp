// Behaviour test for 00c1c900: CMatrix3x4::RotateB -- rotate every row's (Y, Z) pair in place: (c*Y + s*Z, c*Z - s*Y), X untouched
#include <cstdio>
#include <math.h>

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Position;
    void RotateB(float angle)
    {
        float c = (float)cos(angle), s = (float)sin(angle);
        float sy;
        sy = s * Right.Y;    Right.Y    = s * Right.Z    + c * Right.Y;    Right.Z    = c * Right.Z    - sy;
        sy = s * Up.Y;       Up.Y       = s * Up.Z       + c * Up.Y;       Up.Z       = c * Up.Z       - sy;
        sy = s * Forward.Y;  Forward.Y  = s * Forward.Z  + c * Forward.Y;  Forward.Z  = c * Forward.Z  - sy;
        sy = s * Position.Y; Position.Y = s * Position.Z + c * Position.Y; Position.Z = c * Position.Z - sy;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }
static bool Is(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }

int main()
{
    if (sizeof(CMatrix3x4) != 0x30) { std::printf("00c1c900_TEST FAIL\n"); return 1; }
    CMatrix3x4 m = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 5, 2, 3 } };
    m.RotateB(1.5707964f);                 // quarter turn: (Y, Z) -> (Z, -Y)
    if (!Is(m.Right, 1, 0, 0) || !Is(m.Up, 0, 0, -1) || !Is(m.Forward, 0, 1, 0) || !Is(m.Position, 5, 3, -2)) {
        std::printf("00c1c900_TEST FAIL\n"); return 1;
    }
    CMatrix3x4 n = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 1, 1, 0 } };
    n.RotateB(0.5f);
    float c = (float)cos(0.5f), s = (float)sin(0.5f);
    if (!Is(n.Right, 1, c * 2 + s * 3, c * 3 - s * 2) || !Is(n.Position, 1, c, -s)) { std::printf("00c1c900_TEST FAIL\n"); return 1; }
    n.RotateB(-0.5f);                      // inverse rotation restores the rows
    if (!Is(n.Right, 1, 2, 3) || !Is(n.Up, 4, 5, 6) || !Is(n.Forward, 7, 8, 9) || !Is(n.Position, 1, 1, 0)) {
        std::printf("00c1c900_TEST FAIL\n"); return 1;
    }
    std::printf("00c1c900_TEST PASS\n");
    return 0;
}
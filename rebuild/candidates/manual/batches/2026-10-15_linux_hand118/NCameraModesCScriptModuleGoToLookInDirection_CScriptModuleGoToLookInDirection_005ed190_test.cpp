// Behaviour test for 0x005ED190 NCameraModes::CScriptModuleGoToLookInDirection::CScriptModuleGoToLookInDirection
#include <cstdio>
#include <cmath>
static int g_base = 0;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    float Magnitude() const { return (float)std::sqrt((double)(X * X + Y * Y + Z * Z)); }
    void Normalise() { float scale = 1.0f / Magnitude(); X *= scale; Y *= scale; Z *= scale; }
};
struct CScriptModuleBase { CScriptModuleBase() { ++g_base; } virtual ~CScriptModuleBase() {} };
struct CScriptModuleGoToLookInDirection : CScriptModuleBase {
    C3DVector GoTo, LookDirection;
    CScriptModuleGoToLookInDirection(const C3DVector& g, const C3DVector& l) : CScriptModuleBase(), GoTo(g), LookDirection(l) { LookDirection.Normalise(); }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    C3DVector g; g.X = 1; g.Y = 2; g.Z = 3;
    C3DVector l; l.X = 3; l.Y = 0; l.Z = 4;
    CScriptModuleGoToLookInDirection m(g, l);
    bool ok = g_base == 1 && m.GoTo.X == 1 && m.GoTo.Y == 2 && m.GoTo.Z == 3
        && Near(m.LookDirection.X, 0.6f) && Near(m.LookDirection.Y, 0.0f) && Near(m.LookDirection.Z, 0.8f);
    if (!ok) { std::printf("005ed190_TEST FAIL\n"); return 1; }
    std::printf("005ed190_TEST PASS\n");
    return 0;
}

// Behaviour test for CRenderManager2D::Draw2DLine (0x009DD960), g++ model: two screen vertices (shared RHW, colour, zero UV/specular/Z) and the blend mode from the flags.
#include <cstdio>
struct Vtx { float X, Y, Z, RHW; unsigned long Diffuse, Specular; float U, V; };
static float g_ScreenRHW = 0.5f;
static float AX(float x) { return x * 640.0f; }
static float AY(float y) { return y * 480.0f; }
static long Draw2DLine(float sx, float sy, float ex, float ey, unsigned long argb, unsigned char alpha, unsigned long flags, Vtx& v0, Vtx& v1)
{
    long mode = 1;
    v0.X = AX(sx); v0.Y = AY(sy); v1.X = AX(ex); v1.Y = AY(ey);
    v1.RHW = g_ScreenRHW; v0.Diffuse = v1.Diffuse = argb; v0.Specular = v1.Specular = 0; v0.Z = v1.Z = 0; v0.RHW = v1.RHW;
    v0.U = v1.U = v0.V = v1.V = 0;
    if (flags & 1) { if (alpha != 0xff) mode = 3; } else mode = (flags & 2) ? 2 : 1;
    return mode;
}
int main()
{
    Vtx a, b;
    long m = Draw2DLine(0.5f, 0.25f, 1.0f, 0.0f, 0x80ff0000, 0x80, 1, a, b);
    int ok = m == 3 && a.X == 320 && a.Y == 120 && b.X == 640 && a.RHW == 0.5f && b.RHW == 0.5f && a.Diffuse == 0x80ff0000 && b.Specular == 0;
    ok = ok && Draw2DLine(0, 0, 0, 0, 0, 0xff, 1, a, b) == 1 && Draw2DLine(0, 0, 0, 0, 0, 0x10, 2, a, b) == 2 && Draw2DLine(0, 0, 0, 0, 0, 0x10, 0, a, b) == 1;
    std::printf(ok ? "009dd960_TEST PASS\n" : "009dd960_TEST FAIL\n");
    return ok ? 0 : 1;
}

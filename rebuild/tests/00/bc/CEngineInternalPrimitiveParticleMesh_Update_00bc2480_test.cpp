// Behaviour test for CEngineInternalPrimitiveParticleMesh::Update (0x00BC2480): flags and colour are copied; the transform rows are
// -(Forward x Up) * (Size.X / 100), -Forward * (Size.Y / 100), Up * (Size.Z / 100) and the world position.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const V3& operator*=(float v) { X *= v; Y *= v; Z *= v; return *this; } };
struct Set { V3 Up, Forward;
    V3 GetRight() const { return V3((Forward.Y * Up.Z - Forward.Z * Up.Y), (Forward.Z * Up.X - Forward.X * Up.Z), (Forward.X * Up.Y - Forward.Y * Up.X)); } };
struct Desc { V3 WorldPos, Size; Set RightHandedSet; unsigned long Colour; };
struct Prim { unsigned long EnginePrimitiveFlags; float E[12]; unsigned long Colour;
    void Update(const Desc& mesh, unsigned long flags)
    {
        EnginePrimitiveFlags = flags;
        Colour = mesh.Colour;
        float scale_x = mesh.Size.X * 0.01f, scale_y = mesh.Size.Y * 0.01f, scale_z = mesh.Size.Z * 0.01f;
        V3 forward = mesh.RightHandedSet.Forward;
        V3 up = mesh.RightHandedSet.Up;
        V3 right = mesh.RightHandedSet.GetRight();
        right *= scale_x; forward *= scale_y; up *= scale_z;
        E[0] = -right.X; E[1] = -right.Y; E[2] = -right.Z;
        E[3] = -forward.X; E[4] = -forward.Y; E[5] = -forward.Z;
        E[6] = up.X; E[7] = up.Y; E[8] = up.Z;
        E[9] = mesh.WorldPos.X; E[10] = mesh.WorldPos.Y; E[11] = mesh.WorldPos.Z;
    } };
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main()
{
    Desc d; d.WorldPos = V3(1, 2, 3); d.Size = V3(200, 50, 300);
    d.RightHandedSet.Up = V3(0, 0, 1); d.RightHandedSet.Forward = V3(0, 1, 0); d.Colour = 0x80402010;
    Prim p; p.Update(d, 0x55);
    // Forward x Up = (1, 0, 0): row 1 = -(1,0,0)*2, row 2 = -(0,1,0)*0.5, row 3 = (0,0,1)*3
    static const float expect[12] = { -2, 0, 0,  0, -0.5f, 0,  0, 0, 3,  1, 2, 3 };
    bool ok = p.EnginePrimitiveFlags == 0x55 && p.Colour == 0x80402010;
    for (int i = 0; i < 12; ++i) ok = ok && Near(p.E[i], expect[i]);
    d.RightHandedSet.Up = V3(0, 1, 0); d.RightHandedSet.Forward = V3(1, 0, 0); d.Size = V3(100, 100, 100);
    p.Update(d, 1);                              // Forward x Up = (0, 0, 1)
    ok = ok && Near(p.E[2], -1) && Near(p.E[3], -1) && Near(p.E[7], 1) && Near(p.E[0], 0) && p.EnginePrimitiveFlags == 1;
    if (!ok) { std::printf("00bc2480_TEST FAIL\n"); return 1; }
    std::printf("00bc2480_TEST PASS\n");
    return 0;
}
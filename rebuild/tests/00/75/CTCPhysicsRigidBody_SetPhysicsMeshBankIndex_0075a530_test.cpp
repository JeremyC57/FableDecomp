// Behaviour test for 0x0075A530 CTCPhysicsRigidBody::SetPhysicsMeshBankIndex (see src/0075a530.cpp): the mesh info takes the
// render scale, the box centre scaled by scale/100, and the bounding sphere / circle radii scaled the same way; the 2D offset
// is the centre's X and Y.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; };
struct Info { float RenderSizeX; V3 ToCentre; float BoundingRadius, BoundingCircleRadius; };
static void Fill(Info& info, float scale, V3 mn, V3 mx, float sphere, float circle, float offset[2])
{
    info.RenderSizeX = scale;
    V3 c = { (mn.X + mx.X) * 0.5f, (mn.Y + mx.Y) * 0.5f, (mn.Z + mx.Z) * 0.5f };
    float k = info.RenderSizeX * 0.01f;
    c.X *= k; c.Y *= k; c.Z *= k;
    info.ToCentre = c;
    info.BoundingRadius = info.RenderSizeX * sphere * 0.01f;
    info.BoundingCircleRadius = info.RenderSizeX * circle * 0.01f;
    offset[0] = info.ToCentre.X; offset[1] = info.ToCentre.Y;
}
int main()
{
    Info info; float off[2];
    V3 mn = { -2.0f, 0.0f, 2.0f }, mx = { 6.0f, 4.0f, 4.0f };
    Fill(info, 200.0f, mn, mx, 10.0f, 5.0f, off);
    if (std::fabs(info.ToCentre.X - 4.0f) > 1e-4f || std::fabs(info.ToCentre.Z - 6.0f) > 1e-4f) { std::printf("0075a530_TEST FAIL\n"); return 1; }
    if (std::fabs(info.BoundingRadius - 20.0f) > 1e-4f || std::fabs(info.BoundingCircleRadius - 10.0f) > 1e-4f) { std::printf("0075a530_TEST FAIL\n"); return 1; }
    if (off[0] != info.ToCentre.X || off[1] != info.ToCentre.Y) { std::printf("0075a530_TEST FAIL\n"); return 1; }
    std::printf("0075a530_TEST PASS\n");
    return 0;
}
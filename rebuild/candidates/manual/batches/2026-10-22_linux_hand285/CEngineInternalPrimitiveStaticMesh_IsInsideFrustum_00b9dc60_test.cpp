#include <cstdio>
// Model of the static-mesh frustum test: world centre = local centre through a 3x4 transform; reject beyond the fade
// distance; optional skip of the frustum part; then near/far depth along the look vector and four side planes vs radius.
struct V { float X, Y, Z; };
static V Sub(const V& a, const V& b) { V r; r.X = a.X - b.X; r.Y = a.Y - b.Y; r.Z = a.Z - b.Z; return r; }
static float Dot(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct Cam { V Pos; float Near, Far; V Look; V Planes[4]; };
struct Mesh { V Centre; float Radius; float Scale; float M[12]; float Fade; };
static bool IsInside(const Mesh& mesh, const Cam& cam, bool ignoreFrustum)
{
    const float* m = mesh.M;
    V c; c.X = mesh.Centre.X * m[0] + mesh.Centre.Y * m[3] + mesh.Centre.Z * m[6] + m[9];
    c.Y = mesh.Centre.X * m[1] + mesh.Centre.Y * m[4] + mesh.Centre.Z * m[7] + m[10];
    c.Z = mesh.Centre.X * m[2] + mesh.Centre.Y * m[5] + mesh.Centre.Z * m[8] + m[11];
    V d = Sub(c, cam.Pos);
    if (Dot(d, d) > mesh.Fade * mesh.Fade) return false;
    if (ignoreFrustum) return true;
    float r = (float)((double)mesh.Radius * mesh.Scale);
    float depth = Dot(d, cam.Look);
    if (depth + r < cam.Near || depth - r > cam.Far) return false;
    static const int order[4] = { 2, 3, 0, 1 };
    for (int i = 0; i < 4; ++i) if (Dot(d, cam.Planes[order[i]]) > r) return false;
    return true;
}
int main()
{
    int bad = 0;
    Mesh mesh = { { 0, 0, 0 }, 1.0f, 2.0f, { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 10 }, 100.0f };
    Cam cam;
    cam.Pos.X = 0; cam.Pos.Y = 0; cam.Pos.Z = 0; cam.Near = 1.0f; cam.Far = 50.0f;
    cam.Look.X = 0; cam.Look.Y = 0; cam.Look.Z = 1;
    V p0 = { 1, 0, -1 }, p1 = { -1, 0, -1 }, p2 = { 0, 1, -1 }, p3 = { 0, -1, -1 };
    cam.Planes[0] = p0; cam.Planes[1] = p1; cam.Planes[2] = p2; cam.Planes[3] = p3;
    if (!IsInside(mesh, cam, false)) bad = 1;               // centre (0,0,10), radius 2: visible
    mesh.M[9] = 20.0f;                                       // centre (20,0,10): outside the x plane
    if (IsInside(mesh, cam, false)) bad = 1;
    if (!IsInside(mesh, cam, true)) bad = 1;                 // frustum skipped
    mesh.Fade = 5.0f;                                        // beyond the fade distance even when skipping
    if (IsInside(mesh, cam, true)) bad = 1;
    mesh.Fade = 100.0f; mesh.M[9] = 0.0f; mesh.M[11] = 60.0f;   // beyond far
    if (IsInside(mesh, cam, false)) bad = 1;
    mesh.M[11] = -5.0f;                                      // behind near
    if (IsInside(mesh, cam, false)) bad = 1;
    if (bad) { std::printf("00b9dc60_TEST FAIL\n"); return 1; }
    std::printf("00b9dc60_TEST PASS\n");
    return 0;
}

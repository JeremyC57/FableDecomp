// behaviour test for 0x00B9DFC0 CEngineInternalPrimitive2DStaticMesh::Update
extern "C" int printf(const char*, ...);
static int g_fail;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
struct V3 { float X, Y, Z; };
struct Sphere { V3 Center; float Radius; };
struct Box { float A, B, C, D; };
struct Colour { unsigned char B, G, R, A; };
static int g_perp;
struct RHSet { V3 Up, Forward; void MakePerpendicular() { ++g_perp; } };
struct OrientInterp { RHSet Start, End; };
struct MeshInfo { Sphere BoundingSphere; Sphere GetBoundingSphere() const { return BoundingSphere; } };
static MeshInfo g_meshes[4];
static unsigned long g_lastIndex;
struct MeshBank { const MeshInfo* PeekStaticMesh(unsigned long i) const { g_lastIndex = i; return &g_meshes[i]; } };
static MeshBank g_bank;
struct Desc { long InterpolationSet; unsigned long MeshIndex; Box ClipBox; unsigned char Alpha; OrientInterp Orientation; Sphere BoundingSphere; Colour Tint; };
struct Prim {
    unsigned long Flags; unsigned char Alpha; long InterpolationSet; const MeshInfo* Mesh; Colour Tint;
    bool Initialised; Box ClipBox; long Layer; Sphere BoundingSphere; OrientInterp Orientation;
    void Update(const Desc& d, unsigned long flags)
    {
        Flags = flags;
        Mesh = g_bank.PeekStaticMesh(d.MeshIndex);
        Alpha = d.Alpha; Tint = d.Tint; ClipBox = d.ClipBox; Layer = 0x48; InterpolationSet = d.InterpolationSet;
        Orientation = d.Orientation;
        Orientation.Start.MakePerpendicular();
        Orientation.End.MakePerpendicular();
        if (flags & 0x80) Flags |= 0x80;
        if (d.BoundingSphere.Radius > 0.0f) {
            BoundingSphere.Center.X = d.BoundingSphere.Center.X * 100.0f;
            BoundingSphere.Center.Y = d.BoundingSphere.Center.Y * 100.0f;
            BoundingSphere.Center.Z = d.BoundingSphere.Center.Z * 100.0f;
            BoundingSphere.Radius = d.BoundingSphere.Radius * 100.0f;
        } else
            BoundingSphere = Mesh->GetBoundingSphere();
        Initialised = false;
    }
};
int main()
{
    Desc d = { 3, 2, { 1, 2, 3, 4 }, 0x7F, { { { 0, 1, 0 }, { 0, 0, 1 } }, { { 0, 1, 0 }, { 1, 0, 0 } } }, { { 1, 2, 3 }, 0.5f }, { 1, 2, 3, 4 } };
    g_meshes[2].BoundingSphere.Radius = 7.0f; g_meshes[2].BoundingSphere.Center.X = 9.0f;
    Prim p; p.Initialised = true;
    p.Update(d, 0x81);
    CHECK(g_lastIndex == 2 && p.Mesh == &g_meshes[2]);
    CHECK(p.Flags == 0x81 && p.Alpha == 0x7F && p.Layer == 0x48 && p.InterpolationSet == 3);
    CHECK(p.ClipBox.D == 4 && p.Tint.A == 4 && g_perp == 2 && !p.Initialised);
    CHECK(p.BoundingSphere.Radius == 50.0f && p.BoundingSphere.Center.Z == 300.0f);
    d.BoundingSphere.Radius = 0.0f;
    p.Update(d, 1);
    CHECK(p.BoundingSphere.Radius == 7.0f && p.BoundingSphere.Center.X == 9.0f && p.Flags == 1 && g_perp == 4);
    if (g_fail == 0) printf("00b9dfc0_TEST PASS\n");
    return g_fail;
}
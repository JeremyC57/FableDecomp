// Behaviour test for 0x00BD2BC0 CEngineInternalPrimitive2DAnimatedMesh::Update(const CEnginePrimitive&, unsigned long)
#include <stdio.h>

struct Vec3 { float X, Y, Z; };
struct Box { float TLX, TLY, BRX, BRY; };
struct Sphere { Vec3 Center; float Radius; };
struct Desc {                                   // CEnginePrimitive2DAnimatedMesh
    int InterpolationSet; Box ClipBox; unsigned int MeshIndex; unsigned char Alpha; int Orientation;
    int Anim; Sphere ObjectBoundingSphere; unsigned int TintColour;
};
static int g_meshes[4];
static int g_perpendicularCalls = 0;
static int g_initialiseCalls = 0;
struct Internal {
    unsigned int EnginePrimitiveFlags; unsigned char Alpha; int InterpolationSet; const int* MeshInfo;
    unsigned int TintColour; bool BulletTimeAffectsCloth; int Anim; int OrientationStart, OrientationEnd;
    int CacheEntries; bool Initialised; int Layer; Box ClipBox; Sphere ObjectBoundingSphere;
    void Initialise() { ++g_initialiseCalls; Initialised = true; }
    static void MakePerpendicular(int& set) { ++g_perpendicularCalls; set |= 0x100; }
    void Update(const Desc& mesh, unsigned int flags)             // the row's body
    {
        MeshInfo = &g_meshes[mesh.MeshIndex];
        ClipBox = mesh.ClipBox;
        OrientationStart = OrientationEnd = mesh.Orientation;
        InterpolationSet = mesh.InterpolationSet;
        BulletTimeAffectsCloth = false;
        MakePerpendicular(OrientationStart);
        MakePerpendicular(OrientationEnd);
        Alpha = mesh.Alpha;
        TintColour = mesh.TintColour;
        Anim = mesh.Anim;
        Layer = 0x48;
        ObjectBoundingSphere.Center.X = mesh.ObjectBoundingSphere.Center.X * 100.0f;
        ObjectBoundingSphere.Center.Y = mesh.ObjectBoundingSphere.Center.Y * 100.0f;
        ObjectBoundingSphere.Center.Z = mesh.ObjectBoundingSphere.Center.Z * 100.0f;
        ObjectBoundingSphere.Radius = mesh.ObjectBoundingSphere.Radius * 100.0f;
        if (flags & 0x80)
            EnginePrimitiveFlags |= 0x80;
        CacheEntries = 0;
        Initialise();
        Initialised = false;
    }
};

int main()
{
    Desc d = { 2, { 1, 2, 3, 4 }, 1, 77, 5, 6, { { 0.5f, 1.0f, 2.0f }, 0.25f }, 0x01020304u };
    Internal m; m.EnginePrimitiveFlags = 1; m.CacheEntries = 3; m.BulletTimeAffectsCloth = true; m.Initialised = true;
    m.Update(d, 0x81u);
    int ok = 1;
    ok &= m.MeshInfo == &g_meshes[1] && m.ClipBox.BRY == 4.0f && m.InterpolationSet == 2;
    ok &= g_perpendicularCalls == 2 && m.OrientationStart == (5 | 0x100) && m.OrientationEnd == (5 | 0x100);
    ok &= !m.BulletTimeAffectsCloth && m.Alpha == 77 && m.TintColour == 0x01020304u && m.Anim == 6 && m.Layer == 0x48;
    ok &= m.ObjectBoundingSphere.Center.Z == 200.0f && m.ObjectBoundingSphere.Radius == 25.0f;
    ok &= m.EnginePrimitiveFlags == 0x81u && m.CacheEntries == 0 && g_initialiseCalls == 1 && !m.Initialised;
    m.EnginePrimitiveFlags = 1;
    m.Update(d, 0x01u);
    ok &= m.EnginePrimitiveFlags == 1u;
    printf(ok ? "00bd2bc0_TEST PASS\n" : "00bd2bc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
// Behaviour test for 0x00BD2920 CEngineInternalPrimitiveAnimatedMesh::Update(const CEnginePrimitive&, unsigned long)
#include <stdio.h>

struct Vec3 { float X, Y, Z; };
struct Desc {                                   // CEnginePrimitiveAnimatedMesh
    unsigned int MeshIndex; float Size; float FadeStart; unsigned char Alpha;
    Vec3 StartPosition, EndPosition; int Orientation; int Anim; int MeshEffect;
    unsigned int ThemeChannelDesc; unsigned int TintColour; bool BulletTimeAffectsCloth;
};
static int g_meshes[8];
static float g_fadeSeen = -1.0f;
struct Internal {
    unsigned int EnginePrimitiveFlags; unsigned char Alpha; bool OverrideFadeDistance;
    const int* MeshInfo; float Scale; unsigned int TintColour; bool BulletTimeAffectsCloth;
    int MeshEffect; unsigned int ThemeChannelDesc; int Anim; Vec3 StartPosition, EndPosition; int Orientation;
    int CacheEntries;
    void SetFadeDistanceWithGamePrimitiveDefaults(float f) { g_fadeSeen = f; }
    void Update(const Desc& mesh, unsigned int flags)             // the row's body
    {
        MeshInfo = &g_meshes[mesh.MeshIndex];                     // PeekMeshBank(0)->PeekAnimatedMesh(index)
        Scale = mesh.Size * 0.01f;
        StartPosition = mesh.StartPosition;
        EndPosition = mesh.EndPosition;
        Orientation = mesh.Orientation;
        EnginePrimitiveFlags = flags;
        ThemeChannelDesc = mesh.ThemeChannelDesc;
        TintColour = mesh.TintColour;
        BulletTimeAffectsCloth = mesh.BulletTimeAffectsCloth;
        CacheEntries = 0;                                          // Cache = CCache()
        Alpha = mesh.Alpha;
        MeshEffect = mesh.MeshEffect;
        Anim = mesh.Anim;
        SetFadeDistanceWithGamePrimitiveDefaults(mesh.FadeStart);
        OverrideFadeDistance = true;
    }
};

int main()
{
    Desc d = { 3, 250.0f, 40.0f, 200, { 1, 2, 3 }, { 4, 5, 6 }, 7, 8, 9, 0x11u, 0xff102030u, true };
    Internal m; m.CacheEntries = 5; m.OverrideFadeDistance = false; m.EnginePrimitiveFlags = 0;
    m.Update(d, 0x80u);
    int ok = 1;
    ok &= m.MeshInfo == &g_meshes[3];
    ok &= m.Scale > 2.49f && m.Scale < 2.51f;
    ok &= m.StartPosition.Z == 3.0f && m.EndPosition.X == 4.0f && m.Orientation == 7;
    ok &= m.EnginePrimitiveFlags == 0x80u && m.ThemeChannelDesc == 0x11u && m.TintColour == 0xff102030u;
    ok &= m.BulletTimeAffectsCloth && m.CacheEntries == 0 && m.Alpha == 200 && m.MeshEffect == 9 && m.Anim == 8;
    ok &= g_fadeSeen == 40.0f && m.OverrideFadeDistance;
    printf(ok ? "00bd2920_TEST PASS\n" : "00bd2920_TEST FAIL\n");
    return ok ? 0 : 1;
}
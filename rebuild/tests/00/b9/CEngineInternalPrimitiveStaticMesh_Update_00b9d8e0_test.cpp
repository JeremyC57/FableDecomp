#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };
class CRGBColour { public: unsigned char B, G, R, A; };
static int g_destroys = 0; static float g_matrixScale = 0.0f; static float g_fadeStart = 0.0f;
class CEffect { public: long RefCount; virtual ~CEffect() { } virtual void Destroy() { ++g_destroys; } void Release() { if (--RefCount == 0) Destroy(); } };
class CIVCountedPointer {
public:
    CEffect* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CEffect* old = Object;
        if (old != other.Object) { if (old) { old->Release(); Object = 0; } Object = other.Object; if (Object) ++Object->RefCount; }
        return *this;
    }
};
struct CMeshInfo { int id; };
static CMeshInfo g_meshes[4];
struct CMeshBank { CMeshInfo* GetMesh(unsigned int i) { return &g_meshes[i]; } };
struct SRes { CMeshBank* MeshBank; CMeshBank* GetMeshBank() const { return MeshBank; } };
static CMeshBank g_bank; static SRes g_res; static SRes* g_pEngineResources = &g_res;
struct CDesc { unsigned int MeshIndex; C3DVector WorldPos; float Size; float FadeStart; CRightHandedSet RightHandedSet; CIVCountedPointer MeshEffect;
               unsigned int ThemeChannelDesc; CRGBColour TintColour; unsigned char Alpha; bool BulletTimeAffectsCloth; };
struct CMesh {
    unsigned int EnginePrimitiveFlags; unsigned char Alpha; unsigned char Enable : 1; unsigned char NonPersistent : 1; unsigned char OverrideFadeDistance : 1;
    CMeshInfo* MeshInfo; CMeshInfo* ShadowMeshInfo; float Scale; CRGBColour TintColour; bool BulletTimeAffectsCloth; CIVCountedPointer MeshEffect; unsigned int ThemeChannelDesc;
    void SetFadeDistanceWithGamePrimitiveDefaults(float f) { g_fadeStart = f; }
    void Update(const CDesc& mesh, unsigned int flags)
    {
        EnginePrimitiveFlags = flags;
        ShadowMeshInfo = MeshInfo = g_pEngineResources->GetMeshBank()->GetMesh(mesh.MeshIndex);
        Scale = mesh.Size * 0.01f; Alpha = mesh.Alpha; TintColour = mesh.TintColour; BulletTimeAffectsCloth = mesh.BulletTimeAffectsCloth;
        MeshEffect = mesh.MeshEffect; ThemeChannelDesc = mesh.ThemeChannelDesc;
        g_matrixScale = Scale;                       // CalcObjectMatrix(&ObjectTransform, &WorldPos, &RightHandedSet, Scale)
        SetFadeDistanceWithGamePrimitiveDefaults(mesh.FadeStart);
        OverrideFadeDistance = 1;
    }
};
int main()
{
    g_res.MeshBank = &g_bank;
    CEffect oldE, newE; oldE.RefCount = 1; newE.RefCount = 1;
    CDesc d; d.MeshIndex = 2; d.Size = 250.0f; d.FadeStart = 30.0f; d.MeshEffect.Object = &newE; d.ThemeChannelDesc = 3; d.TintColour.R = 9; d.Alpha = 77; d.BulletTimeAffectsCloth = true;
    CMesh m; m.Enable = 1; m.NonPersistent = 0; m.OverrideFadeDistance = 0; m.MeshEffect.Object = &oldE;
    m.Update(d, 0x21);
    if (m.EnginePrimitiveFlags != 0x21 || m.MeshInfo != &g_meshes[2] || m.ShadowMeshInfo != &g_meshes[2] || m.Scale != 2.5f || g_matrixScale != 2.5f
        || m.Alpha != 77 || m.TintColour.R != 9 || !m.BulletTimeAffectsCloth || m.MeshEffect.Object != &newE || newE.RefCount != 2 || g_destroys != 1
        || m.ThemeChannelDesc != 3 || g_fadeStart != 30.0f || !m.OverrideFadeDistance) { std::printf("00b9d8e0_TEST FAIL\n"); return 1; }
    std::printf("00b9d8e0_TEST PASS\n");
    return 0;
}
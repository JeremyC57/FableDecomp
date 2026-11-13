// Behaviour test for 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette(CEngineInternalPrimitiveStaticMesh&): a host model
// of the row's logic -- the two early-outs (visibility ratios invalid, no silhouette colour), the world-space centre (object transform of
// the shadow mesh's sphere centre), camera distance, LOD choice from the screen-space error, the mesh fetch (failure returns), the render
// info (mesh, LOD, lights), the stencil (ShowSilhouettes == 1) or z-buffer render states, the stencil pass draw and the alpha box.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
struct Matrix { float E[12]; };      // E11 E12 E13 / E21 E22 E23 / E31 E32 E33 / E41 E42 E43
struct MeshInfo { Vec Centre; float Radius; float LODErrors[4]; unsigned long LODDataSizes[4]; int MeshForLOD[4]; };
struct Primitive { const MeshInfo* Info; const MeshInfo* ShadowInfo; float Scale; Matrix Transform; int Mesh; unsigned long LOD; int Lights; };
struct States { int StencilEnable, StencilOp, StencilRef, StencilWriteMask, ColourWrite, ZEnable, ZWrite, StencilFunc, StencilMask; };

static bool g_RatiosValid; static bool g_HaveColour; static long g_ShowSilhouettes;
static Vec g_Camera; static float g_Tolerance = 2.0f, g_FOV = 0.5f;
static States g_S; static int g_DrawImmediately, g_DrawBox; static unsigned long g_BoxColour; static int g_Restores;

static void Transform(const Matrix& m, Vec& p)
{
    float x = p.X, y = p.Y, z = p.Z;
    p.X = m.E[0] * x + (m.E[3] * y + m.E[6] * z) + m.E[9];
    p.Y = m.E[1] * x + (m.E[4] * y + m.E[7] * z) + m.E[10];
    p.Z = m.E[2] * x + (m.E[5] * y + m.E[8] * z) + m.E[11];
}
static bool HasLOD(const MeshInfo& m, unsigned long lod) { return lod < 4 && m.LODDataSizes[lod] != 0; }

static void DrawSilhouette(Primitive& primitive)
{
    ++g_Restores;                                    // the scoped capture block restores at every exit
    if (!g_RatiosValid) return;
    unsigned long colour = 0xff102030;
    if (!g_HaveColour) return;
    Vec centre = primitive.ShadowInfo->Centre;
    Transform(primitive.Transform, centre);
    float dx = centre.X - g_Camera.X, dy = centre.Y - g_Camera.Y, dz = centre.Z - g_Camera.Z;
    float squared = dx * dx; squared += dz * dz; squared += dy * dy;
    float distance = (float)std::sqrt(squared);
    float radius = primitive.Info->Radius * primitive.Scale;
    float nearest = distance - radius;
    if (!(nearest > 1e-4f)) nearest = 1e-4f;
    float lod_error = nearest / (radius * g_FOV) * g_Tolerance;
    unsigned long lod = 0;
    while (HasLOD(*primitive.Info, lod + 1) && !(primitive.Info->LODErrors[lod + 1] > lod_error)) ++lod;
    int mesh = primitive.Info->MeshForLOD[lod];
    if (!mesh) return;
    primitive.Mesh = mesh; primitive.LOD = lod; primitive.Lights = 7;
    if (g_ShowSilhouettes == 1) {
        g_S.StencilEnable = 1; g_S.StencilOp = 2; g_S.StencilRef = 1; g_S.StencilWriteMask = 1; g_S.ColourWrite = 0; g_S.ZEnable = 0;
        ++g_DrawImmediately;
        g_S.ColourWrite = 15; g_S.StencilFunc = 2; g_S.StencilMask = 1; g_S.StencilOp = 1;
    } else {
        g_S.ZEnable = 1; g_S.ZWrite = 0;
    }
    ++g_DrawBox; g_BoxColour = colour;
}

int main()
{
    bool ok = true;
    MeshInfo info = { { 0, 0, 0 }, 1.0f, { 0.0f, 1.0f, 4.0f, 100.0f }, { 1, 1, 1, 0 }, { 11, 12, 13, 0 } };
    Primitive p = { &info, &info, 2.0f, { { 1,0,0, 0,1,0, 0,0,1, 10,0,0 } }, 0, 0, 0 };
    g_Camera.X = 0; g_Camera.Y = 0; g_Camera.Z = 0;
    // invalid ratios: nothing but the capture block
    g_RatiosValid = false; g_HaveColour = true; g_ShowSilhouettes = 1;
    DrawSilhouette(p);
    ok = ok && g_Restores == 1 && g_DrawBox == 0 && p.Mesh == 0;
    // no colour: early out
    g_RatiosValid = true; g_HaveColour = false;
    DrawSilhouette(p);
    ok = ok && g_Restores == 2 && g_DrawBox == 0;
    // stencil silhouette: distance 10, radius 2 -> nearest 8, error 8 / (2 * 0.5) * 2 = 16 -> LOD 2 (LOD 3 has no data)
    g_HaveColour = true;
    DrawSilhouette(p);
    ok = ok && p.Mesh == 13 && p.LOD == 2 && p.Lights == 7 && g_DrawImmediately == 1 && g_DrawBox == 1 && g_BoxColour == 0xff102030u;
    ok = ok && g_S.StencilOp == 1 && g_S.ColourWrite == 15 && g_S.ZEnable == 0 && g_S.StencilFunc == 2;
    // z-buffer path, near camera: nearest clamps to the epsilon -> LOD 0
    g_ShowSilhouettes = 0; p.Transform.E[9] = 0.5f;
    DrawSilhouette(p);
    ok = ok && p.LOD == 0 && p.Mesh == 11 && g_S.ZEnable == 1 && g_S.ZWrite == 0 && g_DrawImmediately == 1 && g_DrawBox == 2;
    if (!ok) { std::printf("00ba6ff0_TEST FAIL\n"); return 1; }
    std::printf("00ba6ff0_TEST PASS\n");
    return 0;
}

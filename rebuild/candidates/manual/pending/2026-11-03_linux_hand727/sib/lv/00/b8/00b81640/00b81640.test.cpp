#include <stdio.h>
#include <math.h>
// Behaviour model of the cloth-simulation Render loop: one fade / position-buffer / prepare / render round per simulation,
// the centre transformed by the matrix, the buffer sized by the program's vertex position count.
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    C3DVector operator*(const C3DVector& v) const
    {
        C3DVector r = { v.X * E11 + v.Y * E21 + v.Z * E31 + E41, v.X * E12 + v.Y * E22 + v.Z * E32 + E42, v.X * E13 + v.Y * E23 + v.Z * E33 + E43 };
        return r;
    }
};
struct Program { unsigned long Size, NonSimCount; unsigned long GetVertexPositionCount() const { return Size + NonSimCount; } };
struct ClothPrimitive { Program* ParticleProgram; };
static C3DVector g_lastCentre;
static unsigned long g_bufferSizes[4];
static int g_renders, g_prepares, g_buffers;
static float g_lastFade, g_lastBlend;
static float CalculateFadeOut(const C3DVector& p) { g_lastCentre = p; return p.X > 10.0f ? 1.0f : 0.0f; }
static float* GetPositionBuffer(unsigned long size) { static float buffer[64]; g_bufferSizes[g_buffers++] = size; return buffer; }
static void PrepareForRendering(float*, float blend, float fade) { g_lastBlend = blend; g_lastFade = fade; ++g_prepares; }
static void RenderClothPrimitive(const ClothPrimitive&) { ++g_renders; }
static void Render(unsigned long count, ClothPrimitive* prims, const C3DVector& centre, const CMatrix3x4& transform, float blend)
{
    for (unsigned long i = 0; i < count; ++i) {
        const ClothPrimitive* clothPrimitive = &prims[i];
        float fade = CalculateFadeOut(transform * centre);
        Program* program = clothPrimitive->ParticleProgram;
        float* buffer = GetPositionBuffer(program->GetVertexPositionCount());
        PrepareForRendering(buffer, blend, fade);
        RenderClothPrimitive(*clothPrimitive);
    }
}
int main()
{
    Program p1 = { 10, 3 }, p2 = { 4, 0 };
    ClothPrimitive prims[2] = { { &p1 }, { &p2 } };
    CMatrix3x4 m = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 20, 0, 0 };   // translate by (20, 0, 0)
    C3DVector c = { 1, 2, 3 };
    Render(2, prims, c, m, 0.25f);
    int ok = g_renders == 2 && g_prepares == 2 && g_bufferSizes[0] == 13 && g_bufferSizes[1] == 4
        && g_lastCentre.X == 21.0f && g_lastCentre.Y == 2.0f && g_lastCentre.Z == 3.0f && g_lastFade == 1.0f && g_lastBlend == 0.25f;
    Render(0, prims, c, m, 0.5f);
    ok = ok && g_renders == 2;
    if (!ok) { printf("00b81640_TEST FAIL\n"); return 1; }
    printf("00b81640_TEST PASS\n");
    return 0;
}
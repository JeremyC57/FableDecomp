// behaviour test for 0x00B812F0 CEngineClothSimulationStaticMesh::Render
extern "C" int printf(const char*, ...);
static int g_fail;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
struct V3 { float X, Y, Z; };
struct Matrix { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct Sim { int Id; };
struct Program { long Count; long GetVertexPositionCount() const { return Count; } };
struct ClothPrim { Program* P; };
static float g_lastFadeX; static long g_bufferCount, g_prepares, g_renders; static float g_lastFade;
static float g_buffer[16];
static float CalculateFadeOut(const V3& pos) { g_lastFadeX = pos.X; return pos.X * 0.5f; }
static float* GetPositionBuffer(long n) { g_bufferCount = n; return g_buffer; }
static void PrepareForRendering(float* buf, Sim&, float, float fade) { ++g_prepares; g_lastFade = fade; CHECK(buf == g_buffer); }
static void RenderClothPrimitive(const Sim& s, const ClothPrim&, bool) { ++g_renders; CHECK(s.Id == (int)g_renders); }
struct Owner { Matrix ObjectTransform; };
static void Render(ClothPrim* prims, Sim* sims, unsigned long count, Owner* owner, const V3& centre)
{
    for (unsigned long i = 0; i < count; ++i) {
        const ClothPrim* cloth = &prims[i];
        Sim* sim = &sims[i];
        float fade;
        V3 p;
        p.X = centre.X * owner->ObjectTransform.E11 + centre.Y * owner->ObjectTransform.E21 + centre.Z * owner->ObjectTransform.E31 + owner->ObjectTransform.E41;
        p.Y = centre.X * owner->ObjectTransform.E12 + centre.Y * owner->ObjectTransform.E22 + centre.Z * owner->ObjectTransform.E32 + owner->ObjectTransform.E42;
        p.Z = centre.X * owner->ObjectTransform.E13 + centre.Y * owner->ObjectTransform.E23 + centre.Z * owner->ObjectTransform.E33 + owner->ObjectTransform.E43;
        fade = CalculateFadeOut(p);
        float* buffer = GetPositionBuffer(cloth->P->GetVertexPositionCount());
        PrepareForRendering(buffer, *sim, 1.0f, fade);
        RenderClothPrimitive(*sim, *cloth, true);
    }
}
int main()
{
    Program prog = { 12 };
    ClothPrim prims[2] = { { &prog }, { &prog } };
    Sim sims[2] = { { 1 }, { 2 } };
    Owner o = { { 2, 0, 0, 0, 1, 0, 0, 0, 1, 10, 20, 30 } };
    V3 c = { 3, 4, 5 };
    Render(prims, sims, 2, &o, c);
    CHECK(g_lastFadeX == 16.0f && g_lastFade == 8.0f && g_bufferCount == 12 && g_prepares == 2 && g_renders == 2);
    if (g_fail == 0) printf("00b812f0_TEST PASS\n");
    return g_fail;
}
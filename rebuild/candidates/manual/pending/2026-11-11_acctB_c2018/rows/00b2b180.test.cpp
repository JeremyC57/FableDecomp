// Behaviour test for 0x00B2B180 CEngineSurfaceCompositionManager::RenderToTexture: a host model of the body (same statement order and
// arithmetic) over recording mocks: shader enable save / restore, constant layout save / restore, fog / specular states, one compositor per
// mip level with one quad per blend layer, the box halving and the final GenerateMipmaps(5, levels - 1).
#include <cstdio>
#include <cmath>

struct Log { int fogSets[8]; int fogCount; int copies; long copyW[16], copyH[16]; int quads; float lastQuad[4][6]; unsigned long lastColour;
             int lastBlend; long genFilter, genLevels; int composites; int ends; bool vsEnabled, psEnabled; long layout; bool released; };
static Log g;

struct Box { long TLX, TLY, BRX, BRY; Box(long a, long b, long c, long d) : TLX(a), TLY(b), BRX(c), BRY(d) { } };
struct BoxF { float TLX, TLY, BRX, BRY; };
struct Vec { float X, Y, Z; Vec(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct Vertex {
    void SetPosition(const Vec& p) { X = p.X; Y = p.Y; Z = p.Z; }
    void SetU1V1(float u, float v) { U1 = u; V1 = v; }
    float X, Y, Z; unsigned long DiffuseColour; float U1, V1;
};
struct Blend { unsigned long TextureIndex; int BlendType; unsigned long DiffuseColour, SpecularColour; };
struct Surface { int level; void CopyFromSurface(const Box* d, const Surface&, const Box* s, unsigned long) {
    if (d == s) { g.copyW[g.copies] = d->BRX; g.copyH[g.copies] = d->BRY; } ++g.copies; } };
struct Texture { Surface GetSurfaceLevel(long l) const { Surface s; s.level = (int)l; return s; }
                 void GenerateMipmaps(unsigned long f, unsigned long n) { g.genFilter = (long)f; g.genLevels = (long)n; } };
struct Compositor {
    Compositor(Surface&) { }
    void StartComposition(bool) { ++g.composites; }
    void EndComposition() { ++g.ends; }
    void CompositeQuad(const Texture&, const Vertex* v, int blend) {
        ++g.quads; g.lastBlend = blend; g.lastColour = v[2].DiffuseColour;
        for (int i = 0; i < 4; ++i) { g.lastQuad[i][0] = v[i].X; g.lastQuad[i][1] = v[i].Y; g.lastQuad[i][2] = v[i].Z; g.lastQuad[i][3] = v[i].U1; g.lastQuad[i][4] = v[i].V1; g.lastQuad[i][5] = 0; }
    }
};
struct ScopedShaders { bool oldV, oldP; ScopedShaders(bool v, bool p) { oldV = g.vsEnabled; oldP = g.psEnabled; g.vsEnabled = v; g.psEnabled = p; }
                       ~ScopedShaders() { g.vsEnabled = oldV; g.psEnabled = oldP; } };
struct ScopedLayout { long old; ScopedLayout(long l) { old = g.layout; g.layout = l; } ~ScopedLayout() { g.layout = old; } };
static void SetFogEnable(bool e) { g.fogSets[g.fogCount++] = e ? 1 : 0; }

static void RenderToTexture(Texture& texture, long extX, long extY, bool alpha, const Blend* blends, unsigned long blendCount,
                            Texture* const* frames, unsigned long minRenderTargetSize)
{
    Texture scratchTexture;
    Surface render_surface = scratchTexture.GetSurfaceLevel(0);
    ScopedShaders shaders_enable(true, false);
    ScopedLayout constant_layout(3);
    SetFogEnable(false);
    Box box(0, 0, extX, extY);
    unsigned long size = extX < extY ? extX : extY;
    long level = 0;
    while (size >= minRenderTargetSize) {
        Surface level_surface = texture.GetSurfaceLevel(level);
        BoxF quad;
        quad.TLX = -1.0f;
        quad.TLY = 1.0f - (float)(box.BRY * 2) / extY;
        quad.BRX = (float)(box.BRX * 2) / extX - 1.0f;
        quad.BRY = 1.0f;
        Compositor compositor(render_surface);
        compositor.StartComposition(alpha);
        unsigned long count = blendCount;
        for (unsigned long i = 0; i < count; ++i) {
            const Blend& blend = blends[i];
            const Texture* layer_texture = frames[i];
            float u = 0.25f / box.BRX;
            float v = 0.25f / box.BRY;
            Vertex vertices[4];
            vertices[0].SetPosition(Vec(quad.TLX, quad.BRY, 0.5f)); vertices[0].SetU1V1(u, v); vertices[0].DiffuseColour = blend.DiffuseColour;
            vertices[1].SetPosition(Vec(quad.BRX, quad.BRY, 0.5f)); vertices[1].SetU1V1(u + 1.0f, v); vertices[1].DiffuseColour = blend.DiffuseColour;
            vertices[2].SetPosition(Vec(quad.BRX, quad.TLY, 0.5f)); vertices[2].SetU1V1(u + 1.0f, v + 1.0f); vertices[2].DiffuseColour = blend.DiffuseColour;
            vertices[3].SetPosition(Vec(quad.TLX, quad.TLY, 0.5f)); vertices[3].SetU1V1(u, v + 1.0f); vertices[3].DiffuseColour = blend.DiffuseColour;
            compositor.CompositeQuad(*layer_texture, vertices, blend.BlendType);
        }
        compositor.EndComposition();
        level_surface.CopyFromSurface(&box, render_surface, &box, (unsigned long)-1);
        box.BRX /= 2;
        box.BRY /= 2;
        size >>= 1;
        ++level;
    }
    SetFogEnable(true);
    texture.GenerateMipmaps(5, level - 1);
    g.released = true;
}

static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    Texture target, layer0, layer1;
    Texture* frames[2] = { &layer0, &layer1 };
    Blend blends[2] = { { 0, 2, 0x11223344UL, 0 }, { 1, 1, 0xAABBCCDDUL, 0 } };
    g.vsEnabled = false; g.psEnabled = true; g.layout = 7;
    RenderToTexture(target, 256, 128, true, blends, 2, frames, 64);
    bool ok = true;
    // min(256,128) = 128 -> levels 128, 64 rendered (2 levels), then 32 < 64 stops
    ok = ok && g.composites == 2 && g.ends == 2 && g.copies == 2 && g.quads == 4;
    ok = ok && g.copyW[0] == 256 && g.copyH[0] == 128 && g.copyW[1] == 128 && g.copyH[1] == 64;
    ok = ok && g.genFilter == 5 && g.genLevels == 1 && g.released;
    ok = ok && g.fogCount == 2 && g.fogSets[0] == 0 && g.fogSets[1] == 1;
    ok = ok && !g.vsEnabled && g.psEnabled && g.layout == 7;
    // last quad: level 1, box 128 x 64 of 256 x 128 -> right = 0, bottom = 0; u = 0.25 / 128, v = 0.25 / 64
    ok = ok && g.lastBlend == 1 && g.lastColour == 0xAABBCCDDUL;
    ok = ok && near(g.lastQuad[0][0], -1.0f) && near(g.lastQuad[0][1], 1.0f) && near(g.lastQuad[0][2], 0.5f);
    ok = ok && near(g.lastQuad[1][0], 0.0f) && near(g.lastQuad[2][1], 0.0f) && near(g.lastQuad[3][0], -1.0f) && near(g.lastQuad[3][1], 0.0f);
    ok = ok && near(g.lastQuad[0][3], 0.25f / 128) && near(g.lastQuad[0][4], 0.25f / 64) && near(g.lastQuad[2][3], 1.0f + 0.25f / 128)
            && near(g.lastQuad[2][4], 1.0f + 0.25f / 64) && near(g.lastQuad[1][4], 0.25f / 64) && near(g.lastQuad[3][3], 0.25f / 128);
    if (!ok) { std::printf("00b2b180_TEST FAIL\n"); return 1; }
    std::printf("00b2b180_TEST PASS\n");
    return 0;
}

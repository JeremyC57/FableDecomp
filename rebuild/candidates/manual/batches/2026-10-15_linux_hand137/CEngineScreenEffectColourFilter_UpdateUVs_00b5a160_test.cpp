// Behaviour test for 0x00B5A160 CEngineScreenEffectColourFilter::UpdateUVs: quarter-texel UVs for a 400x200 back buffer.
#include <cstdio>

struct C2DCoordI { long X, Y; };
struct CDisplayManager { C2DCoordI m_backBufferSize; };
static CDisplayManager g_display;
static CDisplayManager* g_pDisplayManager = &g_display;

struct CTVertexColTex1 { float X, Y, Z; unsigned int Colour; float U, V; };

struct CEngineScreenEffectColourFilter {
    CTVertexColTex1 TextureToBackBufferVerts[4];
    void UpdateUVs()
    {
        C2DCoordI size = g_pDisplayManager->m_backBufferSize;
        float du = 0.25f / (float)size.X;
        float dv = 0.25f / (float)size.Y;
        CTVertexColTex1* verts = TextureToBackBufferVerts;
        verts[0].U = du;
        verts[0].V = 1.0f + dv;
        verts[1].U = du + 1.0f;
        verts[1].V = 1.0f + dv;
        verts[2].U = du + 1.0f;
        verts[2].V = dv;
        verts[3].U = du;
        verts[3].V = dv;
    }
};

int main()
{
    g_display.m_backBufferSize.X = 400;
    g_display.m_backBufferSize.Y = 200;
    CEngineScreenEffectColourFilter f;
    f.UpdateUVs();
    const float du = 0.25f / 400.0f, dv = 0.25f / 200.0f;
    const float wantU[4] = { du, du + 1.0f, du + 1.0f, du };
    const float wantV[4] = { 1.0f + dv, 1.0f + dv, dv, dv };
    for (int i = 0; i < 4; ++i) {
        float eu = f.TextureToBackBufferVerts[i].U - wantU[i], ev = f.TextureToBackBufferVerts[i].V - wantV[i];
        if (eu > 1e-6f || eu < -1e-6f || ev > 1e-6f || ev < -1e-6f) { std::printf("00b5a160_TEST FAIL\n"); return 1; }
    }
    std::printf("00b5a160_TEST PASS\n");
    return 0;
}

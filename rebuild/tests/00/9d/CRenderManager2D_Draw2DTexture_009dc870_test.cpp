#include <cstdio>

// Model of CRenderManager2D::Draw2DTexture: the virtual box is converted to screen space, the four corners (TL, TR,
// BR, BL) carry the ARGB colour, Z 0 and the screen RHW, the UVs are nudged by a hundredth (compressed formats) or a
// hundredth of a texel, the blend mode is 2 / 3 / 1 for flag 2 / flag 1 / neither, and two triangles (0,1,2) and
// (0,2,3) are queued.
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CUVBox { public: float U[4]; float V[4]; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((unsigned long)A << 8) | R) << 8 | G) << 8) | B; }
};
class CTexture { public: bool compressed; unsigned long Width, Height; };
struct CTVertex { float X, Y, Z, RHW; unsigned long Diffuse, Specular; float U, V; };
static const float g_ScreenRHW = 0.5f;
static void GFVBoxToABox(const C2DBoxF* box, C2DBoxF* out)
{
    out->TLX = box->TLX * 2.0f; out->TLY = box->TLY * 2.0f; out->BRX = box->BRX * 2.0f; out->BRY = box->BRY * 2.0f;
}
struct CRenderManager2D {
    CTVertex tris[4][3]; long modes[4]; CTexture* textures[4]; int nTris;
    void Add2DTri(const CTVertex& a, const CTVertex& b, const CTVertex& c, long mode, CTexture* t)
    { tris[nTris][0] = a; tris[nTris][1] = b; tris[nTris][2] = c; modes[nTris] = mode; textures[nTris] = t; ++nTris; }
    bool Draw2DTexture(const C2DBoxF& box, const CUVBox& uvs, const CRGBColour& colour, CTexture& texture, unsigned long flags)
    {
        unsigned long argb = colour.GetARGB();
        C2DBoxF vbox;
        GFVBoxToABox(&box, &vbox);
        CTVertex v[4];
        v[0].X = vbox.TLX; v[0].Y = vbox.TLY;
        v[1].X = vbox.BRX; v[1].Y = vbox.TLY;
        v[2].X = vbox.BRX; v[2].Y = vbox.BRY;
        v[3].X = vbox.TLX; v[3].Y = vbox.BRY;
        float offsetU, offsetV;
        if (texture.compressed) { offsetU = 0.01f; offsetV = 0.01f; }
        else { offsetU = 0.01f / (float)texture.Width; offsetV = 0.01f / (float)texture.Height; }
        for (int i = 0; i < 4; ++i) {
            v[i].Diffuse = argb; v[i].U = offsetU + uvs.U[i]; v[i].V = offsetV + uvs.V[i]; v[i].Z = 0.0f; v[i].RHW = g_ScreenRHW;
        }
        long mode;
        if (flags & 2) mode = 2;
        else if (flags & 1) mode = 3;
        else mode = 1;
        Add2DTri(v[0], v[1], v[2], mode, &texture);
        Add2DTri(v[0], v[2], v[3], mode, &texture);
        return true;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.000001f && d > -0.000001f; }

int main()
{
    CRenderManager2D m; m.nTris = 0;
    C2DBoxF box; box.TLX = 1; box.TLY = 2; box.BRX = 3; box.BRY = 4;
    CUVBox uv; for (int i = 0; i < 4; ++i) { uv.U[i] = 0.25f * i; uv.V[i] = 1.0f - 0.25f * i; }
    CRGBColour c; c.B = 0x11; c.G = 0x22; c.R = 0x33; c.A = 0x44;
    CTexture t; t.compressed = false; t.Width = 100; t.Height = 50;
    if (!m.Draw2DTexture(box, uv, c, t, 1)) { std::printf("009dc870_TEST FAIL\n"); return 1; }
    const CTVertex* a = m.tris[0];
    const CTVertex* b = m.tris[1];
    if (m.nTris != 2 || m.modes[0] != 3 || m.modes[1] != 3 || m.textures[0] != &t) { std::printf("009dc870_TEST FAIL\n"); return 1; }
    if (a[0].X != 2 || a[0].Y != 4 || a[1].X != 6 || a[1].Y != 4 || a[2].X != 6 || a[2].Y != 8 || b[2].X != 2 || b[2].Y != 8 || b[1].X != 6)
        { std::printf("009dc870_TEST FAIL\n"); return 1; }
    if (a[0].Diffuse != 0x44332211u || a[1].Z != 0.0f || a[2].RHW != 0.5f || !Near(a[1].U, 0.25f + 0.0001f) || !Near(a[1].V, 0.75f + 0.0002f) || !Near(b[2].U, 0.75f + 0.0001f))
        { std::printf("009dc870_TEST FAIL\n"); return 1; }
    t.compressed = true; m.nTris = 0;
    m.Draw2DTexture(box, uv, c, t, 3);
    if (m.modes[0] != 2 || !Near(m.tris[0][0].U, 0.01f) || !Near(m.tris[0][0].V, 1.01f)) { std::printf("009dc870_TEST FAIL\n"); return 1; }
    m.nTris = 0; m.Draw2DTexture(box, uv, c, t, 0);
    if (m.modes[1] != 1) { std::printf("009dc870_TEST FAIL\n"); return 1; }
    std::printf("009dc870_TEST PASS\n");
    return 0;
}
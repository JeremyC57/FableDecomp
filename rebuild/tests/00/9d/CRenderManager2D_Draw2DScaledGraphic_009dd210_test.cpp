#include <cstdio>

// Model of CRenderManager2D::Draw2DScaledGraphic: the frame is stretched over the converted box, all corners white. The frame's quad UVs are nudged by a hundredth of a texel, read
// through the flip order (normal, flag 0x10 = {1,0,3,2}, flag 0x20 = {3,2,1,0}), Z is 0 so RHW is the screen RHW, and
// the blend mode is 3 (flag 1), 2 (flag 2), 4 (frame flag 2 or flag 4), else 3 / 1 by frame flag 1; two triangles
// (0,1,2) and (0,2,3) are queued. A frame the bank cannot produce queues nothing and returns false.
struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CTVertex { float X, Y, Z, RHW; unsigned long Diffuse, Specular; float U, V; };
struct CTextureQuadUVs { float U[4]; float V[4]; };
struct CTexture { long W, H; };
struct CFrameData { CTextureQuadUVs quad; CTexture* tex; unsigned char m_flags; };
struct CBank {
    bool has; CFrameData data; unsigned long w, h;
    unsigned long GetFrameWidth(unsigned long, unsigned long) const { return w; }
    unsigned long GetFrameHeight(unsigned long, unsigned long) const { return h; }
};
static const float g_ScreenRHW = 0.25f;
static const unsigned long s_CornerOrderNormal[4] = { 0, 1, 2, 3 };
static const unsigned long s_CornerOrderFlipX[4]  = { 1, 0, 3, 2 };
static const unsigned long s_CornerOrderFlipY[4]  = { 3, 2, 1, 0 };
static float GFAXToVX_009e1e10(float x) { return x * 2.0f; }
static float GFAYToVY(float y) { return y * 4.0f; }
static void SetRHWForZ(CTVertex& vert)
{
    if (vert.Z >= 0.0f && vert.Z < 0.01f) vert.RHW = g_ScreenRHW;
    else if (vert.Z >= -0.01f && vert.Z < 0.0f) vert.RHW = -g_ScreenRHW;
    else vert.RHW = 1.0f / vert.Z;
}
struct CRenderManager2D {
    CTVertex tris[2][3]; long modes[2]; CTexture* texs[2]; int nTris;
    void Add2DTri(const CTVertex& a, const CTVertex& b, const CTVertex& c, long mode, CTexture* t)
    { tris[nTris][0] = a; tris[nTris][1] = b; tris[nTris][2] = c; modes[nTris] = mode; texs[nTris] = t; ++nTris; }
    bool Draw(const C2DBoxF& box, const CBank& bank, unsigned long graphic, unsigned long frameIndex, unsigned long flags)
    {
        const unsigned long* order = s_CornerOrderNormal;
        CTVertex v[4];
        v[0].X = GFAXToVX_009e1e10(box.TLX); v[0].Y = GFAYToVY(box.TLY); v[0].Diffuse = 0xffffffff;
        v[2].X = v[1].X = GFAXToVX_009e1e10(box.BRX); v[1].Y = v[0].Y; v[1].Diffuse = 0xffffffff;
        v[3].Y = v[2].Y = GFAYToVY(box.BRY); v[3].X = v[0].X; v[2].Diffuse = 0xffffffff; v[3].Diffuse = 0xffffffff;
        if (!bank.has)
            return false;
        CTextureQuadUVs uvs = bank.data.quad;
        if (flags & 0x10) order = s_CornerOrderFlipX;
        else if (flags & 0x20) order = s_CornerOrderFlipY;
        long width = bank.data.tex->W;
        long height = bank.data.tex->H;
        int i;
        for (i = 0; i < 4; ++i) { uvs.U[i] += 0.01f / width; uvs.V[i] += 0.01f / height; }
        for (i = 0; i < 4; ++i) { v[i].U = uvs.U[order[i]]; v[i].V = uvs.V[order[i]]; v[i].Z = 0.0f; SetRHWForZ(v[i]); }
        long mode;
        if (flags & 1) mode = 3;
        else if (flags & 2) mode = 2;
        else if ((bank.data.m_flags & 2) || (flags & 4)) mode = 4;
        else mode = (bank.data.m_flags & 1) == 1 ? 3 : 1;
        Add2DTri(v[0], v[1], v[2], mode, bank.data.tex);
        Add2DTri(v[0], v[2], v[3], mode, bank.data.tex);
        return true;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.00001f && d > -0.00001f; }

int main()
{
    CTexture tex; tex.W = 100; tex.H = 50;
    CBank bank; bank.has = true; bank.w = 10; bank.h = 20; bank.data.tex = &tex; bank.data.m_flags = 0;
    for (int i = 0; i < 4; ++i) { bank.data.quad.U[i] = (float)i; bank.data.quad.V[i] = 10.0f * i; }
    CRenderManager2D m; m.nTris = 0;
    C2DBoxF box; box.TLX = 3; box.TLY = 5; box.BRX = 8; box.BRY = 10;
    if (!m.Draw(box, bank, 1, 0, 0x10)) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    const CTVertex* a = m.tris[0]; const CTVertex* b = m.tris[1];
    if (m.nTris != 2 || m.modes[0] != 1 || m.texs[1] != &tex) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    if (a[0].X != 6.0f || a[0].Y != 20.0f || a[1].X != 16.0f || a[1].Y != 20.0f || a[2].X != 16.0f || a[2].Y != 40.0f || b[2].X != 6.0f || b[2].Y != 40.0f)
        { std::printf("009dd210_TEST FAIL\n"); return 1; }
    if (a[0].Diffuse != 0xffffffffu || b[2].Diffuse != 0xffffffffu || a[1].RHW != 0.25f || a[1].Z != 0.0f) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    if (!Near(a[0].U, 1.0001f) || !Near(a[1].U, 0.0001f) || !Near(a[2].V, 30.0002f) || !Near(b[2].U, 2.0001f)) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    m.nTris = 0; m.Draw(box, bank, 1, 0, 0x20);
    if (!Near(m.tris[0][0].U, 3.0001f) || m.modes[0] != 1) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    m.nTris = 0; m.Draw(box, bank, 1, 0, 4);
    if (m.modes[0] != 4) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    bank.data.m_flags = 1; m.nTris = 0; m.Draw(box, bank, 1, 0, 0);
    if (m.modes[0] != 3) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    m.nTris = 0; m.Draw(box, bank, 1, 0, 3);
    if (m.modes[0] != 3) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    m.nTris = 0; m.Draw(box, bank, 1, 0, 2);
    if (m.modes[0] != 2) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    bank.has = false; m.nTris = 0;
    if (m.Draw(box, bank, 1, 0, 0) || m.nTris != 0) { std::printf("009dd210_TEST FAIL\n"); return 1; }
    std::printf("009dd210_TEST PASS\n");
    return 0;
}
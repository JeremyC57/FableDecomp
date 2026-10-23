#include <cstdio>

// Model of CRenderManager2D::Draw2DBoxColours: the box edges are converted to screen space, the four corners (TL, TR,
// BR, BL) take their own ARGB colour, Z 0, UV 0 and the screen RHW; flag 1 picks blend mode 3, else flag 2 picks 2,
// else 1; two triangles (0,1,2) and (0,2,3) are queued and the call returns true.
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour {
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((unsigned long)A << 8) | R) << 8 | G) << 8) | B; }
};
struct CTVertex { float X, Y, Z, RHW; unsigned long Diffuse, Specular; float U, V; };
static const float g_ScreenRHW = 0.5f;
static float GFAXToVX(float x) { return x * 2.0f; }
static float GFAYToVY(float y) { return y * 3.0f; }
struct CRenderManager2D {
    CTVertex tris[2][3]; long modes[2]; int nTris;
    void Add2DTri(const CTVertex& a, const CTVertex& b, const CTVertex& c, long mode, void*)
    { tris[nTris][0] = a; tris[nTris][1] = b; tris[nTris][2] = c; modes[nTris] = mode; ++nTris; }
    bool Draw2DBoxColours(const C2DBoxF& box, const CRGBColour* colours, unsigned long flags)
    {
        unsigned long argb[4];
        for (int i = 0; i < 4; ++i) argb[i] = colours[i].GetARGB();
        float left = GFAXToVX(box.TLX), top = GFAYToVY(box.TLY), right = GFAXToVX(box.BRX), bottom = GFAYToVY(box.BRY);
        CTVertex v[4];
        for (int i = 0; i < 4; ++i) { v[i].U = 0.0f; v[i].V = 0.0f; v[i].Diffuse = argb[i]; v[i].Z = 0.0f; v[i].RHW = g_ScreenRHW; }
        v[0].X = left; v[0].Y = top; v[1].X = right; v[1].Y = top; v[2].X = right; v[2].Y = bottom; v[3].X = left; v[3].Y = bottom;
        long mode;
        if (flags & 1) mode = 3;
        else mode = (flags & 2) ? 2 : 1;
        Add2DTri(v[0], v[1], v[2], mode, 0);
        Add2DTri(v[0], v[2], v[3], mode, 0);
        return true;
    }
};

int main()
{
    CRenderManager2D m; m.nTris = 0;
    C2DBoxF box; box.TLX = 1; box.TLY = 2; box.BRX = 4; box.BRY = 5;
    CRGBColour c[4];
    for (int i = 0; i < 4; ++i) { c[i].B = (unsigned char)i; c[i].G = 0x10; c[i].R = 0x20; c[i].A = 0x80; }
    if (!m.Draw2DBoxColours(box, c, 3)) { std::printf("009dda50_TEST FAIL\n"); return 1; }
    const CTVertex* a = m.tris[0]; const CTVertex* b = m.tris[1];
    if (m.nTris != 2 || m.modes[0] != 3 || m.modes[1] != 3) { std::printf("009dda50_TEST FAIL\n"); return 1; }
    if (a[0].X != 2 || a[0].Y != 6 || a[1].X != 8 || a[1].Y != 6 || a[2].X != 8 || a[2].Y != 15 || b[2].X != 2 || b[2].Y != 15)
        { std::printf("009dda50_TEST FAIL\n"); return 1; }
    if (a[0].Diffuse != 0x80201000u || a[1].Diffuse != 0x80201001u || a[2].Diffuse != 0x80201002u || b[2].Diffuse != 0x80201003u
        || a[1].RHW != 0.5f || a[1].Z != 0.0f || a[2].U != 0.0f || b[2].V != 0.0f)
        { std::printf("009dda50_TEST FAIL\n"); return 1; }
    m.nTris = 0; m.Draw2DBoxColours(box, c, 2);
    if (m.modes[0] != 2) { std::printf("009dda50_TEST FAIL\n"); return 1; }
    m.nTris = 0; m.Draw2DBoxColours(box, c, 0);
    if (m.modes[1] != 1) { std::printf("009dda50_TEST FAIL\n"); return 1; }
    std::printf("009dda50_TEST PASS\n");
    return 0;
}
#include <cstdio>
class C2DVector { public: float X, Y; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((unsigned long)A << 8) | R) << 8 | G) << 8) | B; }
};
class CTexture;
class CTVertexRHWColSpecTex1 { public:
    float X, Y, Z, RHW;
    unsigned long Diffuse;
    unsigned long Specular;
    float U, V;
};
float GFAXToVX(float x) { return x * 2.0f; }
float GFAYToVY(float y) { return y * 3.0f; }
float g_ScreenRHW = 0.5f;

struct CRenderManager2D {
    CTVertexRHWColSpecTex1 got[3]; long gotMode; CTexture* gotTexture;
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long mode, CTexture* texture) { got[0] = v0; got[1] = v1; got[2] = v2; gotMode = mode; gotTexture = texture; }
    bool Draw2DTriangle(const C2DVector& p0, const C2DVector& p1, const C2DVector& p2, const CRGBColour& colour,
                        unsigned long flags);
};

bool CRenderManager2D::Draw2DTriangle(const C2DVector& p0, const C2DVector& p1, const C2DVector& p2,
                                      const CRGBColour& colour, unsigned long flags)
{
    unsigned long argb = colour.GetARGB();
    long mode = 1;
    CTVertexRHWColSpecTex1 v[3];
    v[0].X = GFAXToVX(p0.X);
    v[0].Y = GFAYToVY(p0.Y);
    v[0].Z = 0.0f;
    v[0].Diffuse = argb;
    v[0].U = 0.0f;
    v[0].V = 0.0f;
    v[0].RHW = g_ScreenRHW;
    v[1].X = GFAXToVX(p1.X);
    v[1].Y = GFAYToVY(p1.Y);
    v[1].Z = 0.0f;
    v[1].Diffuse = argb;
    v[1].U = 0.0f;
    v[1].V = 0.0f;
    v[1].RHW = g_ScreenRHW;
    v[2].X = GFAXToVX(p2.X);
    v[2].Y = GFAYToVY(p2.Y);
    v[2].Z = 0.0f;
    v[2].Diffuse = argb;
    v[2].U = 0.0f;
    v[2].V = 0.0f;
    v[2].RHW = g_ScreenRHW;
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    } else
        mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], mode, 0);
    return true;
}

int main()
{
    CRenderManager2D r; C2DVector a = { 1, 2 }, b = { 3, 4 }, c = { 5, 6 };
    CRGBColour col; col.B = 0x11; col.G = 0x22; col.R = 0x33; col.A = 0x80;
    bool ok = r.Draw2DTriangle(a, b, c, col, 0);
    ok = ok && r.gotMode == 1 && r.gotTexture == 0 && r.got[0].X == 2 && r.got[0].Y == 6 && r.got[1].X == 6
            && r.got[2].Y == 18 && r.got[1].Z == 0 && r.got[2].RHW == 0.5f && r.got[0].Diffuse == 0x80332211UL
            && r.got[2].U == 0 && r.got[1].V == 0;
    r.Draw2DTriangle(a, b, c, col, 1);  ok = ok && r.gotMode == 3;
    col.A = 0xff; r.Draw2DTriangle(a, b, c, col, 1);  ok = ok && r.gotMode == 1 && r.got[1].Diffuse == 0xff332211UL;
    r.Draw2DTriangle(a, b, c, col, 2);  ok = ok && r.gotMode == 2;
    if (!ok) { std::printf("009ddff0_TEST FAIL\n"); return 1; }
    std::printf("009ddff0_TEST PASS\n");
    return 0;
}
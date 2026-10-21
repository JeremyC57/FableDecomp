// CRenderManager2D::Draw2DTexture(const C2DBoxF&, const CRGBColour&, CTexture*, unsigned long) -- draw a textured quad (four TL vertices) through the 2D renderer
// Grade: DIFFER(ours v retail) -- 384v388: zero-constant register choice (retail uses an eax zero for Specular and v1..v3 Z, immediates for U/V and v0.Z) and copy-chain register scheduling
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour {
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((unsigned long)A << 8) | R) << 8 | G) << 8) | B; }
};
struct CTexture;
struct CTVertexRHWColSpecTex1 {
    float X, Y, Z, RHW;
    unsigned long Diffuse;
    unsigned long Specular;
    float U, V;
};
void __fastcall GFVBoxToABox(const C2DBoxF& in, C2DBoxF* out);   // 0x009E1E70
extern float g_ScreenRHW;                          // 0x0139652C

struct CRenderManager2D {
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long mode, CTexture* texture);   // 0x009DC030
    bool Draw2DTexture(const C2DBoxF& box, const CRGBColour& colour, CTexture* texture, unsigned long flags);
};

bool CRenderManager2D::Draw2DTexture(const C2DBoxF& box, const CRGBColour& colour, CTexture* texture, unsigned long flags)
{
    unsigned long argb = colour.GetARGB();
    C2DBoxF screen;
    GFVBoxToABox(box, &screen);
    CTVertexRHWColSpecTex1 v[4];
    v[0].X = screen.TLX; v[0].Y = screen.TLY; v[0].Z = 0.0f; v[0].RHW = g_ScreenRHW;
    v[0].Diffuse = argb; v[0].Specular = 0; v[0].U = 0.0f; v[0].V = 0.0f;
    v[1].X = screen.BRX; v[1].Y = screen.TLY; v[1].Z = 0.0f; v[1].RHW = g_ScreenRHW;
    v[1].Diffuse = argb; v[1].Specular = 0; v[1].U = 1.0f; v[1].V = 0.0f;
    v[2].X = screen.BRX; v[2].Y = screen.BRY; v[2].Z = 0.0f; v[2].RHW = g_ScreenRHW;
    v[2].Diffuse = argb; v[2].Specular = 0; v[2].U = 1.0f; v[2].V = 1.0f;
    v[3].X = screen.TLX; v[3].Y = screen.BRY; v[3].Z = 0.0f; v[3].RHW = g_ScreenRHW;
    v[3].Diffuse = argb; v[3].Specular = 0; v[3].U = 0.0f; v[3].V = 1.0f;
    long mode;
    if (flags & 2)
        mode = 2;
    else if (flags & 1)
        mode = 3;
    else
        mode = 1;
    Add2DTri(v[0], v[1], v[2], mode, texture);
    Add2DTri(v[0], v[2], v[3], mode, texture);
    return true;
}

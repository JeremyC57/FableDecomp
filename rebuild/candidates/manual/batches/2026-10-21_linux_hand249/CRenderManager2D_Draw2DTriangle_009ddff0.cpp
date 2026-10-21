// CRenderManager2D::Draw2DTriangle(const C2DVector&, const C2DVector&, const C2DVector&, const CRGBColour&, unsigned long) -- queue a flat-coloured screen-space triangle
// Leaf: Draw2DTriangle
// Retail 0x009DDFF0, 329 bytes, ret 0x14. Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__stdcall float), the
// screen RHW float 0x0139652C, CRenderManager2D::Add2DTri 0x009DC030 (v0, v1, v2, mode, texture).
// Shape: the three CTVertexRHWColSpecTex1 are an array (separate locals get the reverse frame order) filled field by
// field (Specular left unset as in retail) with the RHW copied last, so the global is re-read for each vertex; the
// ARGB word is packed A,R,G,B from the colour bytes (`movzx eax,R ; mov ah,A`). Blend mode: 1 by default; flag 1 asks
// for alpha only when the colour is not opaque (3), otherwise flag 2 picks 2 (`setne al ; inc eax`). Returns true.
struct C2DVector { float X, Y; };
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
float __stdcall GFAXToVX(float x);                 // 0x009E1E10
float __stdcall GFAYToVY(float y);                 // 0x009E1E40
extern float g_ScreenRHW;                          // 0x0139652C

struct CRenderManager2D {
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long mode, CTexture* texture);   // 0x009DC030
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

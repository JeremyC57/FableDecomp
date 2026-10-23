// CRenderManager2D::Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags) -- queue a flat-coloured screen-space box at depth z as two triangles, deriving each vertex's RHW from z
// Grade: DIFFER(899v899) -- frame-slot swap only: retail homes the converted top edge at [esp+14h] and the right edge at [esp+18h], ours the other way round (every later integer copy reads the swapped slot). Tried: SetRHWForZ void helper, rest-first/positions-last store order (894), `float top, right;` and right-first declarations, `mode = 1` before the ARGB pack (fixes the `or ebx,ecx` schedule).
// Leaf: Draw2DBoxWithZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DDC60, length 899 bytes (ret 0x10); PDB
// ?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z.
// Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__stdcall float), the screen RHW float 0x0139652C,
// CRenderManager2D::Add2DTri 0x009DC030 (v0, v1, v2, mode, texture) twice.
// Shape: the landed Draw2DTriangle 0x009DDFF0 for a quad. The four vertices (TL, TR, BR, BL) are filled field by
// field; each RHW comes from an inline depth helper expanded once per vertex: the screen RHW for z in [0, 0.01),
// its negation for z in [-0.01, 0), else 1/z. Blend mode as in Draw2DTriangle (flag 1: 3 unless the colour is
// opaque; else flag 2: 2; else 1). Triangles (0, 1, 2) and (0, 2, 3). Returns true.
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
float __stdcall GFAXToVX(float x);                 // 0x009E1E10
float __stdcall GFAYToVY(float y);                 // 0x009E1E40
extern float g_ScreenRHW;                          // 0x0139652C

static __forceinline float GetRHWForZ(float z)
{
    if (z >= 0.0f && z < 0.01f)
        return g_ScreenRHW;
    if (z >= -0.01f && z < 0.0f)
        return -g_ScreenRHW;
    return 1.0f / z;
}

struct CRenderManager2D {
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long mode, CTexture* texture);   // 0x009DC030
    bool Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags)
{
    long mode = 1;
    unsigned long argb = colour.GetARGB();
    float left = box.TLX;
    left = GFAXToVX(left);
    float right;
    float top = GFAYToVY(box.TLY);
    right = GFAXToVX(box.BRX);
    float bottom = GFAYToVY(box.BRY);
    CTVertexRHWColSpecTex1 v[4];
    v[0].X = left;
    v[0].Y = top;
    v[0].Z = z;
    v[0].Diffuse = argb;
    v[0].U = 0.0f;
    v[0].V = 0.0f;
    v[0].RHW = GetRHWForZ(z);
    v[1].X = right;
    v[1].Y = top;
    v[1].Z = z;
    v[1].Diffuse = argb;
    v[1].U = 0.0f;
    v[1].V = 0.0f;
    v[1].RHW = GetRHWForZ(z);
    v[2].X = right;
    v[2].Y = bottom;
    v[2].Z = z;
    v[2].Diffuse = argb;
    v[2].U = 0.0f;
    v[2].V = 0.0f;
    v[2].RHW = GetRHWForZ(z);
    v[3].X = left;
    v[3].Y = bottom;
    v[3].Z = z;
    v[3].Diffuse = argb;
    v[3].U = 0.0f;
    v[3].V = 0.0f;
    v[3].RHW = GetRHWForZ(z);
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    } else
        mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], mode, 0);
    Add2DTri(v[0], v[2], v[3], mode, 0);
    return true;
}

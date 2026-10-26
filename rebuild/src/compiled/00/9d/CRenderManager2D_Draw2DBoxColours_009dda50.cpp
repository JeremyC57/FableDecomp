// CRenderManager2D::Draw2DBoxColours(const C2DBoxF& box, const CRGBColour* colours, unsigned long flags) -- queue a screen-space box with one colour per corner (TL, TR, BR, BL) as two triangles
// Leaf: Draw2DBoxColours
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DDA50, length 515 bytes (ret 0xC); PDB
// ?Draw2DBoxColours@CRenderManager2D@@QAE_NABVC2DBoxF@@QBVCRGBColour@@K@Z.
// Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__stdcall float), the screen RHW float 0x0139652C,
// CRenderManager2D::Add2DTri 0x009DC030 (v0, v1, v2, mode, texture) twice.
// Shape: the landed Draw2DTriangle 0x009DDFF0 for a quad: the four ARGB words packed first (the last one lands in the
// dead colours argument slot), the four box edges converted in TLX, TLY, BRX, BRY order, then the vertex array
// (TL, TR, BR, BL) filled field by field -- U, V, Diffuse, Z, RHW of every vertex first, the X/Y positions of all four
// last (that source order reproduces retail's store schedule and the RHW word copied into three registers); triangles (0, 1, 2) and (0, 2, 3). Blend mode: flag 1 picks 3, otherwise
// flag 2 picks 2, else 1 (`setne dl ; inc edx`). Returns true.
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
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
float __stdcall GFAXToVX(float x);                 // 0x009E1E10
float __stdcall GFAYToVY(float y);                 // 0x009E1E40
extern float g_ScreenRHW;                          // 0x0139652C

struct CRenderManager2D {
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long mode, CTexture* texture);   // 0x009DC030
    bool Draw2DBoxColours(const C2DBoxF& box, const CRGBColour* colours, unsigned long flags);
};

bool CRenderManager2D::Draw2DBoxColours(const C2DBoxF& box, const CRGBColour* colours, unsigned long flags)
{
    unsigned long argb[4];
    argb[0] = colours[0].GetARGB();
    argb[1] = colours[1].GetARGB();
    argb[2] = colours[2].GetARGB();
    argb[3] = colours[3].GetARGB();
    float left = box.TLX;
    left = GFAXToVX(left);
    float top = GFAYToVY(box.TLY);
    float right = GFAXToVX(box.BRX);
    float bottom = GFAYToVY(box.BRY);
    CTVertexRHWColSpecTex1 v[4];
    v[0].U = 0.0f;
    v[0].V = 0.0f;
    v[0].Diffuse = argb[0];
    v[0].Z = 0.0f;
    v[0].RHW = g_ScreenRHW;
    v[1].U = 0.0f;
    v[1].V = 0.0f;
    v[1].Diffuse = argb[1];
    v[1].Z = 0.0f;
    v[1].RHW = g_ScreenRHW;
    v[2].U = 0.0f;
    v[2].V = 0.0f;
    v[2].Diffuse = argb[2];
    v[2].Z = 0.0f;
    v[2].RHW = g_ScreenRHW;
    v[3].U = 0.0f;
    v[3].V = 0.0f;
    v[3].Diffuse = argb[3];
    v[3].Z = 0.0f;
    v[3].RHW = g_ScreenRHW;
    v[0].X = left;
    v[0].Y = top;
    v[1].X = right;
    v[1].Y = top;
    v[2].X = right;
    v[2].Y = bottom;
    v[3].X = left;
    v[3].Y = bottom;
    long mode;
    if (flags & 1)
        mode = 3;
    else
        mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], mode, 0);
    Add2DTri(v[0], v[2], v[3], mode, 0);
    return true;
}
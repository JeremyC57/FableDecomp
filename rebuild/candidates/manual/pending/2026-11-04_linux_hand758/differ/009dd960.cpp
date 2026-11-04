// CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags) -- queue a flat-coloured screen-space line (two pre-transformed vertices) with the blend mode the flags ask for
// Grade: DIFFER(227v229) -- (try e, landed sibling spellings GFVXToAX/GFVYToAY/g_ScreenRHW/private Add2DLine): retail copies the RHW global through edx/eax integer moves, ours fld/fst/fstp; integer copies only with two separate reads of the global (dl=16)
// Leaf: ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009DD960, 229 bytes, `ret 10h`. Ego_r.pdb ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z.
// Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (__fastcall float, the landed spellings; Ego_r.pdb ?GFVXToAX@@YIMM@Z),
//   the screen RHW float 0x0139652C (landed g_ScreenRHW), CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1&,
//   const CTVertexRHWColSpecTex1&, long, CTexture*) 0x009DC2C0 (Ego_r.pdb, private).
// Shape: the landed Draw2DTriangle 0x009DDFF0 model (CRGBColour::GetARGB, a vertex array filled field by field, the blend mode
//   1 / 3 for a non-opaque colour under flag 1 / 1 + (flag 2)); returns true.
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
float __fastcall GFVXToAX(float x);                // 0x009E1E10
float __fastcall GFVYToAY(float y);                // 0x009E1E40
extern float g_ScreenRHW;                          // 0x0139652C

class CRenderManager2D { public:
    bool Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags);
private:
    void Add2DLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, long mode, CTexture* texture);   // 0x009DC2C0
};

bool CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags)
{
    unsigned long argb = colour.GetARGB();
    long mode = 1;
    CTVertexRHWColSpecTex1 v0;
    CTVertexRHWColSpecTex1 v1;
    v0.X = GFVXToAX(start.X);
    v0.Y = GFVYToAY(start.Y);
    v1.X = GFVXToAX(end.X);
    v1.Y = GFVYToAY(end.Y);
    v1.RHW = g_ScreenRHW;
    v0.Diffuse = argb;
    v1.Diffuse = argb;
    v0.Specular = 0;
    v1.Specular = 0;
    v0.Z = 0.0f;
    v1.Z = 0.0f;
    v0.RHW = v1.RHW;
    v1.U = 0.0f;
    v0.U = 0.0f;
    v1.V = 0.0f;
    v0.V = 0.0f;
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    } else
        mode = (flags & 2) ? 2 : 1;
    Add2DLine(v0, v1, mode, 0);
    return true;
}

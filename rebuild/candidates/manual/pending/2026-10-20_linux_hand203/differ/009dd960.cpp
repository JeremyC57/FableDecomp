// CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags) -- queue a screen-space line between two virtual-screen points and report success
// Leaf: Draw2DLine
// Grade: DIFFER(227v229) -- the RHW global is copied through the FPU (fld ; fst ; fstp) where retail loads it once into edx and
// stores it with integer moves (mov eax,edx for the end vertex first); everything else matches.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DD960, length 229 bytes (`ret 10h`).
// PDB: ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z.
// Callees: GFAXToVX(float) @ 0x009E1E10 and GFAYToVY(float) @ 0x009E1E40 (__stdcall, float in a stack slot: the
// manifest's __fastcall is wrong), CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1*, const
// CTVertexRHWColSpecTex1*, long mode, CTexture*) @ 0x009DC2C0. Global @ 0x0139652C: the RHW written into every 2D
// vertex.
// Shape: the colour is packed to a D3DCOLOR (A<<24 | R<<16 | G<<8 | B) into a local first, then two pre-transformed
// vertices are filled member by member (start in the higher frame slot); the blend mode is 1 (opaque), 3 for an
// alpha-blended colour when flag 1 asks for alpha, else 1 + (flag 2 set). Always returns true (`mov al,1`).
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CTexture;

struct CTVertexRHWColSpecTex1 {                 // 0x20 bytes
    float         X, Y, Z;                      // +0x00
    float         RHW;                          // +0x0c
    unsigned long Colour;                       // +0x10 D3DCOLOR
    unsigned long Specular;                     // +0x14
    float         U, V;                         // +0x18
};

float __stdcall GFAXToVX(float x);              // 0x009E1E10
float __stdcall GFAYToVY(float y);              // 0x009E1E40
extern float g_2DVertexRHW;                     // 0x0139652C

struct CRenderManager2D {
    void Add2DLine(const CTVertexRHWColSpecTex1* start, const CTVertexRHWColSpecTex1* end, long mode, CTexture* texture);   // 0x009DC2C0
    bool Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags)
{
    unsigned long argb = ((unsigned long)colour.A << 24) | ((unsigned long)colour.R << 16) | ((unsigned long)colour.G << 8) | colour.B;
    long mode = 1;
    CTVertexRHWColSpecTex1 v0;
    CTVertexRHWColSpecTex1 v1;
    v0.X = GFAXToVX(start.X);
    v0.Y = GFAYToVY(start.Y);
    v1.X = GFAXToVX(end.X);
    v1.Y = GFAYToVY(end.Y);
    v1.RHW = g_2DVertexRHW;
    v0.Colour = argb;
    v1.Colour = argb;
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
    }
    else
        mode = (flags & 2) ? 2 : 1;
    Add2DLine(&v0, &v1, mode, 0);
    return true;
}

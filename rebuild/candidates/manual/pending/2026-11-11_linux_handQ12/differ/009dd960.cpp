// CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags) -- queue a flat-coloured screen-space line (two pre-transformed vertices) with the blend mode the flags ask for
// Grade: DIFFER(227v229) -- on the shared headers (opt-ins FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR, FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DLINE): retail copies the RHW float 0x0139652C with integer moves from ONE load (`mov edx,[g] ; mov eax,edx`, v1.RHW = eax, v0.RHW = edx); one load stored twice compiles to fld/fst/fstp, two reads (fields or inline SetRHW setters, const or not) give integer moves from two loads (233v229), a float local read before the calls gives the copy pattern but loads early and swaps the targets; v1 built as a copy of v0 is far off. Candidate name for 0x0139652C: the per-TU `BIGGEST_RHW_VERTEX_VALUE` (Ego_r.pdb has one `dynamic initializer for BIGGEST_RHW_VERTEX_VALUE` per TU; every xref of 0x0139652C is in the 2D render TU) -- unproven, the bound spelling g_ScreenRHW is kept
// Leaf: ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009DD960, 229 bytes, thiscall, `ret 10h`. Ego_r.pdb / FableWin.pdb ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z.
// Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (Ego_r.pdb ?GFVXToAX@@YIMM@Z / ?GFVYToAY@@YIMM@Z, __fastcall float, the argument
//   on the stack), the 2D TU's screen RHW float 0x0139652C (bound spelling g_ScreenRHW), the private CRenderManager2D::Add2DLine
//   0x009DC2C0 (Ego_r.pdb ?Add2DLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z).
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DLINE
#include "fable_compiler.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CRenderManager2D.h"
float FABLE_FASTCALL GFVXToAX(float x);           // 0x009E1E10
float FABLE_FASTCALL GFVYToAY(float y);           // 0x009E1E40
extern float g_ScreenRHW;                         // 0x0139652C

bool CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags)
{
    unsigned long argb = colour.GetD3DCOLOR();
    long mode = 1;
    CTVertexRHWColSpecTex1 v0;
    CTVertexRHWColSpecTex1 v1;
    v0.X = GFVXToAX(start.X);
    v0.Y = GFVYToAY(start.Y);
    v1.X = GFVXToAX(end.X);
    v1.Y = GFVYToAY(end.Y);
    v0.RHW = g_ScreenRHW;
    v1.RHW = v0.RHW;
    v0.DiffuseColour = argb;
    v1.DiffuseColour = argb;
    v0.SpecularColour = 0;
    v1.SpecularColour = 0;
    v0.Z = 0.0f;
    v1.Z = 0.0f;
    v1.U1 = 0.0f;
    v0.U1 = 0.0f;
    v1.V1 = 0.0f;
    v0.V1 = 0.0f;
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    } else
        mode = (flags & 2) ? 2 : 1;
    Add2DLine(v0, v1, mode, 0);
    return true;
}

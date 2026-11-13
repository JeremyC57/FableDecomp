// CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags) -- queue a flat-coloured screen-space line (two pre-transformed vertices) with the blend mode the flags ask for
// Lever (chunk Q13): the two RHW stores are two plain reads of the screen RHW float placed after the Z zeroes (`v0.Z = 0; v1.Z = 0; v0.RHW = g_ScreenRHW; v1.RHW = g_ScreenRHW;`): c2 merges the two reads into one load and copies it (`mov edx,[0139652C] ; xor ecx,ecx ; mov eax,edx ; mov [v1.RHW],eax`, 0x009DD9C8..0x009DD9D2, v0.RHW = edx at 0x009DD9F4), as retail; read before the Z zeroes it compiles to two loads, `v1.RHW = v0.RHW` to fld/fst/fstp. Graded RELOCATION_MATCH with FABLE_HDR_OVERLAY=agentQ13/hdr (no overlay header used). Name note: 0x0139652C is plausibly the per-TU const `BIGGEST_RHW_VERTEX_VALUE` (FableWin.pdb has one dynamic initializer per TU); unproven, the landed spelling g_ScreenRHW (0x009DC870) is kept.
// Leaf: ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009DD960, 229 bytes, thiscall, `ret 10h`. Ego_r.pdb / FableWin.pdb ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z.
// Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (Ego_r.pdb ?GFVXToAX@@YIMM@Z / ?GFVYToAY@@YIMM@Z, __fastcall float, the argument
//   on the stack), the 2D TU's screen RHW float 0x0139652C (bound spelling g_ScreenRHW), the private CRenderManager2D::Add2DLine
//   0x009DC2C0 (Ego_r.pdb ?Add2DLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z).
// Colour: the D3DCOLOR is packed here from the four channel accessors (Ego_r.pdb / FableWin.pdb GetA / GetR / GetG / GetB), the
//   D3DCOLOR_ARGB shape (`movzx eax,[R] ; mov ah,[A] ; shl eax,8 ; or eax,G ; shl eax,8 ; or eax,B`). It is not CRGBColour::GetD3DCOLOR:
//   FableWin.pdb's debug GetD3DCOLOR 0x02CCFDA0 is a one-load accessor (0x20-byte slot), see engine/CRGBColour.h.
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
    unsigned long argb = ((unsigned long)colour.GetA() << 24) | ((unsigned long)colour.GetR() << 16) | ((unsigned long)colour.GetG() << 8) | colour.GetB();
    long mode = 1;
    CTVertexRHWColSpecTex1 v0;
    CTVertexRHWColSpecTex1 v1;
    v0.X = GFVXToAX(start.X);
    v0.Y = GFVYToAY(start.Y);
    v1.X = GFVXToAX(end.X);
    v1.Y = GFVYToAY(end.Y);
    v0.DiffuseColour = argb;
    v1.DiffuseColour = argb;
    v0.SpecularColour = 0;
    v1.SpecularColour = 0;
    v0.Z = 0.0f;
    v1.Z = 0.0f;
    v0.RHW = g_ScreenRHW;
    v1.RHW = g_ScreenRHW;
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
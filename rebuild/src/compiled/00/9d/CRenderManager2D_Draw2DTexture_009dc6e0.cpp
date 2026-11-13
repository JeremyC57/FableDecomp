// CRenderManager2D::Draw2DTexture(const C2DBoxF& box, const CRGBColour& colour, CTexture& texture, unsigned long flags) -- queue a whole texture as a flat-tinted screen-space box (two triangles, UVs 0..1) with the blend mode the flags ask for
// Leaf: ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@AAVCTexture@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009DC6E0, 388 bytes, thiscall, `ret 10h`, returns true. Ego_r.pdb 0x00B302F0 / FableWin.pdb
// ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@AAVCTexture@@K@Z (the box overload, just before the CUVBox one
// 0x009DC870).
// Callees: the colour packing (inline, the A,R,G,B word, 0x009DC6ED..0x009DC70B), GFVBoxToABox 0x009E1E70 (__fastcall, virtual
//   box in, screen box out; Ego_r.pdb ?GFVBoxToABox@@YIXABVC2DBoxF@@AAV1@@Z), CGenericVertex<CTVertexRHWColSpecTex1Base>::SetRHWFromZ
//   (inline; reads the 2D TU's screen RHW float 0x0139652C, bound spelling g_ScreenRHW), the private CRenderManager2D::Add2DTri
//   0x009DC030 (v0, v1, v2, mode, &texture) twice.
// Shape: the ARGB word is packed before the box conversion (esi across the call). The four corners (TL, TR, BR, BL) take X/Y from
//   the converted box first, one statement per member in vertex order: c2 merges the TLY / BRX / BRY reads of neighbouring
//   vertices into a load + register copy (0x009DC72D, 0x009DC733, 0x009DC741) and reloads TLX for the last vertex (0x009DC750).
//   The fixed 0..1 UVs follow as immediates (0x009DC798..0x009DC7D3). Then, per vertex, Z = 0, SetRHWFromZ(), specular 0, diffuse:
//   with Z a known 0 the inline SetRHWFromZ folds to its first arm (RHW = the screen RHW float), which is why retail loads that
//   float twice (0x009DC747 for vertex 0, 0x009DC75E shared by vertices 1..3 with one register copy 0x009DC796) and why vertex
//   0's Z is the one late immediate store (0x009DC7DE) while the other Z and specular zeroes share `xor eax,eax`. Plain
//   `v[i].RHW = g_ScreenRHW` stores in the same places leave a third register copy for vertex 3 and other argument registers.
//   Blend mode: flag 2 picks 2, else flag 1 picks 3, else 1 (`test al,2` hoisted above the stores, the inner choice branchless).
//   Triangles (0, 1, 2) and (0, 2, 3).
#define FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ
// Colour: the D3DCOLOR is packed here from the four channel accessors (Ego_r.pdb / FableWin.pdb GetA / GetR / GetG / GetB), the
//   D3DCOLOR_ARGB shape (`movzx eax,[R] ; mov ah,[A] ; shl eax,8 ; or eax,G ; shl eax,8 ; or eax,B`). It is not CRGBColour::GetD3DCOLOR:
//   FableWin.pdb's debug GetD3DCOLOR 0x02CCFDA0 is a one-load accessor (0x20-byte slot), see engine/CRGBColour.h.
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ
#include "fable_compiler.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CTexture.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CRenderManager2D.h"
void FABLE_FASTCALL GFVBoxToABox(const C2DBoxF& v, C2DBoxF& a);   // 0x009E1E70

bool CRenderManager2D::Draw2DTexture(const C2DBoxF& box, const CRGBColour& colour, CTexture& texture, unsigned long flags)
{
    unsigned long argb = ((unsigned long)colour.GetA() << 24) | ((unsigned long)colour.GetR() << 16) | ((unsigned long)colour.GetG() << 8) | colour.GetB();
    C2DBoxF screen;
    GFVBoxToABox(box, screen);
    CTVertexRHWColSpecTex1 v[4];
    v[0].X = screen.TLX;
    v[0].Y = screen.TLY;
    v[1].X = screen.BRX;
    v[1].Y = screen.TLY;
    v[2].X = screen.BRX;
    v[2].Y = screen.BRY;
    v[3].X = screen.TLX;
    v[3].Y = screen.BRY;
    v[0].U1 = 0.0f;
    v[0].V1 = 0.0f;
    v[1].U1 = 1.0f;
    v[1].V1 = 0.0f;
    v[2].U1 = 1.0f;
    v[2].V1 = 1.0f;
    v[3].U1 = 0.0f;
    v[3].V1 = 1.0f;
    v[0].Z = 0.0f;
    v[0].SetRHWFromZ();
    v[0].SpecularColour = 0;
    v[0].DiffuseColour = argb;
    v[1].Z = 0.0f;
    v[1].SetRHWFromZ();
    v[1].SpecularColour = 0;
    v[1].DiffuseColour = argb;
    v[2].Z = 0.0f;
    v[2].SetRHWFromZ();
    v[2].SpecularColour = 0;
    v[2].DiffuseColour = argb;
    v[3].Z = 0.0f;
    v[3].SetRHWFromZ();
    v[3].SpecularColour = 0;
    v[3].DiffuseColour = argb;
    long mode;
    if (flags & 2)
        mode = 2;
    else if (flags & 1)
        mode = 3;
    else
        mode = 1;
    Add2DTri(v[0], v[1], v[2], mode, &texture);
    Add2DTri(v[0], v[2], v[3], mode, &texture);
    return true;
}
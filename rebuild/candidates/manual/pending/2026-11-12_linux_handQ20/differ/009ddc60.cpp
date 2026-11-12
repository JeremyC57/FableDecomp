// CRenderManager2D::Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags) -- queue a flat-coloured screen-space box at depth z as two triangles, deriving each vertex's RHW from z
// Grade: DIFFER(899v899) -- Q20 port onto the shared C2DBoxF / CRGBColour / CTVertexRHWColSpecTex1 / CRenderManager2D headers (overlay DRAW2DBOXWITHZ + SETRHWFROMZ); 12 diff lines, frame-slot swap only: retail homes the converted top edge (GFVYToAY(TLY)) at [esp+14h] and the right edge (GFVXToAX(BRX)) at [esp+18h], ours the other way round (every later integer copy reads the swapped slot). A C2DBoxF screen-box local gives the right order but grows the frame 0x94 v 0x90; boxcopy / all-from-box spellings no better.
// Leaf: ?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009DDC60, 899 bytes, thiscall, `ret 10h`. Ego_r.pdb / FableWin.pdb
//   ?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z.
// Callees: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 (Ego_r.pdb ?GFVXToAX@@YIMM@Z / ?GFVYToAY@@YIMM@Z, __fastcall float, the argument on
//   the stack), the 2D TU's screen RHW float 0x0139652C (landed spelling g_ScreenRHW, read by the inline SetRHWFromZ), the private
//   CRenderManager2D::Add2DTri 0x009DC030 (Ego_r.pdb ?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z) twice.
// Shape: the landed Draw2DLine 0x009DD960 / Draw2DBoxColours 0x009DDA50 for a quad at depth z. The four vertices (TL, TR, BR, BL) are
//   filled field by field; each RHW comes from the vertex's own inline SetRHWFromZ (FableWin.pdb CGenericVertex<...>::SetRHWFromZ),
//   expanded once per vertex. Blend mode as Draw2DLine (flag 1: 3 unless the colour is opaque; else flag 2: 2; else 1). Triangles
//   (0, 1, 2) and (0, 2, 3). Returns true.
// Header opt-ins (chunk Q20 overlay): FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ (declarations), FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ.
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ
#define FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ
#include "fable_compiler.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CRenderManager2D.h"
float FABLE_FASTCALL GFVXToAX(float x);           // 0x009E1E10
float FABLE_FASTCALL GFVYToAY(float y);           // 0x009E1E40

bool CRenderManager2D::Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags)
{
    long mode = 1;
    unsigned long argb = colour.GetD3DCOLOR();
    float left = box.TLX;
    left = GFVXToAX(left);
    float right;
    float top = GFVYToAY(box.TLY);
    right = GFVXToAX(box.BRX);
    float bottom = GFVYToAY(box.BRY);
    CTVertexRHWColSpecTex1 v[4];
    v[0].X = left;
    v[0].Y = top;
    v[0].Z = z;
    v[0].DiffuseColour = argb;
    v[0].U1 = 0.0f;
    v[0].V1 = 0.0f;
    v[0].SetRHWFromZ();
    v[1].X = right;
    v[1].Y = top;
    v[1].Z = z;
    v[1].DiffuseColour = argb;
    v[1].U1 = 0.0f;
    v[1].V1 = 0.0f;
    v[1].SetRHWFromZ();
    v[2].X = right;
    v[2].Y = bottom;
    v[2].Z = z;
    v[2].DiffuseColour = argb;
    v[2].U1 = 0.0f;
    v[2].V1 = 0.0f;
    v[2].SetRHWFromZ();
    v[3].X = left;
    v[3].Y = bottom;
    v[3].Z = z;
    v[3].DiffuseColour = argb;
    v[3].U1 = 0.0f;
    v[3].V1 = 0.0f;
    v[3].SetRHWFromZ();
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    } else
        mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], mode, 0);
    Add2DTri(v[0], v[2], v[3], mode, 0);
    return true;
}

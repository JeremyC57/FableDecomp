// 009ddc60 CRenderManager2D::Draw2DBoxWithZ
// Leaf: ?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z
// Flags: /d2inlT200
// Inline-budget: native expands SetRHWFromZ four times; ordinary inline defaults473B, T200 restores all four expansions and899B.
// Original GetTLX accessor keeps the native input home and converted-edge allocation.
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ
#define FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ
#define FABLE_RETAIL_CGENERICVERTEX_ORIGINAL_RHW_PASS23
#define FABLE_RETAIL_CGENERICVERTEX_RHW_PLAIN_PASS23
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DBOXF
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CRenderManager2D.h"
float FABLE_FASTCALL GFVXToAX(float x);           // 0x009E1E10
float FABLE_FASTCALL GFVYToAY(float y);           // 0x009E1E40

bool CRenderManager2D::Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags)
{
    long mode = 1;
    unsigned long argb = ((unsigned long)colour.GetA()<<24)|((unsigned long)colour.GetR()<<16)|((unsigned long)colour.GetG()<<8)|colour.GetB();
    float left = GFVXToAX(box.GetTLX());
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

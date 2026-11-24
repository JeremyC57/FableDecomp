// 00ab7c20 CStaticFontBank::Render
// Inferred: parenthesized texture-coordinate products reproduce the vertex-copy schedule.
// Leaf: ?Render@CStaticFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z
// Flags: default
#define FABLE_RETAIL_CSTATICFONTBANK_RENDER_PASS28
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTATICFONTBANK
#define FABLE_RETAIL_CSTATICFONTBANK_CTOR
#define FABLE_RETAIL_CSTATICFONTBANK_FINDGLYPH
#define FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#include <math.h>
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontManager.h"
#include "engine/CStaticFontBank.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"

#define FABLE_RETAIL_FONT2DVERTEX_PASS28
#include "engine/FONT2DVERTEX.h"
inline FONT2DVERTEX __fastcall InitFont2DVertex(const D3DXVECTOR4& p, unsigned long color, float tu, float tv)
{
    FONT2DVERTEX vertex;
    vertex.p = p;
    vertex.color = color;
    vertex.tu = tu;
    vertex.tv = tv;
    return vertex;
}

void CStaticFontBank::Render(const CWideString& str, const C3DVector& pos, float scale,
                             const CRGBColour& colour, unsigned long flags, const CTexture* mask) const
{
    const float originx = pos.X;
    const float originy = pos.Y;
    float sx = originx;
    float sy = originy;
    const float rhw = 1.0f;
    const unsigned long d3dcolor = colour.GetD3DCOLOR();
    CRenderManager& render_manager = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    const CTexture* handles[2] = {PTexture, mask};
    const unsigned long texture_count = mask ? 2 : 1;
    CVertexBufferWin32* pvb = &PManager->GetFreeVertexBuffer();
    if (flags & 8) {
        C2DExtentsF extents = GetStringExtents(reinterpret_cast<const FableFontWideChar*>(static_cast<const wchar_t*>(str)), false);
        sy = (float)floor(sy - extents.Y * 0.5f);
    }
    unsigned long no_tris = 0;
    FONT2DVERTEX* pvertices = reinterpret_cast<FONT2DVERTEX*>(pvb->Lock(0, 0, 0));
    int starting_new_line = 1;
    const unsigned short* pstr = str.GetAsCharArray();
    while (*pstr) {
        if (starting_new_line) {
            if (flags & 6) {
                C2DExtentsF extents = GetStringExtents(reinterpret_cast<const FableFontWideChar*>(pstr), true);
                if (flags & 2)
                    sx = (float)floor(originx - extents.X);
                if (flags & 4)
                    sx = (float)floor(originx - extents.X * 0.5f);
            }
            starting_new_line = 0;
        }
        unsigned short letter = *pstr++;
        if (letter == '\n') {
            sx = originx;
            sy += (float)FontHeight;
            starting_new_line = 1;
        }
        if (letter < MinChar || letter > MaxChar)
            continue;
        const CGlyph* pglyph = FindGlyph(letter);
        if (!pglyph)
            continue;
        sx += (float)pglyph->Offset;
        const float sx1 = (sx - originx) * scale + originx - 0.5f;
        const float sx2 = ((sx - originx) * scale + originx - 0.5f) + ((float)pglyph->Width + 1.0f) * scale;
        const float sy1 = (sy - originy) * scale + originy - 0.5f;
        const float sy2 = ((sy - originy) * scale + originy - 0.5f) + ((float)FontHeight + 1.0f) * scale;
        sx += (float)pglyph->Advance;
        const float tx1 = ((TextureSize.X - 1) * pglyph->Left) / TextureSize.X;
        const float ty1 = ((TextureSize.Y - 1) * pglyph->Top) / TextureSize.Y;
        const float tx2 = ((TextureSize.X - 1) * pglyph->Right) / TextureSize.X;
        const float ty2 = ((TextureSize.Y - 1) * pglyph->Bottom) / TextureSize.Y;
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx1, sy2, pos.Z, rhw), d3dcolor, tx1, ty2);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx1, sy1, pos.Z, rhw), d3dcolor, tx1, ty1);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx2, sy2, pos.Z, rhw), d3dcolor, tx2, ty2);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx2, sy1, pos.Z, rhw), d3dcolor, tx2, ty1);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx2, sy2, pos.Z, rhw), d3dcolor, tx2, ty2);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx1, sy1, pos.Z, rhw), d3dcolor, tx1, ty1);
        no_tris += 2;
        if (no_tris * 3 > 0x126) {
            pvb->Unlock();
            render_manager.SelectVertexBuffer(0, pvb);
            render_manager.DrawCurrentBuffersAsTris(handles, texture_count, 0, no_tris);
            render_manager.SelectVertexBuffer(0, 0);
            pvb = &PManager->GetFreeVertexBuffer();
            pvertices = reinterpret_cast<FONT2DVERTEX*>(pvb->Lock(0, 0, 0));
            no_tris = 0;
        }
    }
    pvb->Unlock();
    if (no_tris > 0) {
        render_manager.SelectVertexBuffer(0, pvb);
        render_manager.DrawCurrentBuffersAsTris(handles, texture_count, 0, no_tris);
        render_manager.SelectVertexBuffer(0, 0);
    }
}
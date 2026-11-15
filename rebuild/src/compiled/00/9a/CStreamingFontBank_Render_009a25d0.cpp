// CStreamingFontBank::Render(const CWideString&, const C3DVector&, float, const CRGBColour&, unsigned long, const CTexture*) const -- draw a string from the streamed glyph cache as screen-space quads
// Grade: RELOCATION_MATCH (wfE9 successor; fix-up 2026-11-15) -- flag 8 centres the block vertically (cursor_y = floor(line_y - extents.Y / 2), retail 0x009A2676..0x009A268F; the earlier draft centred line_x); `cell` read between `weight` and `weight_scale` (EVIDENCED: retail 0x009A2BF8 `mov ax,[ecx]` after the weight lea); font manager read through the shared headers with the PDB accessors GFGetSystemManager()->GetFontManager()->GetLoadFontGlyphsAsynchronously() (Ego_r + FableWin ?GetFontManager@CSystemManager@@QAEPAVCFontManager@@XZ / ?GetLoadFontGlyphsAsynchronously@CFontManager@@QBE_NXZ, inline in retail 0x009A25E2..0x009A25ED; landed sibling 0x00494900 uses the same pair).
// FITTED: declaration order inv_half_width before inv_half_height and verts before tex_verts (frame-packer ties of equal-weight 4-byte locals; retail slots 0x64/0x60 and 0x58/0x5C); the quad-index fill writes (unsigned short)(i * 4 + n) without a named `base` local (a named bool for the accessor result also matches): with a named `base` the inline accessor records push the SetRenderState bookmark load order at 0x009A291F one record late (c2 record-count tie; a named font_manager local, as 0x00494900 spells it, does the same). No device (c)/(d), no flags.
// Leaf: ?Render@CStreamingFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z
// Retail: Fable.exe 0x009A25D0, 3722 bytes, `ret 18h`.
#include <math.h>
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER
#define FABLE_RETAIL_CFONTMANAGER_ASYNC
#define FABLE_RETAIL_CFONTMANAGER_GETASYNC
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK_GLYPHLOOKUP
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CStreamingFontBank.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/C4DVector.h"
#include "engine/C2DVector.h"
#include "engine/CStreamingGlyphData.h"
#include "engine/CStreamingGlyphAdjustment.h"
#include "engine/CRGBColour.h"
#include "engine/CFontManager.h"
#include "engine/CShaderRenderManager.h"

class CStreamingGlyph {                 // Ego_r.pdb, 4 bytes
public:
    unsigned long GlyphIndex:24;
    unsigned long AdjustmentIndex:8;
};
struct CStreamingGlyphLookup {         // Ego_r.pdb, 16 bytes
    unsigned short font:4;
    unsigned short cachePos:12;
    unsigned short ch;
    CStreamingGlyph indices;
    CStreamingGlyphData data;
};
struct CFontCacheLayout { unsigned long CellsPerRow; unsigned long Rows; };
extern bool g_FontTextureCacheDirty;                   // 0x013C5A70
extern bool g_FontQuadIndicesDirty;                    // 0x0138E0B5
extern unsigned short g_FontQuadIndices[64 * 6];       // 0x013CA308
extern CFontCacheLayout g_FontCacheLayout;             // 0x013C5A98
extern CFontCacheLayout g_FontCacheLayoutSecond;       // 0x013C7AC8
namespace {
void __fastcall GFUpdateFontTextureCache();            // 0x009A23E0 ?GFUpdateFontTextureCache@?A0xfe46a155@@YIXXZ
extern CTexture GFontTextureCache;                     // 0x013C5A80 (Ego_r: dynamic initializer for 'GFontTextureCache')
extern CStreamingGlyphAdjustment* glyphTextureCacheGlyphAdjustment;   // 0x013BDA40 (landed 0x009A3640 spelling)
}

struct CFontVertex {
    C4DVector Pos; unsigned long Colour; float U, V;
    CFontVertex(const C4DVector& pos, unsigned long colour, float u, float v) : Pos(pos), Colour(colour), U(u), V(v) { }
};
struct CFontVertexTextured {
    C4DVector Pos; unsigned long Colour; float U, V, U2, V2;
    CFontVertexTextured(const C4DVector& pos, unsigned long colour, float u, float v, float u2, float v2) : Pos(pos), Colour(colour), U(u), V(v), U2(u2), V2(v2) { }
};

// Inline in retail (0x009A2696 / 0x009A2784 read the flags in place), as the landed row 0x00BA1B00 defines GetPixelShadersEnabled.
FABLE_FORCEINLINE bool CShaderRenderManager::GetVertexShadersEnabled() const { return VertexShadersEnabled; }
FABLE_FORCEINLINE bool CShaderRenderManager::GetPixelShadersEnabled() const { return PixelShadersEnabled; }

void CStreamingFontBank::Render(const CWideString& text, const C3DVector& pos, float scale, const CRGBColour& colour, unsigned long flags, const CTexture* texture) const
{
    if (!Preload(text, GFGetSystemManager()->GetFontManager()->GetLoadFontGlyphsAsynchronously()))
        return;
    float line_x = pos.X;
    float line_y = pos.Y;
    unsigned long col = colour.IntValue;
    float cursor_x = line_x;
    float cursor_y = line_y;
    CRenderManager& rm = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    if (g_FontTextureCacheDirty)
        GFUpdateFontTextureCache();
    if (flags & 8) {
        C2DExtentsF extents = GetStringExtents(text, false);
        cursor_y = (float)floor(line_y - extents.Y * 0.5f);
    }
    long stride = 0x1c;
    bool two_textures = false;
    if (!CShaderRenderManager::Get().GetVertexShadersEnabled()) {
        if (texture) {
            rm.SetVertexFormat(0x244);
            stride = 0x24;
            two_textures = true;
        } else
            rm.SetVertexFormat(0x144);
    }
    unsigned char vertex_buffer[0x2400];
    CFontVertex* verts = (CFontVertex*)vertex_buffer;
    CFontVertexTextured* tex_verts = (CFontVertexTextured*)vertex_buffer;
    if (g_FontQuadIndicesDirty) {
        unsigned short* index = g_FontQuadIndices;
        for (unsigned long i = 0; i < 64; ++i) {
            *index++ = (unsigned short)(i * 4);
            *index++ = (unsigned short)(i * 4 + 1);
            *index++ = (unsigned short)(i * 4 + 2);
            *index++ = (unsigned short)(i * 4 + 2);
            *index++ = (unsigned short)(i * 4 + 3);
            *index++ = (unsigned short)(i * 4);
        }
        g_FontQuadIndicesDirty = false;
    }
    rm.AttachTextureToStage(0, &GFontTextureCache);
    rm.AttachTextureToStage(1, texture);
    if (texture && !CShaderRenderManager::Get().GetPixelShadersEnabled()) {
        rm.SetTextureOperation(1, TEXTURE_OP_SELECT_ARG1);
        rm.SetTextureArgument1(1, TEXTURE_ARG_CURRENT);
        rm.SetTextureAlphaOperation(1, TEXTURE_OP_MODULATE);
        rm.SetTextureAlphaArgument1(1, TEXTURE_ARG_TEXTURE);
        rm.SetTextureAlphaArgument2(1, TEXTURE_ARG_CURRENT);
        rm.SetTextureOperation(2, TEXTURE_OP_DISABLE);
        rm.SetTextureAlphaOperation(2, TEXTURE_OP_DISABLE);
    }
    C2DExtentsI dims = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
    float inv_half_width = 1.0f / ((float)dims.X * 0.5f);
    float inv_half_height = 1.0f / ((float)dims.Y * 0.5f);
    unsigned long vertex_count = 0;
    long new_line = 1;
    const unsigned short* p = text.GetAsCharArray();
    while (*p) {
        if (new_line) {
            if (flags & 6) {
                C2DExtentsF extents = GetStringExtents(p, true);
                if (flags & 2)
                    cursor_x = (float)floor(line_x - extents.X);
                if (flags & 4)
                    cursor_x = (float)floor(line_x - extents.X * 0.5f);
            }
            new_line = 0;
        }
        unsigned short c = *p++;
        if (c == 10) {
            cursor_x = line_x;
            cursor_y += (float)FontHeight;
            new_line = 1;
            continue;
        }
        if (c == 0xa0)
            c = 0x20;
        const CStreamingGlyphLookup* glyph = GlyphLookup(c);
        if (!glyph)
            continue;
        const CStreamingGlyphAdjustment* weight = glyphTextureCacheGlyphAdjustment + glyph->indices.AdjustmentIndex;
        unsigned short cell = glyph->cachePos;
        float weight_scale = (float)weight->Scale * (1.0f / 64.0f);
        if (c == 0x20) {
            float advance = (float)(glyph->data.Advance) * weight_scale;
            cursor_x += (float)(signed char)(long)(advance < 0.0f ? advance - 0.5f : advance + 0.5f);
            continue;
        }
        const CFontCacheLayout* layout = glyph->data.BigChar ? &g_FontCacheLayoutSecond : &g_FontCacheLayout;
        unsigned long per_row = layout->CellsPerRow;
        float u = (float)(cell % per_row) / (float)per_row;
        float v = (float)(cell / per_row) / (float)layout->Rows;
        float glyph_scale = scale * weight_scale;
        float x0 = (cursor_x - line_x) * scale + (float)glyph->data.OffsetX * glyph_scale + line_x - 0.5f;
        float y0 = (cursor_y - line_y) * scale + (float)(weight->Baseline + glyph->data.OffsetY) * glyph_scale + line_y - 0.5f;
        float width = (float)glyph->data.Width;
        float x1 = width * glyph_scale + x0;
        float height = (float)glyph->data.Height;
        float y1 = height * glyph_scale + y0;
        float advance = (float)(glyph->data.Advance) * weight_scale;
        cursor_x += (float)(signed char)(long)(advance < 0.0f ? advance - 0.5f : advance + 0.5f);
        float u1 = width * (1.0f / 256.0f) + u;
        float v1 = height * (1.0f / 256.0f) + v;
        if (two_textures) {
            float su0 = (x0 * inv_half_width - 1.0f) * CStreamingFontBank::AlphaMaskUVMul.X + CStreamingFontBank::AlphaMaskUVAdd.X;
            float sv0 = (1.0f - y0 * inv_half_height) * CStreamingFontBank::AlphaMaskUVMul.Y + CStreamingFontBank::AlphaMaskUVAdd.Y;
            float su1 = (x1 * inv_half_width - 1.0f) * CStreamingFontBank::AlphaMaskUVMul.X + CStreamingFontBank::AlphaMaskUVAdd.X;
            float sv1 = (1.0f - y1 * inv_half_height) * CStreamingFontBank::AlphaMaskUVMul.Y + CStreamingFontBank::AlphaMaskUVAdd.Y;
            *tex_verts++ = CFontVertexTextured(C4DVector(x0, y1, pos.Z, 1.0f), col, u, v1, su0, sv1);
            *tex_verts++ = CFontVertexTextured(C4DVector(x0, y0, pos.Z, 1.0f), col, u, v, su0, sv0);
            *tex_verts++ = CFontVertexTextured(C4DVector(x1, y0, pos.Z, 1.0f), col, u1, v, su1, sv0);
            *tex_verts++ = CFontVertexTextured(C4DVector(x1, y1, pos.Z, 1.0f), col, u1, v1, su1, sv1);
        } else {
            *verts++ = CFontVertex(C4DVector(x0, y1, pos.Z, 1.0f), col, u, v1);
            *verts++ = CFontVertex(C4DVector(x0, y0, pos.Z, 1.0f), col, u, v);
            *verts++ = CFontVertex(C4DVector(x1, y0, pos.Z, 1.0f), col, u1, v);
            *verts++ = CFontVertex(C4DVector(x1, y1, pos.Z, 1.0f), col, u1, v1);
        }
        vertex_count += 4;
        if (vertex_count > 0xfc) {
            rm.DrawAsIndexedTris(vertex_buffer, stride, vertex_count, g_FontQuadIndices, vertex_count >> 1);
            tex_verts = (CFontVertexTextured*)vertex_buffer;
            verts = (CFontVertex*)vertex_buffer;
            vertex_count = 0;
        }
    }
    if (vertex_count > 0)
        rm.DrawAsIndexedTris(vertex_buffer, stride, vertex_count, g_FontQuadIndices, vertex_count >> 1);
    rm.AttachTextureToStage(0, 0);
    rm.AttachTextureToStage(1, 0);
}
// CStreamingFontBank::Render(const CWideString&, const C3DVector&, float, const CRGBColour&, unsigned long, const CTexture*) const -- draw a string from the streamed glyph cache as screen-space quads
// Grade: DIFFER(3715v3722) -- 282 difflines (2 are the normaliser printing AlphaMaskUVMul+4 as [4]): frame size now equal (0x25F0); the rest is local slot order (vertex count +0x2C, inverse half extents +0x64/+0x60, cursor and line origin), the CRenderStateManager bookmark test/or flip in the seven stage setters, and register choice in the glyph metric loads
// Next idea: declare the locals in retail's slot order (line origin, cursor, colour, count, new-line flag, inverse half width before height), keep width/height floats live as retail does (fst [1C] / fst [10]), and model the block-on-load flag through the real CSystemManager member at +0x84
// Leaf: ?Render@CStreamingFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z
#include <math.h>
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
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

class CRGBColour { public: unsigned long Colour; };
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
struct CFontManagerView { unsigned char _pad[0x38]; bool BlockOnLoad; };
struct CShaderEnableView { unsigned char _pad[0x390]; bool VertexShadersEnabled; bool PixelShadersEnabled; };
extern CShaderEnableView g_ShaderRenderManager;        // 0x013BC470
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

void CStreamingFontBank::Render(const CWideString& text, const C3DVector& pos, float scale, const CRGBColour& colour, unsigned long flags, const CTexture* texture) const
{
    bool block = (*(CFontManagerView**)((char*)GFGetSystemManager() + 0x84))->BlockOnLoad;
    if (!Preload(text, block))
        return;
    float line_x = pos.X;
    float line_y = pos.Y;
    unsigned long col = colour.Colour;
    float cursor_x = line_x;
    float cursor_y = line_y;
    CRenderManager& rm = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    if (g_FontTextureCacheDirty)
        GFUpdateFontTextureCache();
    if (flags & 8) {
        C2DExtentsF extents = GetStringExtents(text, false);
        line_x = (float)floor(line_x - extents.X * 0.5f);
    }
    long stride = 0x1c;
    bool two_textures = false;
    if (!g_ShaderRenderManager.VertexShadersEnabled) {
        if (texture) {
            rm.SetVertexFormat(0x244);
            stride = 0x24;
            two_textures = true;
        } else
            rm.SetVertexFormat(0x144);
    }
    unsigned char vertex_buffer[0x2400];
    CFontVertexTextured* tex_verts = (CFontVertexTextured*)vertex_buffer;
    CFontVertex* verts = (CFontVertex*)vertex_buffer;
    if (g_FontQuadIndicesDirty) {
        unsigned short* index = g_FontQuadIndices;
        for (unsigned long i = 0; i < 64; ++i) {
            unsigned short base = (unsigned short)(i * 4);
            *index++ = base;
            *index++ = base + 1;
            *index++ = base + 2;
            *index++ = base + 2;
            *index++ = base + 3;
            *index++ = base;
        }
        g_FontQuadIndicesDirty = false;
    }
    rm.AttachTextureToStage(0, &GFontTextureCache);
    rm.AttachTextureToStage(1, texture);
    if (texture && !g_ShaderRenderManager.PixelShadersEnabled) {
        rm.SetTextureOperation(1, TEXTURE_OP_SELECT_ARG1);
        rm.SetTextureArgument1(1, TEXTURE_ARG_CURRENT);
        rm.SetTextureAlphaOperation(1, TEXTURE_OP_MODULATE);
        rm.SetTextureAlphaArgument1(1, TEXTURE_ARG_TEXTURE);
        rm.SetTextureAlphaArgument2(1, TEXTURE_ARG_CURRENT);
        rm.SetTextureOperation(2, TEXTURE_OP_DISABLE);
        rm.SetTextureAlphaOperation(2, TEXTURE_OP_DISABLE);
    }
    C2DExtentsI dims = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
    float inv_half_height = 1.0f / ((float)dims.Y * 0.5f);
    float inv_half_width = 1.0f / ((float)dims.X * 0.5f);
    unsigned long vertex_count = 0;
    long new_line = 1;
    const wchar_t* p = text.GetAsCharArray();
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
        wchar_t c = *p++;
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
        float weight_scale = (float)weight->Scale * (1.0f / 64.0f);
        unsigned short cell = glyph->cachePos;
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

// CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, long blend_mode, CTexture* texture) -- queue a screen-space line: both vertices converted to viewport space (x and y to -1..1, z 0, rhw 1) on the vertex list, then a Line record (window, blend mode, texture) on the quick-draw list, unless the viewport is offscreen
// Leaf: ?Add2DLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DC2C0, length 509 bytes (`ret 10h`). Ego_r.pdb 0x00B2FE90 / FableWin.pdb
//   ?Add2DLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z (private).
// Callees: CRenderManagerCore::GetCurrentAWindow() const 0x00A0AAA0 (Ego_r 0x00B74050; once per converted coordinate and once for the
//   window), the vertex vector's _M_insert_overflow 0x009E14B0, CWideString() 0x0099AED0, CTexture::operator= 0x009FA1C0, CQuickDrawTriInfo
//   copy ctor 0x009DE890 (landed), the record vector's _M_insert_overflow 0x009E1750, CTexture::Uninitialise 0x009F9F70 (inline ~CTexture),
//   ~CWideString 0x0099B510.
// Layout: engine/CRenderManager2D.h retail branch (QuickDraw2DVertices +0x3E88, QuickDraw2DTriInfo +0x3E94).
// Shape: one vertex local (rep movsd copy) reused for both ends; each coordinate goes through an inline helper that takes the
//   coordinate by const reference and reads the current window once: x / width * 2 - 1 (`fadd st,st`), 1 - y / height * 2; z 0,
//   rhw 1. Inline STLport push_backs. The record: blend mode, the texture copy when one is given, the window, Line set / IsText
//   cleared, pushed, then its destructors. ViewportXFromScreenX / ViewportYFromScreenY are descriptive names (no PDB, FSE or
//   EgoCore record of the helpers).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_ADD2DLINE
#define FABLE_RETAIL_CQUICKDRAWTRIINFO_COPY
#define FABLE_RETAIL_CDISPLAYMANAGER_ISVIEWPORTOFFSCREEN
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CTEXTURE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CTexture.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CQuickDrawTriInfo.h"
#include "engine/CRenderManager2D.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

static inline float ViewportXFromScreenX(const CRenderManagerCore& manager, const float& x)
{
    const C2DBoxF& box = manager.GetCurrentAWindow();
    return x / (box.BRX - box.TLX) * 2.0f - 1.0f;
}
static inline float ViewportYFromScreenY(const CRenderManagerCore& manager, const float& y)
{
    const C2DBoxF& box = manager.GetCurrentAWindow();
    return 1.0f - y / (box.BRY - box.TLY) * 2.0f;
}

void CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, long blend_mode,
                                 CTexture* texture)
{
    if (GetDisplayManager().IsViewportOffscreen())
        return;
    CTVertexRHWColSpecTex1 vertex = v0;
    vertex.X = ViewportXFromScreenX(*this, vertex.X);
    vertex.Y = ViewportYFromScreenY(*this, vertex.Y);
    vertex.Z = 0.0f;
    vertex.RHW = 1.0f;
    QuickDraw2DVertices.push_back(vertex);
    vertex = v1;
    vertex.X = ViewportXFromScreenX(*this, vertex.X);
    vertex.Y = ViewportYFromScreenY(*this, vertex.Y);
    vertex.Z = 0.0f;
    vertex.RHW = 1.0f;
    QuickDraw2DVertices.push_back(vertex);
    CQuickDrawTriInfo info;
    info.BlendMode = blend_mode;
    if (texture)
        info.UploadedTexture = *texture;
    info.Window = GetCurrentAWindow();
    info.Line = true;
    info.IsText = false;
    QuickDraw2DTriInfo.push_back(info);
}

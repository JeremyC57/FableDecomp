// CRenderManager2D::AddLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, long blend_mode, CTexture* texture) -- queue a world-overlay line: both vertices offset by the viewport's top-left corner and converted to viewport space (x and y to -1..1, rhw 1) on the line vertex list, then a Line record (window, blend mode, texture) on the line-record list, unless the viewport is offscreen
// Leaf: ?AddLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DBAE0, length 557 bytes (`ret 10h`). Ego_r.pdb 0x00B2F4C0 / FableWin.pdb
//   ?AddLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z (private).
// Callees: CRenderManagerCore::GetCurrentAWindow() const 0x00A0AAA0 (Ego_r 0x00B74050; twice per converted coordinate and once for the
//   window), the vertex vector's _M_insert_overflow 0x009E14B0, CWideString() 0x0099AED0, CTexture::operator= 0x009FA1C0, CQuickDrawTriInfo
//   copy ctor 0x009DE890 (landed), the record vector's _M_insert_overflow 0x009E1750, CTexture::Uninitialise 0x009F9F70 (inline ~CTexture),
//   ~CWideString 0x0099B510.
// Layout: engine/CRenderManager2D.h retail branch (QuickDrawVertices +0x3E58, QuickDrawTriInfo +0x3E64).
// Shape: one vertex local (rep movsd copy) reused for both ends; each coordinate goes through an inline helper that takes the
//   coordinate by const reference and reads the current window twice: (TLX + x) / width * 2 - 1 (`fadd st,st`), 1 - (y + TLY) / height * 2;
//   rhw 1 (z kept). Inline STLport push_backs. The record: blend mode, the texture copy when one is given, the window, Line set / IsText
//   cleared, pushed, then its destructors. ViewportXFromWindowX / ViewportYFromWindowY are descriptive names (no PDB, FSE or
//   EgoCore record of the helpers).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_ADDLINE
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

static inline float ViewportXFromWindowX(const CRenderManagerCore& manager, const float& x)
{
    float origin_x = manager.GetCurrentAWindow().TLX;
    const C2DBoxF& box = manager.GetCurrentAWindow();
    return (origin_x + x) / (box.BRX - box.TLX) * 2.0f - 1.0f;
}
static inline float ViewportYFromWindowY(const CRenderManagerCore& manager, const float& y)
{
    const C2DBoxF& origin = manager.GetCurrentAWindow();
    const C2DBoxF& box = manager.GetCurrentAWindow();
    return 1.0f - (y + origin.TLY) / (box.BRY - box.TLY) * 2.0f;
}

void CRenderManager2D::AddLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, long blend_mode,
                               CTexture* texture)
{
    if (GetDisplayManager().IsViewportOffscreen())
        return;
    CTVertexRHWColSpecTex1 vertex = v0;
    vertex.X = ViewportXFromWindowX(*this, vertex.X);
    vertex.Y = ViewportYFromWindowY(*this, vertex.Y);
    vertex.RHW = 1.0f;
    QuickDrawVertices.push_back(vertex);
    vertex = v1;
    vertex.X = ViewportXFromWindowX(*this, vertex.X);
    vertex.Y = ViewportYFromWindowY(*this, vertex.Y);
    vertex.RHW = 1.0f;
    QuickDrawVertices.push_back(vertex);
    CQuickDrawTriInfo info;
    info.BlendMode = blend_mode;
    if (texture)
        info.UploadedTexture = *texture;
    info.Window = GetCurrentAWindow();
    info.Line = true;
    info.IsText = false;
    QuickDrawTriInfo.push_back(info);
}

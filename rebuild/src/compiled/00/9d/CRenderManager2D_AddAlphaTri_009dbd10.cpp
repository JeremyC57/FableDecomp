// CRenderManager2D::AddAlphaTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2, long blend_mode, CTexture* texture) -- queue a world-overlay alpha triangle: the three vertices offset by the viewport's top-left corner and converted to viewport space on the alpha vertex list, then a triangle record (window, blend mode, texture, average depth) on the alpha record list, unless the viewport is offscreen
// Leaf: ?AddAlphaTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DBD10, length 734 bytes (`ret 14h`). Ego_r.pdb 0x00B2F760 / FableWin.pdb
//   ?AddAlphaTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z (private).
// Callees: CRenderManagerCore::GetCurrentAWindow() const 0x00A0AAA0 (Ego_r 0x00B74050; twice per converted coordinate and once for the
//   window), the vertex vector's _M_insert_overflow 0x009E14B0, CWideString() 0x0099AED0, CTexture::operator= 0x009FA1C0, CQuickDrawTriInfo
//   copy ctor 0x009DE890 (landed), the record vector's _M_insert_overflow 0x009E1750, CTexture::Uninitialise 0x009F9F70 (inline ~CTexture),
//   ~CWideString 0x0099B510.
// Layout: engine/CRenderManager2D.h retail branch (QuickDrawAlphaVertices +0x3E70, QuickDrawAlphaTriInfo +0x3E7C).
// Shape: one vertex local (rep movsd copy) reused for the three corners; each coordinate goes through an inline helper that takes the
//   coordinate by const reference and reads the current window twice: (TLX + x) / width * 2 - 1 (`fadd st,st`), 1 - (y + TLY) / height * 2;
//   z and rhw kept; the record also takes ZAverage = (v0.Z + v1.Z + v2.Z) * (1/3). Inline STLport push_backs. The record: blend mode, the texture copy when one is given, the window, Line and IsText
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

void CRenderManager2D::AddAlphaTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                              long blend_mode, CTexture* texture)
{
    if (GetDisplayManager().IsViewportOffscreen())
        return;
    CTVertexRHWColSpecTex1 vertex = v0;
    vertex.X = ViewportXFromWindowX(*this, vertex.X);
    vertex.Y = ViewportYFromWindowY(*this, vertex.Y);
    QuickDrawAlphaVertices.push_back(vertex);
    vertex = v1;
    vertex.X = ViewportXFromWindowX(*this, vertex.X);
    vertex.Y = ViewportYFromWindowY(*this, vertex.Y);
    QuickDrawAlphaVertices.push_back(vertex);
    vertex = v2;
    vertex.X = ViewportXFromWindowX(*this, vertex.X);
    vertex.Y = ViewportYFromWindowY(*this, vertex.Y);
    QuickDrawAlphaVertices.push_back(vertex);
    CQuickDrawTriInfo info;
    info.BlendMode = blend_mode;
    if (texture)
        info.UploadedTexture = *texture;
    info.Window = GetCurrentAWindow();
    info.Line = false;
    info.IsText = false;
    info.ZAverage = (v0.Z + v1.Z + v2.Z) * 0.33333334f;
    QuickDrawAlphaTriInfo.push_back(info);
}

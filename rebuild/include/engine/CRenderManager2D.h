#ifndef FABLE_ENGINE_CRENDERMANAGER2D_H
#define FABLE_ENGINE_CRENDERMANAGER2D_H
// Retail CRenderManager2D (movie row 0x006286F0). RTTI (vtable 0x0129B9EC) CRenderManager2D : CRenderManagerCore, offset 0.
// Size: the only constructor 0x009D9A30 calls ??0CRenderManagerCore (0x00A0AFE0), stores vtable 0x0129B9EC and constructs
// its own members from +0x3D20 through +0x3E9C; CRenderManager (engine/CRenderManager.h) adds no member, and its creator
// 0x009C0FD0 allocates 0x3EA4 bytes. The own members (Ego_r.pdb 0x21C bytes with Dinkumware containers) are opaque.
// Member functions: Ego_r.pdb + FableWin.pdb spellings; retail addresses follow the Ego_r donor order
// (DrawBufferedPolys 0x00B2D610 < Render2DDrawList 0x00B2E390 < Draw2DTexture(UV) 0x00B30450).
#ifndef FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#endif
#include "engine/CRenderManagerCore.h"
class C2DBoxF; class CUVBox; class CRGBColour; class CTexture;
#ifdef FABLE_RETAIL_CRENDERMANAGER2D
// Retail branch members (2026-11-03 landing b, Codex pass 13 row 0x009DA9F0 Render2DDrawList; Codex's FABLE_RETAIL_RENDER2D_TYPES
// model folded in): Ego_r.pdb names in Ego_r order with STLport sizes (vector 0xC, CShaderDataBank 0x128 in retail): VertexShaderBank
// +0x3D20 (CShaderDataBank, opaque here: no landed row needs its type; Codex's typed CShaderDataBank / CBankFile models are listed
// in batches/2026-11-03_landing_b/HEADER_NEEDS.md), VertexShaderResource +0x3E48, VertexShader2D +0x3E50, the six quick-draw
// vectors +0x3E58..+0x3E94, AlternateRenderTargetClearEnabled +0x3EA0; the class statics FlipTable and
// ?LibraryStateBlockFunctions@CRenderManager2D@@0PAPAVCStateBlockFunctionBase@@A (0x01396510, Codex's PDB DATA citation).
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShader.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CQuickDrawTriInfo.h"
#include "engine/CStateBlockFunctionBase.h"
class C2DVector; class C3DVector; class CFontBank; class CShaderResource; class CGraphicDataBank; class CSurface; class CGraphicFrame;
#endif
class CRenderManager2D : public CRenderManagerCore {
private:
#ifdef FABLE_RETAIL_CRENDERMANAGER2D
    unsigned char VertexShaderBank[0x128];                               // +0x3D20 CShaderDataBank (opaque)
    CCountedPointer<CShaderResource> VertexShaderResource;               // +0x3E48
    CVertexShader VertexShader2D;                                        // +0x3E50
    std::vector<CTVertexRHWColSpecTex1,std::allocator<CTVertexRHWColSpecTex1> > QuickDrawVertices;       // +0x3E58
    std::vector<CQuickDrawTriInfo,std::allocator<CQuickDrawTriInfo> > QuickDrawTriInfo;                   // +0x3E64
    std::vector<CTVertexRHWColSpecTex1,std::allocator<CTVertexRHWColSpecTex1> > QuickDrawAlphaVertices;  // +0x3E70
    std::vector<CQuickDrawTriInfo,std::allocator<CQuickDrawTriInfo> > QuickDrawAlphaTriInfo;              // +0x3E7C
    std::vector<CTVertexRHWColSpecTex1,std::allocator<CTVertexRHWColSpecTex1> > QuickDraw2DVertices;     // +0x3E88
    std::vector<CQuickDrawTriInfo,std::allocator<CQuickDrawTriInfo> > QuickDraw2DTriInfo;                 // +0x3E94
    static const unsigned long FlipTable[3][4];
    static CStateBlockFunctionBase* LibraryStateBlockFunctions[7];       // 0x01396510
    bool AlternateRenderTargetClearEnabled;                              // +0x3EA0
#else
    unsigned char _pad_0x3d20[0x3ea4 - 0x3d20];                          // +0x3D20 own members (opaque)
#endif
public:
    void DrawBufferedPolys();                                            // 0x009D9C80 ?DrawBufferedPolys@CRenderManager2D@@QAEXXZ
    void Render2DDrawList(bool);                                         // 0x009DA9F0 ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z
    bool Draw2DTexture(const C2DBoxF&,const CUVBox&,const CRGBColour&,CTexture&,unsigned long);
                                                                         // 0x009DC870 ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCUVBox@@ABVCRGBColour@@AAVCTexture@@K@Z
#ifdef FABLE_RETAIL_CRENDERMANAGER2D
    // Retail opt-in (chunk 718, row 0x00487570 CPlayer::DrawFreeCamDebug): FableWin.pdb ?Draw2DText@CRenderManager2D@@QAE_NABVC2DVector@@VCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z
    // (the landed body 0x009DD8F0, `ret 14h`; the string travels by value, built in its argument slot). A declaration only.
    bool Draw2DText(const C2DVector&,CWideString,const CFontBank&,const CRGBColour&,unsigned long);   // 0x009DD8F0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CRenderManager2D) == 0x3ea4);
#endif // FABLE_ENGINE_CRENDERMANAGER2D_H

#ifdef FABLE_RETAIL_RENDER2D_TYPES
#ifndef FABLE_ENGINE_CRENDERMANAGER2D_RENDER2D_H
#define FABLE_ENGINE_CRENDERMANAGER2D_RENDER2D_H
#include "rebuild_abi.h"
class C2DVector;class C3DVector;class CUVBox;class CGraphicDataBank;class CSurface;class CGraphicFrame;
#include "engine/CRenderManagerCore.h"
#include "engine/CShaderDataBank.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShader.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CQuickDrawTriInfo.h"
#include "engine/CStateBlockFunctionBase.h"
class CShaderResource;
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CRenderManager2D : public CRenderManagerCore {
private: CShaderDataBank VertexShaderBank;
private: CCountedPointer<CShaderResource> VertexShaderResource;
private: CVertexShader VertexShader2D;
private: std::vector<CTVertexRHWColSpecTex1,std::allocator<CTVertexRHWColSpecTex1> > QuickDrawVertices;
private: std::vector<CQuickDrawTriInfo,std::allocator<CQuickDrawTriInfo> > QuickDrawTriInfo;
private: std::vector<CTVertexRHWColSpecTex1,std::allocator<CTVertexRHWColSpecTex1> > QuickDrawAlphaVertices;
private: std::vector<CQuickDrawTriInfo,std::allocator<CQuickDrawTriInfo> > QuickDrawAlphaTriInfo;
private: std::vector<CTVertexRHWColSpecTex1,std::allocator<CTVertexRHWColSpecTex1> > QuickDraw2DVertices;
private: std::vector<CQuickDrawTriInfo,std::allocator<CQuickDrawTriInfo> > QuickDraw2DTriInfo;
private: static const unsigned long FlipTable[3][4];
private: static CStateBlockFunctionBase * LibraryStateBlockFunctions[7];
private: bool AlternateRenderTargetClearEnabled;
public: CRenderManager2D(CDisplayManager &,const CWideString &,unsigned long);
public: void DrawBufferedPolys();
public: void Render2DDrawList(bool);
public: void AddTriToDrawBuffer(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,unsigned long,CTexture *);
public: void AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,unsigned long,CTexture *);
public: void AddLineToDrawBuffer(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,unsigned long,CTexture *);
public: bool Draw2DText(const C2DVector &,CWideString,const CFontBank &,const CRGBColour &,unsigned long);
public: bool Draw3DScaledText(const C3DVector &,float,CWideString,const CFontBank &,const CRGBColour &,unsigned long);
public: bool Draw2DFormattedText(const C2DVector &,CWideString,const CFontBank &,const CRGBColour &,unsigned long,unsigned long);
public: bool Draw2DTextureTest2(const C2DVector *,const CRGBColour &,CTexture &,unsigned long);
public: bool Draw2DTexture(const C2DBoxF &,const CUVBox &,const CRGBColour &,CTexture &,unsigned long);
public: bool Draw2DTexture(const C2DVector *,const CRGBColour &,CTexture &,unsigned long);
public: bool Draw2DTexture(const C2DBoxF &,const CRGBColour &,CTexture &,unsigned long);
public: bool Draw2DGraphic(const C2DVector &,const CGraphicDataBank &,unsigned long,unsigned long,unsigned long,unsigned long);
public: bool Draw2DSurface(const C2DVector &,CTexture &,const CRGBColour &,unsigned long);
public: bool Draw2DSurface(const C2DVector *,CTexture &,const CRGBColour &,unsigned long);
public: bool Draw2DScaledGraphic(const C2DBoxF &,const CGraphicDataBank &,unsigned long,unsigned long,unsigned long);
public: bool Draw2DScaledGraphicWithUVs(const C2DBoxF &,const C2DBoxF &,const CGraphicDataBank &,unsigned long,unsigned long,unsigned long);
public: bool Draw2DLine(const C2DVector &,const C2DVector &,const CRGBColour &,unsigned long);
public: bool Draw2DCircle(const C2DVector &,float,const CRGBColour &,unsigned long);
public: bool Draw2DBox(const C2DBoxF &,const CRGBColour &,unsigned long);
public: bool Draw2DBoxColours(const C2DBoxF &,const CRGBColour *,unsigned long);
public: bool Draw2DBoxWithZ(const C2DBoxF &,float,const CRGBColour &,unsigned long);
public: bool Draw2DBoxOutline(const C2DBoxF &,const CRGBColour &,unsigned long);
public: bool Draw2DTriangle(const C2DVector &,const C2DVector &,const C2DVector &,const CRGBColour &,unsigned long);
public: void ClearRenderTarget(const CRGBColour &,unsigned long);
private: void Add2DLine(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,long,CTexture *);
private: void Add2DTri(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,long,CTexture *);
private: void AddText(const C3DVector &,float,const CWideString &,const CFontBank &,const CRGBColour &,unsigned long);
private: void AddTri(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,long,CTexture *);
private: void AddLine(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,long,CTexture *);
private: void AddAlphaTri(const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,const CTVertexRHWColSpecTex1 &,long,CTexture *);
private: void SetupD3DUVBoxFromSurface(const CSurface &,CUVBox &);
private: void SetupD3DUVBoxFromGraphic(const CGraphicFrame &,CUVBox &);
};
FABLE_STATIC_ASSERT(sizeof(CBankFile)==0x110);
FABLE_STATIC_ASSERT(sizeof(CShaderDataBank)==0x128);
FABLE_STATIC_ASSERT(sizeof(CRenderManagerCore)==0x3d20);
FABLE_STATIC_ASSERT(sizeof(CRenderManager2D)==0x3ea4);
#endif
#else
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
class CRenderManager2D : public CRenderManagerCore {
private:
    unsigned char _pad_0x3d20[0x3ea4 - 0x3d20];                          // +0x3D20 own members (opaque)
public:
    void DrawBufferedPolys();                                            // 0x009D9C80 ?DrawBufferedPolys@CRenderManager2D@@QAEXXZ
    void Render2DDrawList(bool);                                         // 0x009DA9F0 ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z
    bool Draw2DTexture(const C2DBoxF&,const CUVBox&,const CRGBColour&,CTexture&,unsigned long);
                                                                         // 0x009DC870 ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCUVBox@@ABVCRGBColour@@AAVCTexture@@K@Z
};
FABLE_STATIC_ASSERT(sizeof(CRenderManager2D) == 0x3ea4);
#endif // FABLE_ENGINE_CRENDERMANAGER2D_H

#endif

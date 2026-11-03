#ifndef FABLE_ENGINE_CRENDERMANAGER2D_H
#define FABLE_ENGINE_CRENDERMANAGER2D_H
#include "engine/CRenderManagerCore.h"
class CWideString;class C2DVector;class CFontBank;class CRGBColour;
// Layout: original full hierarchy; native ctor009d9a30 proves offsets and STLport vector sizes.
class CRenderManager2D:public CRenderManagerCore {
public:
 CRenderManager2D(CDisplayManager&,const CWideString&,unsigned long);virtual ~CRenderManager2D();
 bool Draw2DText(const C2DVector&,CWideString,const CFontBank&,const CRGBColour&,unsigned long);
private:
 unsigned char VertexShaderBank[0x128]; // CShaderDataBank+3d20.
 unsigned char VertexShaderResource[8]; // CCountedPointer<CShaderResource>+3e48.
 unsigned char VertexShader2D[8]; // CVertexShader+3e50.
 unsigned char QuickDrawVertices[12],QuickDrawTriInfo[12],QuickDrawAlphaVertices[12],QuickDrawAlphaTriInfo[12],QuickDraw2DVertices[12],QuickDraw2DTriInfo[12]; // original vectors+3e58..3e94.
 bool AlternateRenderTargetClearEnabled;
};
FABLE_STATIC_ASSERT(sizeof(CRenderManager2D)==0x3ea4);
#endif

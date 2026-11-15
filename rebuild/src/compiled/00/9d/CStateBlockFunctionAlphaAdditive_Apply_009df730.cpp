// CStateBlockFunctionAlphaAdditive::Apply() -- configure additive textured rendering.
// Leaf: ?Apply@CStateBlockFunctionAlphaAdditive@@UAEXXZ
// Retail: 0x009DF730,1633 bytes,public virtual thiscall.
// Flags: none
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CSTATEBLOCKFUNCTIONALPHAADDITIVE_PASS22
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CStateBlockFunctionAlphaAdditive.h"

void CStateBlockFunctionAlphaAdditive::Apply()
{
    CRenderManagerCore* rm=&GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    rm->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    rm->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    rm->SetZBufferEnable(true);
    rm->SetZBufferWriteEnable(false);
    rm->SetAlphaBlendEnable(true);
    rm->SetAlphaTestEnable(false);
    rm->SetTextureArgument1(0,TEXTURE_ARG_TEXTURE);
    rm->SetTextureArgument2(0,TEXTURE_ARG_DIFFUSE);
    rm->SetTextureAlphaArgument1(0,TEXTURE_ARG_TEXTURE);
    rm->SetTextureAlphaArgument2(0,TEXTURE_ARG_DIFFUSE);
    rm->SetTextureOperation(0,TEXTURE_OP_MODULATE);
    rm->SetTextureAlphaOperation(0,TEXTURE_OP_MODULATE);
    rm->SetTextureOperation(1,TEXTURE_OP_DISABLE);
    rm->SetTextureAlphaOperation(1,TEXTURE_OP_DISABLE);
    rm->SetTextureFilterModeMin(0,TEXTURE_FILTER_LINEAR);
    rm->SetTextureFilterModeMag(0,TEXTURE_FILTER_LINEAR);
}
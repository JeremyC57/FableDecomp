// 00bc6930 CEnginePrimitiveManagerText::RenderPrimitive
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerText@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
// Flags:
#define FABLE_RETAIL_PROJECT_TEXT_PASS27
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PORTABLE_NEW_PASS23
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_VIEWTOSCREEN
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineInternalPrimitiveText.h"
#include "engine/CEnginePrimitiveManagerText.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEngineSubPrimitiveText.h"
#include "engine/CDisplayManager.h"
#include "engine/CFontBank.h"

namespace NEngineInternal {
    extern CEngineCamera* GPCamera;
    extern CDisplayManager* GPDisplayManager;
    extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;
}
inline CEngineSubPrimitiveText::CEngineSubPrimitiveText(CEngineInternalPrimitiveText* parent)
    : CEngineSubPrimitiveBase(parent) {}
inline void CEnginePrimitiveManager::AddSubPrimitive(CEngineSubPrimitiveBase* sub,float depth,unsigned char alpha,unsigned long flags)
{
    NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(sub,depth,alpha,flags,this);
}

ERendered CEnginePrimitiveManagerText::RenderPrimitive(CEngineInternalPrimitiveBase& primitive_base,EEngineRenderLayer layer,CEngineSubPrimitiveChain& subprim_chain)
{
    CEngineInternalPrimitiveText& primitive=static_cast<CEngineInternalPrimitiveText&>(primitive_base);
    C3DVector view_pos(primitive.Pos.X,primitive.Pos.Y,primitive.Pos.Z);
    NEngineInternal::GPCamera->TransformWorldToView(view_pos);
    if(view_pos.Z>0.0001f) {
        C3DVector screen_pos;
        NEngineInternal::GPCamera->TransformViewToScreen(view_pos,screen_pos);
        float scale=primitive.Scale;
        if(primitive.ScaleIn3D) {
            scale/=view_pos.Z;
            scale/=primitive.Font->GetFontMaxHeight();
            scale*=NEngineInternal::GPDisplayManager->GetBackBuffer().GetWidth()>>1;
        }
        C3DVector coord;
        coord.X=screen_pos.X;
        coord.Y=screen_pos.Y;
        coord.Z=screen_pos.Z;
        CEngineSubPrimitiveText* subprim=new CEngineSubPrimitiveText(&primitive);
        subprim->Pos=coord;
        subprim->Scale=scale;
        subprim->Colour=primitive.Colour;
        subprim->Text=primitive.Text;
        subprim->Font=primitive.Font;
        subprim->Mask=0;
        AddSubPrimitive(subprim,screen_pos.Z,primitive.Colour.A,0);
    }
    return RENDERED_OBJECT;
}
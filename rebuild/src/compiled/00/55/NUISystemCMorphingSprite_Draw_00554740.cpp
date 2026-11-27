// 00554740 NUISystem::CMorphingSprite::Draw
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_MORPHING_SPRITE_DRAW_PASS29
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
// Leaf: ?Draw@CMorphingSprite@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER
// Retail-asm: 0x005549B5 `fld dword ptr [esp+14h]` + 0x005549B9 `fistp dword ptr [esp+18h]` and 0x005549CB `fistp dword ptr [esp+28h]` -- the engine's inline-asm float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h, Retail-asm-helper).
#include "engine/CCountedPointer.h"
class CGraphicDataBank;
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline CCountedPointer<CGraphicDataBank>::~CCountedPointer(){ReleaseData();}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "fable_compiler.h"
#include "engine/CIVCountedPointer.h"
class CEnginePersistentPrimitiveBase;
// Inline-budget: native 0041C500 expands the primitive owner release, including clearing the handle on each path.
template<> FABLE_FORCEINLINE CIVCountedPointer<CEnginePersistentPrimitiveBase>::~CIVCountedPointer(){Release();}
#include "engine/NUISystem_CMorphingSprite.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/GFFloatToLongNear.h"
namespace NUISystem {
void CMorphingSprite::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent)
{
    if (RenderColour.A > 0 && (!(RenderZoom.X <= 0.0f) || !(RenderZoom.Y <= 0.0f))) {
        NoRenderNextFrame = 0;
    } else {
        if (NoRenderNextFrame == 1) {
            if (RenderCount == 0) {
                if (parentHandle == 0) {
                    Handle.Clear();
                    return;
                }
            } else {
                if (Handles.size() >= (unsigned long)RenderCount) {
                    std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin();
                    const std::list<CEnginePrimitiveHandle*>::iterator it_end = Handles.end();
                    for (unsigned long i = 0; i < (unsigned long)(RenderCount - 1) && it != it_end; ++i)
                        ++it;
                    (*it)->Clear();
                    delete *it;
                    Handles.erase(it);
                } else {
                    Handles.push_back(new CEnginePrimitiveHandle);
                }
                if (parentHandle == 0)
                    return;
            }
            engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
            return;
        }
        NoRenderNextFrame = 1;
    }
    CEnginePrimitiveHandle* drawHandle;
    if (RenderCount > 0) {
        if (Handles.size() < (unsigned long)RenderCount) {
            drawHandle = new CEnginePrimitiveHandle;
            Handles.push_back(drawHandle);
        } else {
            std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin();
            const std::list<CEnginePrimitiveHandle*>::iterator it_end = Handles.end();
                    for (unsigned long i = 0; i < (unsigned long)(RenderCount - 1) && it != it_end; ++i)
                ++it;
            drawHandle = *it;
        }
    } else {
        drawHandle = &Handle;
    }
    ++RenderCount;
    C2DVector position((float)GFFloatToLongNear(RelativeRenderPosition.X), (float)GFFloatToLongNear(RelativeRenderPosition.Y));
    C2DVector size(Size.X * RelativeRenderZoom.X, Size.Y * RelativeRenderZoom.Y);
    C2DBoxF box(position.X, position.Y, position.X + size.X, position.Y + size.Y);
    CRGBColour colour = RenderColour;
    bool independent = IsIndependent();
    bool layerIndependent = IsLayerIndependent();
    EEngine2DLayer engineLayer;
    if (!independent && !layerIndependent)
        engineLayer = CManager::GetInstance()->GetLayer(Layer + layer);
    else
        engineLayer = CManager::GetInstance()->GetLayer(Layer);
    unsigned long graphicIndex = GraphicIndices[TargetState];
    if (SpriteGraphicIndex != 0)
        graphicIndex = SpriteGraphicIndex;
    if (graphicIndex > 0) {
        const CGraphicInfoFrame* frame = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(graphicIndex, 0);
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphicFrame;
        frame->GetFrame(graphicFrame, 0, 0x11);
    }
    CEnginePrimitive2DSprite sprite(0, box, engineLayer, graphicIndex, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    if (parentHandle && childIndex)
        engine.SetChildPrimitive(*parentHandle, (*childIndex)++, sprite, 0xc0);
    else
        engine.SetPersistentPrimitive(*drawHandle, sprite, 0xc0);
}
}
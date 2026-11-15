// 0077c4d0 NGraphicAppearance::CTCGraphicAppearanceSprite::OnAppearanceDraw
// NGraphicAppearance::CTCGraphicAppearanceSprite::OnAppearanceDraw(...) -- interpolate and submit a persistent sprite.
// 0x0077C4D0 whole secondary-base virtual; genuine constructor helper0x006558CE independently verified.
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceSprite@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Retail: 0x0077C4D0,611 bytes,thiscall through component subobject.
// Flags: none
// Inferred: scoped source_alpha preserves native0x0077C622 FILD and FMULP; direct products fold to FIMUL.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PASS22_GRAPHICBANK
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCESPRITE_PASS22
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_SPRITE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearanceSprite.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitiveSprite.h"
#include "engine/C3DPositionInterpolator.h"
// Original constructor parameters: donor01D8E9A0 BPREL angle,size_x,size_y; native006558CE.
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector& world_pos,unsigned long bank_index,unsigned long frame,unsigned short flags,float angle,float size_x,float size_y,EEnginePrimitiveBlendMode blend_mode,unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,float fade_start,EEngineInterpolationSet interp_set,EEnginePrimitiveBlendOp blend_op)
    : CEnginePrimitive((EEnginePrimitiveType)7,interp_set),WorldPos(world_pos),BankIndex(bank_index),Frame(frame),SizeX(size_x),SizeY(size_y),FadeStart(fade_start),Angle(angle),Colour(r,g,b,alpha),Flags(flags),BlendMode(blend_mode),BlendOp(blend_op),Offset(0.0f,0.0f)
{
}
#ifdef _MSC_VER
#pragma optimize("t", on)
#pragma optimize("y", on)
#endif
namespace NGraphicAppearance {
bool CTCGraphicAppearanceSprite::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
    const CEngineGraphic& graphic=tc_appearance.GetMainGraphic();
    unsigned long sprite_flags=0;
    if(graphic.Type==2)
        sprite_flags=1; // ENGINE_SPRITE_3D_FACE_ME, original EEngineSpriteFlag.
    if(GetParentThing().HasTC(TCI_EXPERIENCE_ORB))
        sprite_flags|=16; // ENGINE_SPRITE_ALIGN_BOTTOM.
    long no_frames_in_graphic=GetParentThing().PeekGraphicBank()->GetNoFramesInGraphic(graphic.BankIndex);
    float interpolated_anim_stage=GetInterpolatedAnimStage(draw_info.Interpolate);
    unsigned long frame=(unsigned long)(no_frames_in_graphic*interpolated_anim_stage);
    const CGraphicInfoFrame* graphic_frame=GetParentThing().PeekGraphicBank()->PeekGraphic(graphic.BankIndex)->PeekInfoFrame(frame);
    float size_x=tc_appearance.GetFullScale();
    float size_y=(float)graphic_frame->GetHeight()/graphic_frame->GetWidth()*size_x;
    float sprite_angle=(RotationAngle-OldRotationAngle)*draw_info.Interpolate+RotationAngle;
    EEnginePrimitiveBlendMode blend_mode;
    if(graphic.AdditiveAlpha)
        blend_mode=ENGINE_PRIMITIVE_BLEND_ADDITIVE;
    else if(graphic_frame->IsBooleanAlpha())
        blend_mode=ENGINE_PRIMITIVE_BLEND_ALPHA;
    else if(graphic_frame->IsAlpha())
        blend_mode=ENGINE_PRIMITIVE_BLEND_ALPHA;
    else
        blend_mode=ENGINE_PRIMITIVE_BLEND_ADDITIVE;
    long alpha=tc_appearance.GetAlpha();
    if(FadeOutWithAnim) {
        float source_alpha=tc_appearance.GetAlpha();
        alpha=(long)(source_alpha*(1.0f-interpolated_anim_stage));
    }
    if(!PrimHandle)
        PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_MISC));
    if(!tc_appearance.IsSelfIlluminating())
        sprite_flags|=128; // ENGINE_SPRITE_LIT.
    C3DVector pos=draw_info.Pos->GetInterpolated(draw_info.Interpolate);
    tc_appearance.SetPrimitiveGraphics(PrimHandle,CEnginePrimitiveSprite(pos,graphic.BankIndex,frame,(unsigned short)sprite_flags,sprite_angle,size_x,size_y,blend_mode,(unsigned char)alpha,255,255,255,0.0f,ENGINE_INTERPOLATION_SET_0,(EEnginePrimitiveBlendOp)0),0);
    return true;
}
}
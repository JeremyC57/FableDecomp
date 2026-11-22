// NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::DrawAt -- draw the framed minigame progress bar and optional icons.
// Grade: DIFFER(1677v1681) -- frame 0x214 versus 0x210; color and geometry local homes and operation order differ
// Leaf: ?DrawAt@CQuestInfoElementMiniGameBar@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 00651273,1681 bytes,thiscall.
// Flags: /Os
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUIDEF_MINIGAME_OFFSET
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "fable_compiler.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
    const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
    PrimitiveHandles.resize(6);
    const CGraphicDataBank& graphic_bank=*CPlayerGui::Get().GetQuestInfo().Bank;
    float left_graphic_width=(float)graphic_bank.GetGraphicFrameWidth(gui_def.HeroSprintBarLeftGraphic,0);
    float left_graphic_height=(float)graphic_bank.GetGraphicFrameHeight(gui_def.HeroSprintBarLeftGraphic,0);
    C2DVector draw_pos(pos);
    float height=Scale*16.0f;
    if(Graphic>0) {
        float sprite_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
        float sprite_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
        if(sprite_height>height)sprite_height=height;
        const float& iconX=draw_pos.X-(long)(sprite_width*0.5f);
        const float& iconY=draw_pos.Y-(long)(sprite_height*(1.0f/3.0f));

        CEnginePrimitive2DSprite icon(0,C2DVector(iconX,iconY),ENGINE_2D_LAYER_1,Graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[4],icon,0xc0);
    }
    const float inner_width=48.0f;
    C2DVector left_pos(draw_pos.X,draw_pos.Y-Scale*12.0f);
    const C2DVector& left_end=left_pos+C2DVector(left_graphic_width,left_graphic_height*Scale);
    CEnginePrimitive2DSprite left_bar_sprite(0,C2DBoxF(left_pos,left_end),ENGINE_2D_LAYER_3,gui_def.HeroSprintBarLeftGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[0],left_bar_sprite,0xc0);
    C2DVector middle_pos(left_pos.X+left_graphic_width,left_pos.Y);
    const C2DVector& middle_end=middle_pos+C2DVector(inner_width,left_graphic_height*Scale);
    CEnginePrimitive2DSprite middle_bar_sprite(0,C2DBoxF(middle_pos,middle_end),ENGINE_2D_LAYER_3,gui_def.HeroSprintBarMiddleGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[1],middle_bar_sprite,0xc0);
    C2DBoxF right_box(C2DVector(middle_pos.X+inner_width,middle_pos.Y),C2DVector(middle_pos.X+inner_width,middle_pos.Y)+C2DVector(left_graphic_width,left_graphic_height*Scale));
    CEnginePrimitive2DSprite right_bar_sprite(0,right_box,ENGINE_2D_LAYER_3,gui_def.HeroSprintBarRightGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[2],right_bar_sprite,0xc0);
    float value_as_percentage=Percentage;
    C2DVector inner_pos=left_pos+gui_def.HeroSprintBarInnerGraphicOffset;
    float inner_bar_width=((left_graphic_width-gui_def.HeroSprintBarInnerGraphicOffset.X)*2.0f+inner_width)*value_as_percentage;
    const C2DVector& inner_end=inner_pos+C2DVector(inner_bar_width,left_graphic_height*Scale);
    CRGBColour interp_colour(Colour);
    if(Type==MINIGAME_PICKPOCKET||Type==MINIGAME_PICKLOCK||Type==MINIGAME_STEAL) {
        interp_colour.R=(unsigned char)(Colour.R*value_as_percentage+FadeColour.R*(1.0f-value_as_percentage));
        interp_colour.G=(unsigned char)(Colour.G*value_as_percentage+FadeColour.G*(1.0f-value_as_percentage));
        interp_colour.B=(unsigned char)(FadeColour.B*(1.0f-value_as_percentage)+Colour.B*value_as_percentage);
        interp_colour.A=(unsigned char)(Colour.A*value_as_percentage+FadeColour.A*(1.0f-value_as_percentage));
    }
    CEnginePrimitive2DSprite inner_bar_sprite(0,C2DBoxF(inner_pos,inner_end),ENGINE_2D_LAYER_2,gui_def.HeroSprintBarInnerGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().ScaleAlpha(interp_colour.A),interp_colour.R,interp_colour.G,interp_colour.B,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[3],inner_bar_sprite,0xc0);
    if(Type==MINIGAME_FISHING&&value_as_percentage>DELTA_NOTIONAL_ZERO&&gui_def.MiniGameFishingRightGraphic>0) {
        long graphic=gui_def.MiniGameFishingRightGraphic;
        graphic_bank.GetGraphicFrameWidth(graphic,0);
        float iconHeight=(float)graphic_bank.GetGraphicFrameHeight(graphic,0);
        if(iconHeight>height)iconHeight=height;
        const float& arrowY=draw_pos.Y-(long)(iconHeight*(1.0f/3.0f));
        const float& arrowX=(float)(long)(inner_bar_width+inner_pos.X);
        C2DVector sprite_pos(arrowX,arrowY);
        CEnginePrimitive2DSprite icon(0,sprite_pos,ENGINE_2D_LAYER_1,graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[5],icon,0xc0);
    }
    return height;
}

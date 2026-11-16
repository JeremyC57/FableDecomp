// 0x00650E98 NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick::DrawAt -- draw a quest caption or graphic and its optional completion tick.
// Leaf: ?DrawAt@CQuestInfoElementTick@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0x00650E98, 987 bytes, thiscall.
// Flags: none
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_CDRAWQUESTINFO_CTOR_PASS23
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CFONTBANK_HEIGHT
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CSUBTITLERENDERER_QUESTTICK_PASS23
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CFontBank.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CPlayerGuiDef.h"

void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&,C2DVector&);
namespace NPlayerGui {
inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box){return box;}
float CDrawQuestInfo::CQuestInfoElementTick::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
 Clear();
 float max_width;
 float max_height;
 long tick_primitive_index;
 if(Graphic>0){
  PrimitiveHandles.resize(2);
  const CGraphicDataBank& graphic_bank=CPlayerGui::Get().GetQuestInfo().Bank;
  tick_primitive_index=1;
  float graphic_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
  float graphic_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
  max_width=graphic_width;
  max_height=graphic_height;
  C2DBoxF sprite_box(pos,C2DExtentsF(graphic_width*Scale,graphic_height*Scale));
  CEnginePrimitive2DSprite graphic_sprite(0,GFToScreen(sprite_box),ENGINE_2D_LAYER_6,Graphic,0,
   ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().GetAlpha(),255,255,255,2,
   C2DVector(0.0f,0.0f),C2DVector(0.0f,0.0f));
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[0],graphic_sprite,0xc0);
  CWideString text_string=CPlayerGui::Get().GetSubtitleRenderer().GetTextFromEmbeddedTagGraphic(Graphic);
  if(text_string!=L""){
   const C2DVector& text_offset=CPlayerGui::Get().GetSubtitleRenderer().GetTextOffsetFromEmbeddedTagGraphic(Graphic);
   C2DVector vtext_pos;
   C2DVector text_pos=pos+text_offset-C2DVector(4.0f,4.0f);
   GFACoordToVCoord(text_pos,vtext_pos);
   const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
   CPlayerGui::Get().Draw2DText(vtext_pos,text_string,font_bank,CRGBColour(255,255,255),0.75f,ENGINE_2D_LAYER_0,true);
  }
 }else if(Text!=L""){
  PrimitiveHandles.resize(1);
  tick_primitive_index=0;
  C2DVector vtext_pos;
  GFACoordToVCoord(pos,vtext_pos);
  const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
  CPlayerGui::Get().Draw2DText(vtext_pos,Text,font_bank,CRGBColour(255,255,255),0.75f,ENGINE_2D_LAYER_0,true);
  max_height=font_bank.GetFontMaxHeight();
  max_width=font_bank.GetStringWidth(Text);
 }else return 0.0f;
 const CPlayerGuiDef& player_gui_def=CPlayerGui::PeekGuiDef();
 long tick_graphic=player_gui_def.QuestInfoTickGraphic;
 if(tick_graphic>0){
  C2DVector tick_pos(pos.X,pos.Y);
  tick_pos.X=tick_pos.X+max_width*Scale+3.0f;
  const CGraphicDataBank& graphic_bank=CPlayerGui::Get().GetQuestInfo().Bank;
  float graphic_width=(float)graphic_bank.GetGraphicFrameWidth(tick_graphic,0);
  float graphic_height=(float)graphic_bank.GetGraphicFrameHeight(tick_graphic,0);
  if(graphic_height>max_height)max_height=graphic_height;
  if(Ticked){
   C2DBoxF tick_box(tick_pos,C2DExtentsF(graphic_width*Scale,graphic_height*Scale));
   CEnginePrimitive2DSprite tick_sprite(0,GFToScreen(tick_box),ENGINE_2D_LAYER_6,tick_graphic,0,
    ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().GetAlpha(),255,255,255,2,
    C2DVector(0.0f,0.0f),C2DVector(0.0f,0.0f));
   info.Engine.SetPersistentPrimitive(PrimitiveHandles[tick_primitive_index],tick_sprite,0xc0);
  }
 }
 return max_height*Scale+2.0f;
}
}
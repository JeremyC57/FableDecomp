// NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter::DrawAt
// Fidelity: Branch-local double results preserve the return-conversion boundary before text cleanup.
// Retail:0x006509f8,549 bytes,thiscall. Size-optimised player GUI translation unit.
// Leaf: ?DrawAt@CQuestInfoElementCounter@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CFONTBANK_HEIGHT
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_EXTENTS_DECL
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CFontBank.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&,C2DVector&);
CWideString FABLE_FASTCALL GFIntToWideString(long);
CWideString FABLE_FASTCALL operator+(const unsigned short*,const CWideString&);
namespace NPlayerGui {
inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box){return box;}
float CDrawQuestInfo::CQuestInfoElementCounter::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
 Clear();
 float graphic_width=0.0f;
 float graphic_height=0.0f;
 if(Graphic>0){
  PrimitiveHandles.resize(1);
  const CGraphicDataBank& graphic_bank=*CPlayerGui::Get().GetQuestInfo().Bank;
  graphic_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
  graphic_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
  C2DBoxF sprite_box(pos,C2DExtentsF(graphic_width*Scale,graphic_height*Scale));
  CEnginePrimitive2DSprite graphic_sprite(0,GFToScreen(sprite_box),ENGINE_2D_LAYER_6,Graphic,0,
    ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(255),255,255,255,2,
    C2DVector(0.0f,0.0f),C2DVector(0.0f,0.0f));
  info.Engine->SetPersistentPrimitive(PrimitiveHandles[0],graphic_sprite,0xc0);
 }
 const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
 C2DVector text_pos(pos.X+graphic_width*Scale,pos.Y);
 text_pos.Y+=(graphic_height-(float)font_bank.GetFontHeight())*0.5f;
 C2DVector v_text_pos;
 GFACoordToVCoord(text_pos,v_text_pos);
 CWideString text=GFIntToWideString(Value);
 if(MaxValue>0)text+=L" / "+GFIntToWideString(MaxValue);
 CPlayerGui::Get().Draw2DText(v_text_pos,text,font_bank,CRGBColour(255,255,255),Scale,ENGINE_2D_LAYER_6,true);
 if(graphic_height>0.0f){double result=graphic_height*Scale+2.0f;return (float)result;}
 double result=font_bank.GetFontHeight()*Scale;
 return (float)result;
}
}
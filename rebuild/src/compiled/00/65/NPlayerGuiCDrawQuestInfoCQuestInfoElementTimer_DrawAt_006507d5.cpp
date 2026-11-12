// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt
// Fidelity: Branch-local double results preserve the return-conversion boundary before text cleanup: retail stores the
// result to the dead `pos` slot [ebp+8] at 0x006509E3, keeps it across ~CWideString 0x0099B510 (call at 0x006509E9) and
// reloads it at 0x006509EE -- same construct and comment as the landed sibling 0x006509F8 (CQuestInfoElementCounter::DrawAt).
// Draws the quest-info timer element: when it has a graphic, sizes one persistent sprite from the quest-info graphic bank's frame
// width/height times the element Scale and submits it through the prepare info's engine; then prints the timer's remaining
// seconds (CGameScriptInterface::GetTimer(TimerIndex)) as "mm:ss" beside it in the main font, vertically centred on the graphic,
// red-ish (255,100,100) at ten seconds or less, white otherwise. Returns the drawn height (graphic height*Scale+2, else font height*Scale).
// Retail 0x006507D5, 547 bytes, thiscall `ret 8`, size-optimised TU (#pragma optimize("s"): ebp frame, `leave`, unaligned entry).
// Callees: vector<CEnginePrimitiveHandle>::resize 0x00443F90 (out of line), CGraphicDataBank vslots 21/22
// GetGraphicFrameWidth/Height, C2DBoxF(const C2DVector&,const C2DExtentsF&) 0x0041A7E0, __ftol2 0x00BFEA70 (ScaleAlpha),
// CEnginePrimitive2DSprite ctor 0x0041BEB0, CIEngine::SetPersistentPrimitive vslot 23, GFACoordToVCoord 0x009E21D0,
// CGameScriptInterface::GetTimer vslot 90 (0x006E7710), CWideString() 0x0099AED0, CWideString::Sprintf 0x0099BA70 (L"%02d:%02d"
// 0x0125A694), CPlayerGui::Draw2DText 0x00438010, ~CWideString 0x0099B510.
// Shape: the colour is chosen by an if/else around two Draw2DText calls; VC7.1 hoists the common argument pushes and the A,R
// stores above the branch and merges the calls, leaving one colour temp in the dead `pos` slot [ebp+8] (with the spilled seconds).
// Leaf: ?DrawAt@CQuestInfoElementTimer@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
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
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_NSCRIPT
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
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CFontBank.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&,C2DVector&);
namespace NPlayerGui {
// NPlayerGui::GFToScreen (Ego_r.pdb ?GFToScreen@NPlayerGui@@YI?AVC2DBoxF@@ABV2@@Z, Ego_r 0x00686AD4; FableWin 0x01835ABD): its body
// was visible inline in retail's TU -- retail copies the box inline from [ebp-0x3c] to [ebp-0x4c] (0x00650875..0x00650897) before
// the sprite ctor call at 0x006508E7, with no call. Body copied from the landed row 0x006509F8 (CQuestInfoElementCounter::DrawAt).
inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box){return box;}
float CDrawQuestInfo::CQuestInfoElementTimer::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
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
 long seconds=CMainGameComponent::Get()->PeekWorld().GetGameScriptInterface().GetTimer(TimerIndex);
 CWideString text;
 text.Sprintf(L"%02d:%02d",seconds/60,seconds%60);
 if(seconds<11)CPlayerGui::Get().Draw2DText(v_text_pos,text,font_bank,CRGBColour(255,100,100),Scale,ENGINE_2D_LAYER_6,true);
 else CPlayerGui::Get().Draw2DText(v_text_pos,text,font_bank,CRGBColour(255,255,255),Scale,ENGINE_2D_LAYER_6,true);
 if(graphic_height>0.0f){double result=graphic_height*Scale+2.0f;return (float)result;}
 double result=font_bank.GetFontHeight()*Scale;
 return (float)result;
}
}
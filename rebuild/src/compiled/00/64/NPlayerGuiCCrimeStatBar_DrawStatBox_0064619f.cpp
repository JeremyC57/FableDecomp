// NPlayerGui::CCrimeStatBar::DrawStatBox -- draw the crime clock and icon above its stat box.
// Retail: 0064619f,509 bytes,thiscall.
// Flags: /O1 /Oy-
// Leaf: ?DrawStatBox@CCrimeStatBar@NPlayerGui@@MAEXABVCEnginePrimitivePrepareInfo@@ABVCGraphicDataBank@@ABVC2DVector@@JJJABVCWideString@@E_N4@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPLAYERGUIDEF_CRIME
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_SCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#include "engine/C2DVector.h"
#include "engine/Direct3D9Types.h"
#include "engine/NPlayerGui_CCrimeStatBar.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/CDisplayManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitive2DClockSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
extern CDisplayManager* GPDisplayManager;
void NPlayerGui::CCrimeStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo& info,const CGraphicDataBank& graphic_bank,
 const C2DVector& tl_pos,long icon_graphic,long border_graphic,long effect_index,const CWideString& stat_text,
 unsigned char alpha,bool mirror,bool centre)
{
 C2DVector draw_pos=tl_pos+GuiDef.CrimeBarIconOffset;
 if(border_graphic>0) {
  C2DVector offset_pos((GPDisplayManager->GetRenderTargetDimensions().X-graphic_bank.GetGraphicMaxWidth(border_graphic))/2.0f,
   draw_pos.Y+GuiDef.CrimeBarBorderOffset.Y);
  float angle=NormalisedTimeLeft;
  CEnginePrimitive2DClockSprite border_sprite(0,offset_pos,ENGINE_2D_LAYER_2,border_graphic,0,CRGBColour(0,0,0,alpha),
   border_graphic,0,CRGBColour(255,255,255,alpha),ENGINE_PRIMITIVE_BLEND_ALPHA,angle,angle,C2DVector(0,0),C2DVector(0,0),2,ENGINE_INTERPOLATION_SET_0);
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[4],border_sprite,0xc0);
 }
 if(icon_graphic>0) {
  C2DVector offset_pos((GPDisplayManager->GetRenderTargetDimensions().X-graphic_bank.GetGraphicMaxWidth(icon_graphic))/2.0f,draw_pos.Y);
  CEnginePrimitive2DSprite icon_sprite(0,GFToScreen(offset_pos),ENGINE_2D_LAYER_1,icon_graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,
   ENGINE_INTERPOLATION_SET_0,0,0,CPlayerGui::Get().ScaleAlpha(alpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[3],icon_sprite,0xc0);
 }
 CStatBar::DrawStatBox(info,graphic_bank,tl_pos,0,0,0,stat_text,alpha,mirror,centre);
}
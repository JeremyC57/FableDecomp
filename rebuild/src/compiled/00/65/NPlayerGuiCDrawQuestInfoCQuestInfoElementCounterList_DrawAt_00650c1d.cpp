#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_OWNER_SPEED_PASS26
#define FABLE_RETAIL_C2DBOXF_EXTENTS_OWNER_SPEED_PASS26
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
// Leaf: ?DrawAt@CQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy-
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
class CDataInputStream;class CDataOutputStream;
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&,C2DVector&);
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#include "engine/NPlayerGui_Graphics.h"
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
 Clear();
 if(Graphic>0) {
  const CPlayerGuiDef& gui_def=CPlayerGui::Get().PeekGuiDef();
  long num_big=Value/5;
  long num_small=Value%5;
  long total_sprites=num_big+num_small;
  PrimitiveHandles.resize(total_sprites);
  const CGraphicDataBank& graphic_bank=CPlayerGui::Get().GetQuestInfo().Bank;
  float graphic_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
  float graphic_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
  C2DVector draw_pos=pos;
  long handle_index=0;
  for(long i=0;i<total_sprites;++i) {
   C2DVector sprite_pos=draw_pos;
   float sprite_width,sprite_height;
   if(i>=num_big) {
    sprite_width=graphic_width*0.75f;
    sprite_height=graphic_height*0.75f;
    draw_pos.X+=sprite_width*Scale;
   } else {
    C2DVector text_pos;
    GFACoordToVCoord(C2DVector(Scale*8.0f,Scale*4.0f)+draw_pos,text_pos);
    const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
    CPlayerGui::Get().Draw2DText(text_pos,L"5",font_bank,CRGBColour(255,255,255),Scale,ENGINE_2D_LAYER_5,false);
    draw_pos.X+=graphic_width*Scale;
    sprite_width=graphic_width*1.2f;
    sprite_height=graphic_height*1.2f;
    sprite_pos.X-=sprite_width*0.1f;
    sprite_pos.Y-=sprite_height*0.1f;
   }
   C2DBoxF sprite_box(sprite_pos,C2DExtentsF(sprite_width*Scale,sprite_height*Scale));
   CEnginePrimitive2DSprite graphic_sprite(0,GFToScreen(sprite_box),ENGINE_2D_LAYER_6,Graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(255),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
   info.Engine.SetPersistentPrimitive(PrimitiveHandles[handle_index],graphic_sprite,0xc0);
   ++handle_index;
  }
  return graphic_height*Scale+2.0f;
 }
 return 0.0f;
}
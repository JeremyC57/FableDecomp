// 0064ae80 NPlayerGui::CDrawAbilityButtons::DrawAbilityButton
// Leaf: ?DrawAbilityButton@CDrawAbilityButtons@NPlayerGui@@IAEXABVCEnginePrimitivePrepareInfo@@AAV?$vector@V?$CDefPointer@$$CBVCHeroAbilityDef@@@@V?$allocator@V?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@@std@@JJJM@Z
// Flags: none
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_ABILITY_CONTEXT_PASS23
#define FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22
#define FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#define FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NPlayerGui_CDrawAbilityButtons.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"

#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;
// Genuine same-TU FableWin020d9289 body; native uses float640/480 literals.
inline C2DVector FABLE_FASTCALL NPlayerGui::GFAnchorToBottomRight(const C2DVector& pos)
{return C2DVector((float)GPDisplayManager->GetRenderTargetDimensions().X+pos.X-640.0f,(float)GPDisplayManager->GetRenderTargetDimensions().Y+pos.Y-480.0f);}
// Original FableWin020d93ce forwarding body, ordinary out-of-line context.
unsigned char NPlayerGui::CDrawBase::ScaleAlpha(unsigned char alpha)const{return PlayerGui.ScaleAlpha(alpha);}
// Genuine native00645099 / original both-PDB seven-argument DrawButton body.
// Ordinary visibility fixes caller0064af89 schedule; all324 helper bytes independently match.
// Existing GetAlpha/IsShowSpecialAbilityButtons and ScaleAlpha retain their original owners.
void NPlayerGui::CDrawAbilityButtons::DrawButton(const CEnginePrimitivePrepareInfo& info,const C2DVector& position,float alpha,long primitive_index,long graphic_index,bool special,EEngine2DLayer layer)
{
 const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
 std::vector<CEnginePrimitiveHandle>& handles=special?SpecialPrimitiveHandles:PrimitiveHandles;
 bool available=true;
 if(alpha<0.0f){available=false;alpha=-alpha;}
 unsigned char actual_alpha=PlayerGui.GetAlpha();
 if(PlayerGui.IsShowSpecialAbilityButtons()&&actual_alpha<255)actual_alpha=255;
 if(graphic_index>0&&alpha>0.0f){
  float icon_alpha=alpha*actual_alpha;
  if(!available)icon_alpha=160.0f;
  else if(PlayerGui.IsShowSpecialAbilityButtons()&&actual_alpha<255)icon_alpha=special?255.0f:0.0f;
  CEnginePrimitive2DSprite abxy_icon_sprite(0,GFToScreen(position),layer,graphic_index,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha((unsigned char)icon_alpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(handles[primitive_index],abxy_icon_sprite,0xc0);
 }
}
void NPlayerGui::CDrawAbilityButtons::DrawAbilityButton(const CEnginePrimitivePrepareInfo& info,std::vector<CDefPointer<const CHeroAbilityDef> >& ability_defs,long button_index,long ability_index,long level_offset,float alpha)
{
 const CThingPlayerCreature* phero=GetPlayer().PeekPControlledCreature();
 if(IS_THING_ALIVE(phero)){
  std::vector<long> position_mapping;
  GetAbilityPositionMapping(position_mapping);
  const CTCInventoryAbilities* ptc_inventory_abilities=0;
  phero->PeekTC(&ptc_inventory_abilities);
  if(ptc_inventory_abilities){
   long num_abilities_to_draw=((long)ability_defs.size()+2)/Columns*Columns;
   if(ability_index<num_abilities_to_draw&&(unsigned long)ability_index<ability_defs.size()){
    const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
    long handle_index=gui_def.AbilityButtons.size()+ability_index+7;
    C2DVector button_pos=GFAnchorToBottomRight(gui_def.ABXYRingsPos);
    // Fidelity: cache the genuine STLport iterator after anchoring; no substitute container model.
    std::vector<CAbilityButtonGuiDef>::const_iterator button_types=gui_def.SpecialAbilityButtons.begin();
    button_pos.X+=button_types[position_mapping[button_index]].Offset.X;
    // Fidelity: named running sum preserves original addition sequence.
    float button_y=button_pos.Y;
    button_y+=button_types[position_mapping[0]].Offset.Y;
    button_y+=level_offset*gui_def.ABXYAbilityLevelOffset.Y;
    button_y+=gui_def.ABXYAbilityLevelInitialOffset.Y;
    button_pos.Y=button_y;
    CDefPointer<const CHeroAbilityDef> pdef;
    if((unsigned long)ability_index<ability_defs.size())pdef=ability_defs[ability_index];
    float actual_alpha=alpha;
    long graphic=gui_def.ABXYAbilityUnavailableGraphic;
    if(pdef.Get()){
     graphic=pdef->IconGraphicIndex;
     bool can_use_ability=true;
     if(!ptc_inventory_abilities->IsAbleToUseAbility(pdef->Ability,true)){
      if(pdef->Ability!=HERO_ABILITY_PHYSICAL_SHIELD_SPELL||!phero->HasTC(TCI_PHYSICAL_SHIELD_SPELL))can_use_ability=false;
     }else{
      const CTCSpecialAbilities* ptc_special_abilites;
      if(phero->PeekTC(&ptc_special_abilites)&&!ptc_special_abilites->IsAbilityAvailable(pdef->Ability))can_use_ability=false;
     }
     if(!can_use_ability){
      long unusable_graphic=gui_def.ABXYAbilityUncastableGraphic;
      long unusable_handle=handle_index+20;
      if((unsigned long)unusable_handle<PrimitiveHandles.size()){
       EEngine2DLayer layer=ENGINE_2D_LAYER_0;
       if(position_mapping[button_index]==1)layer=ENGINE_2D_LAYER_1;
       CEnginePrimitive2DSprite unusable_sprite(0,GFToScreen(button_pos),layer,unusable_graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha(128),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
       info.Engine.SetPersistentPrimitive(PrimitiveHandles[unusable_handle],unusable_sprite,0x8c0);
      }
     }
     if(pdef->Ability==HERO_ABILITY_PHYSICAL_SHIELD_SPELL&&phero->HasTC(TCI_PHYSICAL_SHIELD_SPELL))graphic=gui_def.ABXYAbilityShieldOffGraphic;
    }else actual_alpha/=3.0f;
    EEngine2DLayer layer=ENGINE_2D_LAYER_1;
    if(position_mapping[button_index]==1)layer=ENGINE_2D_LAYER_2;
    DrawButton(info,button_pos,actual_alpha,handle_index,graphic,false,layer);
   }
  }
 }
}
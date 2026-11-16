// 0064a639 NPlayerGui::CDrawGuildSeal::Draw
// Leaf: ?Draw@CDrawGuildSeal@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail:0x0064A639,911bytes,thiscall.
// Flags: none
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_STLP_QUEUE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ADD_COPY_RHS_X_PASS23
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWGUILDSEAL_PASS23
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISCHILD
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD_CTOR_PASS23
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_DEFINITION_REFERENCE_PASS23
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NPlayerGui_CDrawGuildSeal.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventory.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"

#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;
// Original same-TU inline helper, FableWin020d922d; native00643cda omits frame pointer.
#ifdef _MSC_VER
#pragma optimize("y",on)
#endif
inline C2DVector FABLE_FASTCALL NPlayerGui::GFAnchorToBottomLeft(const C2DVector& pos)
{
 return C2DVector(pos.X,(float)GPDisplayManager->GetRenderTargetDimensions().Y+pos.Y-480.0f);
}

#ifdef _MSC_VER
#pragma optimize("y",off)
#endif
void NPlayerGui::CDrawGuildSeal::Draw(const CEnginePrimitivePrepareInfo& info)
{
 if(ReadyToLevelUp&&!World.IsPaused()){
  const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
  ReminderTime+=info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame;
  if(ReminderTime>gui_def.GuildSealReminderTimeSeconds){
   LevelUpGraphic=gui_def.DPadGuildSealLevelUpGraphic;
   SetEnabled(true);
  }
 }
 if(FlashCycles>0){
  if(Enabled){
   const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
   if(!World.IsPaused()){
    FadeInOut.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    if(FadeInOut.IsOpen())FadeInOut.Close();
    else if(!FadeInOut.IsActive()){
     if(--FlashCycles>0)FadeInOut.Open(gui_def.GuildSealFlashCycleSeconds*0.5f,gui_def.GuildSealFlashCycleSeconds*0.5f,CRGBColour(0,0,0,255));
    }
   }
   FlashAlpha=(unsigned char)(FadeInOut.GetFadeInOutAlpha()*255.0f);
   if(ReadyToLevelUp&&FlashAlpha>0){
    const CThingPlayerCreature* phero=GetPlayer().PeekPControlledCreature();
    const CTCInventory* ptc_inventory=0;
    if(IS_THING_ALIVE(phero)&&!phero->IsChild()&&phero->PeekTC(&ptc_inventory)){
     long def_index=World.PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
     if(def_index>0&&ptc_inventory->GetNumberOfItemsOfTypeInInventory(def_index)>0){
      Drawing=true;
      long graphic=gui_def.GuildSealLevelUpGraphic;
      std::map<CCharString,long>::const_iterator it=gui_def.ScriptSprites.find("HUD_ORB_QUEST_CORE");
      if(it!=gui_def.ScriptSprites.end()&&it->second==LevelUpGraphic)graphic=gui_def.GuildSealQuestGraphic;
      CEnginePrimitive2DSprite level_up_guild_seal_sprite(0,GFToScreen(GFAnchorToBottomLeft(gui_def.DPadRingsPos+gui_def.DPadRingsTopOffset)),ENGINE_2D_LAYER_1,graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha(FlashAlpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
      info.Engine.SetPersistentPrimitive(PrimitiveHandles[0],level_up_guild_seal_sprite,0xc0);
     }
    }
    if(!LevelUpPositions.empty()){
     Drawing=true;
     if(LevelUpGraphic==0)LevelUpGraphic=gui_def.DPadGuildSealLevelUpGraphic;
     for(long i=0;(unsigned long)i<LevelUpPositions.size();++i){
      CEnginePrimitive2DSprite level_up_sprite(0,GFToScreen(LevelUpPositions[i]),ENGINE_2D_LAYER_1,LevelUpGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha(FlashAlpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
      info.Engine.SetPersistentPrimitive(PrimitiveHandles[i+1],level_up_sprite,0xc0);
     }
    }
   }
  }
 }
 if(FlashCycles==0)SetEnabled(false);
}
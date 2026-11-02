// CSubtitleRenderer::GetActionNameFromEmbeddedTag(CWideString const &,CWideString &) -- resolve action tags and movement directions to assigned control names.
// Leaf: ?GetActionNameFromEmbeddedTag@CSubtitleRenderer@@CI_NABVCWideString@@AAV2@@Z
// Grade: DIFFER(1045v1056) -- frame-slot and register allocation around the CCharString temporaries: retail homes the literal at [ebp-8] and the ToCharString result at [ebp-10h] and reads the result through eax; VC7.1 RTM assigns [ebp-0Ch]/[ebp-8], 11 bytes short overall; 10 shapes (declaration order text/str, inline temporary, by-value text, const literal, no block scope, length local, matches init, literal compare, /Oy-) leave it. Graded in Codex's header context, which uses Codex's second CCharString model engine/CCharString.h and FABLE_RETAIL_SUBTITLE (header debt: must be rebased on fable_string.h before any landing)
// Flags: /Oy-
#define FABLE_RETAIL_SUBTITLE
#include <string.h>
#include "engine/CCharString.h"
#include "engine/CWideString.h"
#include "engine/CActionInputControl.h"
#include "stlport/subtitle_map.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CUserProfileManager.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CSubtitleRenderer.h"
#pragma intrinsic(strcmp)
#pragma optimize("s",on)
bool __fastcall CSubtitleRenderer::GetActionNameFromEmbeddedTag(const CWideString& tag,CWideString& action_name)
{
 const CPlayerGuiDef& player_gui_def=CPlayerGui::PeekGuiDef();
 std::map<CCharString,EGameAction>::const_iterator it=player_gui_def.GameActionValues.find(tag.ToCharString());
 if(it!=player_gui_def.GameActionValues.end()) {
  EGameAction game_action=it->second;
  CActionInputControl input_control;
  if(CUserProfileManager::Get().GetAssignedInputForAction(game_action,true,input_control) ||
     CUserProfileManager::Get().GetAssignedInputForAction(game_action,false,input_control)) {
   if(CGamePlayerInterface::GetActionName(input_control,action_name,false))return true;
  }
 } else {
  bool matches;
  {
   const CCharString& text=tag.ToCharString();
   CCharString str("GAME_ACTION_MOVEMENT_");
   matches=strncmp(text.GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0;
  }
  if(matches) {
  C2DVector direction(0.0f,0.0f);
  {
   CCharString str("UP");
   const CCharString& text=tag.ToCharString();
   long offset=str.GetLength();
   offset=text.GetLength()-offset;
   if(offset>=0)matches=strcmp(text.GetAsCharArray()+offset,str.GetAsCharArray())==0;else matches=false;
  }
  if(matches)direction=C2DVector(0.0f,1.0f);
  else {
   {
    CCharString str("RIGHT");
    const CCharString& text=tag.ToCharString();
    long offset=str.GetLength();
   offset=text.GetLength()-offset;
    if(offset>=0)matches=strcmp(text.GetAsCharArray()+offset,str.GetAsCharArray())==0;else matches=false;
   }
   if(matches)direction=C2DVector(1.0f,0.0f);
   else {
    {
     CCharString str("DOWN");
     const CCharString& text=tag.ToCharString();
     long offset=str.GetLength();
   offset=text.GetLength()-offset;
     if(offset>=0)matches=strcmp(text.GetAsCharArray()+offset,str.GetAsCharArray())==0;else matches=false;
    }
    if(matches)direction=C2DVector(0.0f,-1.0f);
    else {
     {
      CCharString str("LEFT");
      const CCharString& text=tag.ToCharString();
      long offset=str.GetLength();
   offset=text.GetLength()-offset;
      if(offset>=0)matches=strcmp(text.GetAsCharArray()+offset,str.GetAsCharArray())==0;else matches=false;
     }
     if(matches)direction=C2DVector(-1.0f,0.0f);
    }
   }
  }
  if(direction.GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO) {
   CActionInputControl input_control;
   if(CUserProfileManager::Get().GetAssignedInputForMovementAction(GAME_ACTION_MOVEMENT,true,direction,input_control) ||
      CUserProfileManager::Get().GetAssignedInputForMovementAction(GAME_ACTION_MOVEMENT,false,direction,input_control)) {
    if(CGamePlayerInterface::GetActionName(input_control,action_name,false))return true;
   }
  }
  }
 }
 return false;
}

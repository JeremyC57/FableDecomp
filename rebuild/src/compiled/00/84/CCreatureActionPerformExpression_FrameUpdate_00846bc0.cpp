// CCreatureAction_PerformExpression::FrameUpdate -- track held expression input and play the newly performed sound.
// Retail: 00846bc0,527 bytes.
// Leaf: ?FrameUpdate@CCreatureAction_PerformExpression@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_PerformExpression.h"
#include "engine/CTCInventory.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CExpressionDef.h"
#include "engine/CTCSoundPlayer.h"
void CCreatureAction_PerformExpression::FrameUpdate()
{
 ButtonHeld=false;
 if(ExpressionItemDefIndex>0) {
  const CTCInventory* ptc_inventory=0;
  if(PCreature->PeekTC(&ptc_inventory)) {
   long button_index=ptc_inventory->GetQuickAccessItemButtonIndex(ExpressionItemDefIndex);
   if(button_index!=-1) {
    if(PCreature->GetPlayer().GetPlayerInterface()->IsQuickAccessEventInQueue(button_index,(EInputButtonEventType)2,PCreature->GetPlayer().GetJoystickDeviceNumber()))ButtonHeld=true;
   }
   if(!ButtonHeld) {
    button_index=ptc_inventory->GetContextSensitiveItemButtonIndex(ExpressionItemDefIndex);
    if(button_index!=-1) {
     if(PCreature->GetPlayer().GetPlayerInterface()->IsContextSensitiveEventInQueue(button_index,(EInputButtonEventType)2,PCreature->GetPlayer().GetJoystickDeviceNumber()))ButtonHeld=true;
    }
   }
  }
 }
 if(!HasPerformedAction()) {
  CCreatureActionBase::FrameUpdate();
  if(HasPerformedAction()) {
   CTCSoundPlayer* ptc_sound=0;
   if(PCreature->GetTC(&ptc_sound)) {
    if(ButtonHeld && PExpressionDef->LoopSoundCriteria!="") {
     CCharString sound=PExpressionDef->LoopSoundCriteria.GetString();
     SoundEffectID=ptc_sound->PlayAttachedCriteria(sound,true,false,false,-1);
    }
    else ptc_sound->PlayAttachedCriteria(PExpressionDef->SoundCriteria,true,false,false,-1);
   }
  }
 } else CCreatureActionBase::FrameUpdate();
}
// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction() -- restore the haste contribution after unsheathing.
// Retail: 006a02b0, 240 bytes, thiscall
// Genuine STLport insertion bodies expose the original temporary lifetimes.
// Leaf: ?FinishAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHE_PRIVATE
#define FABLE_RETAIL_CTCHASTE_MAKE_PAIR
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCHASTE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCHaste.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h"
void CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction()
{
 CCreatureAction_UnsheatheItemFromInventory::FinishAction();
 const CTCBulletTime* tctime=0;
 if(PCreature->PeekTC(&tctime)){
  CTCHaste* ptc_haste=0;
  if(!PCreature->GetTC(&ptc_haste))
   ptc_haste=GFDebugPDynamicCast(PCreature->AddTC("CTCHaste",true,0),(CTCHaste*)0);
  ptc_haste->SetActionSpeedMultiplier(HasteKey,tctime->GetTimeMultiplier());
 }
}
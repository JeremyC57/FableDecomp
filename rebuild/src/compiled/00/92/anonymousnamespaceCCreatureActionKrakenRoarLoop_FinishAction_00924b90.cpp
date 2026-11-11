// CCreatureAction_KrakenRoarLoop::FinishAction -- advance the roar count and select the next action.
// Retail: 00924b90,219 bytes.
// Leaf: ?FinishAction@CCreatureAction_KrakenRoarLoop@?A0xe2035276@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_KRAKEN_ROAR
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#include "engine/CThingCreatureBase.h"
#include "engine/CTCKraken.h"
#include "engine/CCreatureAction_KrakenRoar.h"
void CCreatureAction_KrakenRoarLoop::FinishAction()
{
 CCreatureActionBase::FinishAction();
 CTCKraken* pTCKraken;
 PCreature->GetTC(&pTCKraken);
 if(++LoopNum<pTCKraken->GetKrakenDef().RoarLoops)
  PCreature->SetCurrentAction(CCreatureAction_KrakenRoarLoop(*PCreature,LoopNum));
 else
  PCreature->SetCurrentAction(CCreatureAction_KrakenRoarOutOf(*PCreature));
}
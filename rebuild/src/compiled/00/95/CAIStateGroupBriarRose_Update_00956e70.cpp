// CAIStateGroup_BriarRose::Update -- cast shield and enter the invulnerable combat action.
// Retail: 00956e70,363 bytes.
// Leaf: ?Update@CAIStateGroup_BriarRose@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#include "engine/EAIAction.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAIStateGroup_BriarRose.h"
#include "engine/CAIBrain.h"
#include "engine/CBriarRoseDef.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CCreatureAction_BriarRoseIntoInvulnerable.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
void CAIStateGroup_BriarRose::Update()
{
 if(!IsStillRunnable()){Terminate();return;}
 CDefPointer<const CBriarRoseDef>pdef;
 GetCreature().GetPBaseDef()->GetSubDef(&pdef);
 float seconds_between_teleport=pdef->SecondsBetweenTeleportAway;
 NextRunFrame=(long)(GetWorld().GetFrame()+CMainGameComponent::GetConstantFPS()*seconds_between_teleport);
 CTCPhysicalShieldSpell*ptc_shield=GFDebugPDynamicCast(GetCreature().AddTC("CTCPhysicalShieldSpell",true,0),(CTCPhysicalShieldSpell*)0);
 const long SHIELD_LEVEL=2;
 const float SECONDS_FOR_SHIELD_TO_BE_ACTIVE=30.0f;
 ptc_shield->SetAICasting(SHIELD_LEVEL);
 ptc_shield->SetTimeLimit((long)(CMainGameComponent::GetConstantFPS()*SECONDS_FOR_SHIELD_TO_BE_ACTIVE));
 EAIActionReturn ret=AIAction_PerformActionAndWait(CCreatureAction_BriarRoseIntoInvulnerable(GetCreature(),*PEnemy),false);
 if(!IsStillRunnable()){Terminate();return;}
 NextRunFrame=(long)(GetWorld().GetFrame()+CMainGameComponent::GetConstantFPS()*seconds_between_teleport);
 Terminate();
}
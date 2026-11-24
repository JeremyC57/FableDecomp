// CAIStateGroup_CreatureSeekEnemyUnderground::Update -- seek an enemy while hidden and select a valid rising position.
// Leaf: ?Update@CAIStateGroup_CreatureSeekEnemyUnderground@@MAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS
#define FABLE_RETAIL_IS_THING_ALIVE
// Inline-budget: native 0066E370 expands its repeated-value construction loop.
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_NOINLINE_PASS26
#define FABLE_RETAIL_STLP_VECTOR
#include <stdlib.h>
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_CreatureSeekEnemyUnderground.h"
#include "engine/CAIBrain.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/ThingAlive.h"
#include "engine/GFSqr.h"
#include "engine/NCreatureHiding.h"
void CAIStateGroup_CreatureSeekEnemyUnderground::Update()
{
 const float RADIUS_FOR_UNDEAD_CREATION=2.0f;
 const float MINIMUM_DISTANCE_FROM_HERO_FOR_UNDEAD_CREATION=1.5f;
 const float CREATION_POINT_OFFSET_MULTIPLIER=15.0f;
 while(GetCreature().IsPerformingAnyAction()) {
  EAIActionReturn ret=AIAction_DoNothing();
  if(ret!=AI_ACTION_IN_PROGRESS||!IsStillRunnable()){Terminate();return;}
 }
 if(!IS_THING_ALIVE(PEnemy)) {
  const CTCTargetingBase* ptc_targeting;
  GetCreature().PeekTC(&ptc_targeting);
  CThingCreatureBase* p_enemy=ptc_targeting->GetPBestEnemyCreatureForAttack();
  if(IS_THING_ALIVE(p_enemy))PEnemy=CIntelligentPointer<const CThing>(p_enemy);
 }
 if(!IsStillRunnable()){Terminate();return;}
 const CTCPerceiveThing* ptc_perception=0;
 GetCreature().PeekTC(&ptc_perception);
 if(IS_THING_ALIVE(PEnemy)) {
  if((PEnemy->GetPos()-GetCreature().GetPos()).GetMagnitude2D()<ptc_perception->GetSightRadius()*1.5f) {
   C3DVector creationOffset=(PEnemy->GetPhysicsTC()->GetPosition()-PEnemy->GetPhysicsTC()->GetLastPosition())*CREATION_POINT_OFFSET_MULTIPLIER;
   std::vector<C3DVector> positions;
   C3DVector creationCentrePoint=PEnemy->GetPos()+creationOffset;
   NCreatureHiding::GetValidPositions(positions,PeekWorldMap(),PeekThingSearchTools(),creationCentrePoint,RADIUS_FOR_UNDEAD_CREATION,false,0);
   std::vector<C3DVector> valid_positions;
   long numPositions=positions.size();
   valid_positions.reserve(numPositions);
   long i;
   for(i=0;i<numPositions;++i) {
    if((positions[i]-PEnemy->GetPos()).GetSquaredMagnitude2D()>=GFSqr(MINIMUM_DISTANCE_FROM_HERO_FOR_UNDEAD_CREATION))valid_positions.push_back(positions[i]);
   }
   if(valid_positions.size()>0){i=rand()%(long)valid_positions.size();RisePos=valid_positions[i];}
   Terminate();return;
  }
 }
 AIAction_DoNothing();
 RisePos=GetCreature().GetPos();
}
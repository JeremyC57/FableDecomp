// CTCGhostSword::FrameUpdate() -- fade summoned swords and remove the spell when all are dead.
// Leaf: ?FrameUpdate@CTCGhostSword@@UAEXXZ
// Retail:007c6460,240 bytes,thiscall
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCGHOSTSWORD_FRAME
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCGhostSword.h"
void CTCGhostSword::FrameUpdate()
{
 for(long i=0;i<(long)CurFadeInTimer.size();++i){
  if(CurFadeInTimer[i]>0){
   CurFadeInTimer[i]--;
   if(IS_THING_ALIVE(Minions[i])){
    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=Minions[i]->GetTCGraphicAppearance();
    if(ptc_graphic_appearance){
     float ratio=(float)CurFadeInTimer[i]/FadeInTimer;
     ptc_graphic_appearance->SetAlpha((unsigned char)((1.0f-ratio)*255.0f));
    }
   }
  }
 }
 UpdateGhostSwordParticleEffects();
 CThingCreatureBase* pcreature=GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
 bool all_dead=true;
 for(long i=0;i<Minions.size();++i){
  if(IS_THING_ALIVE(Minions[i])){all_dead=false;break;}
 }
 if(all_dead)pcreature->RemoveTC(TCI_GHOST_SWORD);
}
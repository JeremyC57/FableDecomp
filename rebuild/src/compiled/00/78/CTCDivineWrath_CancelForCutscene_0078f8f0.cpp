// CTCDivineWrath::CancelForCutscene() -- a cutscene cancels the spell: reset every creature it hit, mark the cutscene cancel, fade the environment back at once, finish the caster's current action, unlock its special-abilities mutex, kill the build-up effect, fade the hand effects, stop the looping sound and remove the component
// Leaf: ?CancelForCutscene@CTCDivineWrath@@QAEXXZ
// Retail: 0x0078F8F0, 505 bytes, thiscall, plain `ret`.
// Spelled as its landed twin CTCUnholyPower::CancelForCutscene 0x00794E60 (same TU family, same callees).
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#define FABLE_RETAIL_CTCDIVINEWRATH_STOPSOUND_INLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCDivineWrath.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/ThingAlive.h"
void CTCDivineWrath::CancelForCutscene()
{
 std::map<CThingCreatureBase*,PerCreatureInfo>::iterator endit=LastFrameCreaturesHit.end();
 for(std::map<CThingCreatureBase*,PerCreatureInfo>::iterator it=LastFrameCreaturesHit.begin();it!=endit;++it) {
  CIntelligentPointer<CThingCreatureBase> something=it->second.ActualCreature;
  ResetCreatureHitState(something);
 }
 CutSceneCancel=true;
 GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2,0.001f);
 CThingCreatureBase& parent_creature=static_cast<CThingCreatureBase&>(GetParentThing());
 parent_creature.FinishCurrentAction();
 CTCSpecialAbilities* ptc_spec_abils;
 if(parent_creature.GetTC(&ptc_spec_abils))ptc_spec_abils->UnLockAbilitiyMutex();
 if(IS_THING_ALIVE(BuildUpEffect))BuildUpEffect->Kill(true);
 if(IS_THING_ALIVE(HandEffect1))CTCSpecialAbilities::FadeOrKillEffect(HandEffect1);
 if(IS_THING_ALIVE(HandEffect2))CTCSpecialAbilities::FadeOrKillEffect(HandEffect2);
 StopSound(LoopingSound);
 parent_creature.RemoveTC(TCI_DIVINE_WRATH);
}
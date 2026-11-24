// CTCTrophy::StartMinigame -- initialise witnesses, timer, sound, HUD and hero counter.
// Leaf: ?StartMinigame@CTCTrophy@@AAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCTROPHY_PLAYSOUND_BODY_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCEMOTEICON_STATTYPE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATEGROUPENABLED
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCCARRYABLE_NONCONST_CARRIER
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_CLEARPERSONALITY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_TROPHYGAME_PASS26
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CDRAWQUESTINFO_UPDATEMINIGAME
#define FABLE_RETAIL_CTCHEROSTATS_ADDRENOWN
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_REGIONLOADCONFIRMATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS3
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_OWNER_MODE_PASS26
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SheatheItemToInventory.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCTrophy.h"
#include "engine/ThingAlive.h"

extern unsigned long g_InvalidSoundHandle;                               // 0x0129D25C (the established descriptive spelling)

// CTCTrophy::PlaySoundW 0x006DDCD0 (landed): the body of this TU, defined ahead of the row as in retail (the chunk 2007 rows' form),
// where VC7.1 expands it at both score sites.
void CTCTrophy::StartMinigame()
{
 MinigameStartingWitnessCount=AvailableWitnesses.size();
 NumberOfWitnessesLeft=MinigameStartingWitnessCount;
 WitnessesAchieved.clear();
 for(std::list<CIntelligentPointer<CThingAICreature> >::iterator it=AvailableWitnesses.begin(),end_it=AvailableWitnesses.end();it!=end_it;++it) {
  if(IS_THING_ALIVE(*it)) {
   (*it)->SetStateGroupEnabled("SG_OPINION_REACTION_ROOT",false);
   CTCEmoteIcon* ptc_emoteicon;
   (*it)->GetTC(&ptc_emoteicon);
   if(PDef->EmoteIconForAvailableWitness)ptc_emoteicon->SetEmoteIcon((CTCEmoteIcon::EEmotePriority)2,PDef->EmoteIconForAvailableWitness,1000000.0f,1.0f,255);
  }
 }
 MinigameInProgress=true;
 MinigameInitialFramesLeft=(long)(CMainGameComponent::GetConstantFPS()*PDef->MinigameInitialTimerSecs);
 MinigameCurrentFramesLeft=MinigameInitialFramesLeft;
 GetParentThing().SetAsGameStatePersisted(true);
 PlaySoundW(PDef->SoundStartGame);
 CPlayerGui::Get().GetQuestInfo().DisplayMiniGameInfo(true,MINIGAME_TROPHY,(long)&GetParentThing());
 CPlayerGui::Get().GetQuestInfo().UpdateMiniGame(1.0f,0,MinigameStartingWitnessCount);
 CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 CTCHeroStats* HeroStats;
 if(phero->GetTC(&HeroStats))HeroStats->SetNumTrophyGamePlayed(HeroStats->GetNumTrophyGamePlayed()+1);
}
// 00683c10 CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex
// Flags: none
// Leaf: ?GetGreetDialogueGroupIndex@CTCCreatureOpinionOfHero@@QAEKW4EOpinionAttitudeType@@@Z
// Identity: both original PDBs; FableWin01f47e40 call graph and native callers.
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_GREET
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/EHeroTrainingStatus.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHero.h"
#include "engine/CTCDrunkenness.h"
#include "engine/CTCOpinionSource.h"
#include "engine/CTCWife.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
// Both original name sets and donor call01f48152; native004978a0.
float FABLE_FASTCALL GFFloatRandom(float,unsigned long*);

unsigned long CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex(EOpinionAttitudeType for_attitude)
{
    if (!(GetParentThing().GetTypeFlags() & 8)) return 0;
    const NGameText::CDataBank& text_bank=CMainGameComponent::Get()->GetTextBank();
    const CThingPlayerCreature* pplayer_thing=GetWorld().PeekMyPlayer()->PeekPControlledCreature();
    const CTCHeroStats* ptc_hero_stats=0;
    EHeroTrainingStatus hero_training_status=(pplayer_thing && pplayer_thing->PeekTC(&ptc_hero_stats))
        ?ptc_hero_stats->GetTrainingStatus():TRAINING_STATUS_GRADUATED;
    if (GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0)->GetProperty()!=3) {
        CCharString group_name("");
        const CTCDrunkenness* ptc_drunk=0;
        if (GetParentThing().PeekTC(&ptc_drunk) && ptc_drunk->IsDrunk()) {
            group_name="TEXT_AI_OPINION_ACT_GREET_TO_HERO_DRUNK";
            return text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
        }
        const CTCOpinionSource* ptc_source=0;
        if (pplayer_thing) pplayer_thing->PeekTC(&ptc_source);
        const COpinionReactionManager& reaction_manager=PeekWorld().PeekOpinionReactionManager();
        EOpinionAttitudeType attitude=for_attitude;
        if (attitude==OPINION_ATTITUDE_TYPE_LAST) {
            attitude=reaction_manager.GetAttitudeType(GetParentThing(),ptc_source);
            if (!ptc_source) attitude=reaction_manager.GetBlurredAttitude(attitude);
        }
        const CTCWife* ptc_wife=0;
        if (!(attitude==OPINION_ATTITUDE_TYPE_DISDAIN || attitude==OPINION_ATTITUDE_TYPE_OFFENSIVE || attitude==OPINION_ATTITUDE_TYPE_FEAR)) {
            float prob_of_title_greet=reaction_manager.GetThingOpinionOfPlayer(GetParentThing(),OPINION_RENOWN);
            prob_of_title_greet=prob_of_title_greet*prob_of_title_greet*0.35f;
            if (!ptc_source && GFFloatRandom(1.0f,GetPWorldSeed())<prob_of_title_greet) {
                if (!(GetParentThing().HasTC(TCI_WIFE) && GetParentThing().GetTC(&ptc_wife) && ptc_wife->IsMarriedToThePlayer())) {
                    const CTCHero* ptc_hero=0;
                    if (pplayer_thing->PeekTC(&ptc_hero) && ptc_hero->HasHeroTitleSubDef() && ptc_hero->PeekHeroTitleSubDef().HeroTitleGreetToHeroTag>0)
                        return ptc_hero->PeekHeroTitleSubDef().HeroTitleGreetToHeroTag;
                }
            }
        } else if (GetParentThing().PeekTC(&ptc_wife) && ptc_wife->IsMarriedToThePlayer() && ptc_wife->GetGreetDialogueGroupName(group_name)) {
            unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
            return group_index;
        }
        if (hero_training_status!=TRAINING_STATUS_GRADUATED) {
            group_name=GreetedFlag?"TEXT_AI_OPINION_ACT_REGREET_TO_HERO":"TEXT_AI_OPINION_ACT_GREET_TO_HERO";
            if (hero_training_status==TRAINING_STATUS_BOY) group_name+="_ASBOY";
            else if (hero_training_status==TRAINING_STATUS_APPRENTICE) group_name+="_ASAPPRENTICE";
            unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
            return group_index;
        }
        const CCharString& att_str=reaction_manager.GetAttitudeString(attitude);
        if (att_str!="") {
            group_name=GreetedFlag?"TEXT_AI_OPINION_ACT_REGREET_TO_HERO_":"TEXT_AI_OPINION_ACT_GREET_TO_HERO_";
            group_name+=att_str;
            unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
            if (group_index>0) return group_index;
        }
    } else {
        CCharString group_name(GreetedFlag?"TEXT_AI_GUARD_REGREET_TO_HERO":"TEXT_AI_GUARD_GREET_TO_HERO");
        if (hero_training_status==TRAINING_STATUS_BOY) group_name+="_ASBOY";
        else if (hero_training_status==TRAINING_STATUS_APPRENTICE) group_name+="_ASAPPRENTICE";
        unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
        return group_index;
    }
    return 0;
}
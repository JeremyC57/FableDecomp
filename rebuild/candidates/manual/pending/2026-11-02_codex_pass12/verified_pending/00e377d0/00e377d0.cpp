// NScript::CV_AssassinAttacksScript::Main() -- register assassins, wait for the activation gates and reward the completed attacks.
// Leaf: ?Main@CV_AssassinAttacksScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_BANDIT_CUTSCENE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_QUEST_SCRIPT_PERSIST
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_AssassinAttacksScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
void NScript::CV_AssassinAttacksScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AssassinAttacksScript>("M_AssassinTrigger",this,&CM_AssassinTrigger::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AssassinAttacksScript>("AttackingAssassin",this,&CAttackingAssassin::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AssassinAttacksScript>("ChestHidingAssassin",this,&CChestHidingAssassin::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AssassinAttacksScript>("AssassinChest",this,&CAssassinChest::Alloc,0));
    PostAddScriptedEntities();Game.PostAddScriptedEntities();
    CCharString regionName;
    while(!(Game.GetHeroRenownLevel()>pGlobalScriptDef->AssassinActivateRenownLevel&&Game.MsgOnRegionLoaded(regionName)&&(MasterScriptData.BreakSiegeFinished||Game.IsQuestCompleted("Q_BreakSiege")))){
        Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    InitialiseRegionSpecificInfo();
    if(!Activated){
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.ActivateQuest("V_AssassinAttacks_Activate");
        if(MasterScriptData.BanditCampTwinbladeKilled){
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_007_GM_INTRO_EVIL","",true,true);
        }else{
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_007_GM_INTRO_GOOD","",true,true);
        }
        Activated=true;
    }
    while(!QuestComplete){Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    Game.Pause(pGlobalScriptDef->AssassinRewardDelay);
    Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_007_SUCCESS","",true,true);
    Game.Pause(pGlobalScriptDef->AssassinRewardDelay);
    if(!Game.IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3",Game.GetHero())){
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.GiveHeroObject("OBJECT_TREASURE_CLUE_3",-1,false);
    }
    Game.GiveHeroGold(pGlobalScriptDef->AssassinReward);
    Game.GiveHeroMorality(pGlobalScriptDef->AssassinMorality);
    Game.GiveHeroRenownPoints(pGlobalScriptDef->AssassinRenown);
    RewardsGiven=true;
}

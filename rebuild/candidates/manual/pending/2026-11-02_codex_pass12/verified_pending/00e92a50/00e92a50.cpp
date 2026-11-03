// NScript::CV_HiddenBootyScript::Main() -- wait for a treasure clue, activate the quest, then award the appropriate completion renown.
// Leaf: ?Main@CV_HiddenBootyScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_BANDIT_CUTSCENE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_HiddenBootyScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
void NScript::CV_HiddenBootyScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HiddenBootyScript>("M_HiddenBooty",this,&CM_HiddenBooty::Alloc,0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    CCharString regionName;
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    while(true){
        if(Game.GetHero().MsgReceivedInventoryItem()){
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(Game.IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_1",Game.GetHero())||
               Game.IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_2",Game.GetHero())||
               Game.IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3",Game.GetHero())||
               Game.IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_4",Game.GetHero())||
               Game.IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_5",Game.GetHero())||
               Game.IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_6",Game.GetHero()))break;
        }
        Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(!QuestActivated){
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.ActivateQuest("V_HiddenBooty_Activate");
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_033_GM_INTRO","",true,true);
        Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HIDDEN_BOOTY",Game.GetActiveQuestName(),true);
        QuestActivated=true;
    }
    while(!QuestComplete){Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(RealBootyAddedToDiggingSpot){
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.GiveHeroRenownPoints(pGlobalScriptDef->HiddenBootyRenownReal);
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_033_GM_OUTRO","",true,true);
    }else if(FakeBootyAddedToDiggingSpot){
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.GiveHeroRenownPoints(pGlobalScriptDef->HiddenBootyRenownFake);
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_033_GM_OUTRO_FAKE","",true,true);
    }
    Game.DeactivateQuestLater("V_HiddenBooty_Activate",0);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(),0);
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(),false,false,false);
}

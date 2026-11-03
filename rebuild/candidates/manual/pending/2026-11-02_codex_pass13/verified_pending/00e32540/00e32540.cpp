// NScript::CV_ArcheryCompetitionScript::Main() -- activate and coordinate the archery competition.
// Leaf: ?Main@CV_ArcheryCompetitionScript@NScript@@UAEXXZ
// Grade: RELOCATION_MATCH(2468v2468) -- exact original spawned-function header ctor needs stronger expansion.
// Annotation: Newest Owner conditional header-helper preference; original keyword unproven, ordinary history retained.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSPAWNEDFUNC
#define FABLE_RETAIL_CSPAWNEDFUNC_EXPAND_CTOR
#define FABLE_RETAIL_CENTITYSCRIPTBINDING
#include "engine/CV_ArcheryCompetitionScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFunc.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
extern CGameScriptInterfaceBase* pDSTGame;
inline bool CTimer::operator==(long n) const { return pDSTGame->GetTimer(TimerIndex)==n; }

template <class T> inline T* CLevelScriptBase<T>::GetMasterData() { return &MasterScriptData; }

void NScript::CV_ArcheryCompetitionScript::Main()
{
    CCharString regionName;
    UpdateHighScore(MasterScriptData.ArcheryHighScore);
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ArcheryCompetitionScript>("AC_Owner",this,&CAC_Owner::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ArcheryCompetitionScript>("AC_DummyStatic",this,&CAC_DummyStatic::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ArcheryCompetitionScript>("AC_DummyMoving",this,&CAC_DummyMoving::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ArcheryCompetitionScript>("AC_HighScorePost",this,&CAC_HighScorePost::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ArcheryCompetitionScript>("M_AC_DummyStatic",this,&CM_AC_DummyStatic::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ArcheryCompetitionScript>("M_AC_DummyMoving",this,&CM_AC_DummyMoving::Alloc,0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        while (!Game.MsgOnRegionLoaded(regionName) && !QuestActivated) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (QuestActivated) {
            if (IsActiveThreadTerminating()) return;
            break;
        }
        if (regionName!="KnotholeGlade") {
            if (IsActiveThreadTerminating()) return;
            bool arena=MasterScriptData.ArenaFinished || Game.IsQuestCompleted("Q_Arena");
            bool siege=MasterScriptData.BreakSiegeFinished || Game.IsQuestCompleted("Q_BreakSiege");
            bool balv=MasterScriptData.WhiteBalverineFinished || Game.IsQuestCompleted("Q_WhiteBalverineWW");
            if (arena && siege && balv) {
                if (IsActiveThreadTerminating()) return;
                break;
            }
        }
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (!QuestActivated) {
        if (IsActiveThreadTerminating()) return;
        Game.ActivateQuest("V_ArcheryCompetition_Activate");
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_004_GM_INTRO","",true,true);
        QuestActivated=true;
        GetMasterData()->ArcheryHighScore=pGlobalScriptDef->ArcheryInitialHighScoreMax-rand()%pGlobalScriptDef->ArcheryInitialHighScoreVar;
    }
    AddSpawnedFunction(new CSpawnedFunc<CV_ArcheryCompetitionScript>("WatchForQuestFinished",0,this,&CV_ArcheryCompetitionScript::WatchForQuestFinished),CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ArcheryCompetitionScript>("WatchForOwnerKilled",0,this,&CV_ArcheryCompetitionScript::WatchForOwnerKilled),CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ArcheryCompetitionScript>("WatchForSignpostConditions",0,this,&CV_ArcheryCompetitionScript::WatchForSignpostConditions),CCharString(""));
    Game.SetCreatureGeneratorsEnabled("KnotholeGlade",false);
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        Score=0;
        UpdateGameScore();
        while (ContestState!=CONTEST_STARTED && !QuestFinished) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (QuestFinished) {
            if (IsActiveThreadTerminating()) return;
            break;
        }
        SteppedOut=false;
        ShotOwner=false;
        UsedMagic=false;
        AddArcheryQuestInfo();
        while (!(GameTimeLeft==0) || ContestState==CONTEST_FINISHED) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (ContestState==CONTEST_STARTED) {
            if (IsActiveThreadTerminating()) return;
            ContestState=CONTEST_FINISHED;
        }
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (ContestState!=CONTEST_WAITING && !QuestFinished) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        RemoveArcheryQuestInfo();
        if (QuestFinished) {
            if (IsActiveThreadTerminating()) return;
            break;
        }
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.RemoveQuestCardFromHero(Game.GetActiveQuestName());
    while (!Game.MsgOnRegionLoaded(regionName)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.SetCreatureGeneratorsEnabled("KnotholeGlade",true);
    Game.DeactivateQuestLater("V_ArcheryCompetition_Activate",0);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(),0);
}

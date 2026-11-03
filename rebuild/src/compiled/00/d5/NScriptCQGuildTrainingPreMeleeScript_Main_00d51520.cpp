// NScript::CQ_GuildTrainingPreMeleeScript::Main() -- stage the opening guild lesson, cutscenes and expressions.
// Leaf: ?Main@CQ_GuildTrainingPreMeleeScript@NScript@@UAEXXZ
// Grade: RELOCATION_MATCH(2280v2280) -- genuine timer ctor/dtor context and documented header helper expansion.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT
#define FABLE_RETAIL_STLP_MAP
#include "engine/CQ_GuildTrainingPreMeleeScript.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "stlport/fable_map.h"

enum EScriptAIPriority {
    SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2,
    SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4
};
extern CGameScriptInterfaceBase* pDSTGame;
void __fastcall RunCutsceneMacro(const CCharString&,
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,
    std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);

inline CScriptThing::~CScriptThing() {}
#pragma auto_inline(off)
CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#pragma auto_inline(on)
__forceinline CTimer& CTimer::operator=(long n) { pDSTGame->SetTimer(TimerIndex,n); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator>(long n) const { return GetTime()>n; }

void NScript::CQ_GuildTrainingPreMeleeScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingPreMeleeScript>("TheRealGuildmaster",this,&CTheRealGuildmaster::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingPreMeleeScript>("PreMeleeDummy",this,&CPreMeleeDummy::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingPreMeleeScript>("PreMeleeWhisper",this,&CPreMeleeWhisper::Alloc,1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    MasterScriptData->GuildWarningOccuring=true;
    CScriptThing Maze=Game->GetThingWithScriptName("PreMeleeMaze");
    CScriptThing Whisper=Game->GetThingWithScriptName("PreMeleeWhisper");
    CScriptThing GuildMaster=Game->GetThingWithScriptName("TheRealGuildmaster");
    Game->GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER");
    PreMeleeMode=0;
    {
        CScriptGameResourceObjectScriptedThingBase seh_Maze;
        if (seh_Maze.IsValid()) seh_Maze.Release();
        while (!Game->StartScriptingEntity(Maze,seh_Maze,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_Whisper;
        if (seh_Whisper.IsValid()) seh_Whisper.Release();
        while (!Game->StartScriptingEntity(Whisper,seh_Whisper,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_GuildMaster;
        if (seh_GuildMaster.IsValid()) seh_GuildMaster.Release();
        while (!Game->StartScriptingEntity(GuildMaster,seh_GuildMaster,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid()) seh_Hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_Hero;
        mymap["MAZE"]=seh_Maze;
        mymap["WHISPER"]=seh_Whisper;
        mymap["MASTER"]=seh_GuildMaster;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_INTRO",mymap,0,0,false,true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_BOOHOO",mymap,0,0,false,true);
        RunCutsceneMacro("CS_GUILD_PREMELEE_WAKEUP",mymap,0,0,false,true);
        Game->FixMovieSequenceCamera(false);
        Game->ChangeHeroHealthBy(1000.0f,true,false);
        Game->ResetPlayerCreatureCombatMultiplier();
    }
    WhisperCutsceneFinished=true;
    GuildmasterTeleport=true;
    MasterScriptData->GuildWarningOccuring=false;
    {
    CTimer ExpressionTimer;
    ExpressionTimer=5;
    while (ExpressionTimer.GetTime()>0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->GiveHeroExpression("EXPRESSION_FART",-1,true);
    Game->GiveHeroExpression("EXPRESSION_BELCH",-1,true);
    Game->GiveHeroExpression("EXPRESSION_GIGGLE",-1,true);
    while (!HeroSleeps) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(),0);
    }
}
// NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Main() -- control access to upper Bowerstone after the Arena.
// Leaf: ?Main@CBowerstoneIntroGuardToPosh@CQ_BowerstoneTownLifeIntroScript@NScript@@EAEXXZ
// Grade: DIFFER(2200v2403) -- implicit resource expansion remains; generated special members are not authored.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_STLPORT_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CQ_BowerstoneTownLifeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float);
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);
inline bool CScriptThing::IsAlive() const { if (PImp.IsNull()) return false; return PImp->IsAlive(); }

void NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (sehMe.IsValid()) sehMe.Release();
    while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.DoneGuardSpeech) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing faceMe=Game.GetThingWithScriptName("M_BTLI_GuardPoshFaceMe");
    if (faceMe.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetFacingAngleTowardsThing(Me,faceMe,true);
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!IsDistanceBetweenThingsUnder(Game.GetHero(),Me,pGlobalScriptDef->BowerstoneIntroPoshGuardProximity)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.ArenaFinished) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.PoshGuardComplete) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                CScriptGameResourceObjectScriptedThingBase seh_Hero;
                while (!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                mymap["HERO"]=seh_Hero;
                mymap["Guard"]=sehMe;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_BOWERSTONE_INTRO_POSH_ENTER",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                ParentClass.PoshGuardComplete=true;
                if (sehMe.IsValid()) sehMe.Release();
                Game.EntityUnsetThingAsAllyOfThing(Me,Game.GetHero());
                Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(),Me);
                Game.ClearThingHasInformation(Me);
            } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game.GetHero(),"TEXT_QST_037_GUARD_POSH_REMINDER",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (sehMe.IsValid()) sehMe.Release();
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            CScriptGameResourceObjectScriptedThingBase seh_Hero;
            while (!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            mymap["HERO"]=seh_Hero;
            mymap["Guard"]=sehMe;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_BOWERSTONE_INTRO_POSH_LEAVE",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

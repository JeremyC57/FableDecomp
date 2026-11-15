// 00d29880 NScript::CQ_EndGameScript::CHookCoastTeleporter::Main (assigned tail 00d2988b)
// Inline-budget: ordinary original members expand at T1000; scoped depth0 preserves native called constructors/destructors. No forced-inline member opt-in is used.
// Flags: /d2inlT1000
// Leaf: ?Main@CHookCoastTeleporter@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: 0x00D29880, 1319 bytes, thiscall; authorized 0x00D2988B slice omits the proven eleven-byte head.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CRGBCOLOUR
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_EndGameScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
extern CGameScriptInterfaceBase* pDSTGame;
void FABLE_FASTCALL AddLogbookStoryEntry(long);
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time);return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time)const { return GetTime()==time; }
void NScript::CQ_EndGameScript::CHookCoastTeleporter::Main()
{
    TRetVoid _Fail_Val;
    bool PreviouslyKilledEnoughUndead=ParentClass.UndeadBar>=pGlobalScriptDef->EndGame_UpperBarLimit;
    if(ParentClass.UndeadBar<pGlobalScriptDef->EndGame_BarDecrementAmount) {
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_10","",true,true);
        Game.Pause(3.0f);
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_20","",true,true);
        Game.Pause(4.0f);
        ParentClass.ShowUndeadBar=true;
    }
    {
        CTimer DecrementTimer;
        DecrementTimer=1;
        for(;ParentClass.UndeadBar<pGlobalScriptDef->EndGame_UpperBarLimit;) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(DecrementTimer==0) {
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(ParentClass.UndeadBar<pGlobalScriptDef->EndGame_BarDecrementAmount) {
                    if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    ParentClass.UndeadBar=0;
                } else {
                    if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    ParentClass.UndeadBar-=pGlobalScriptDef->EndGame_BarDecrementAmount;
                }
                DecrementTimer=pGlobalScriptDef->EndGame_BarDecrementInterval;
            }
            Game.UpdateQuestInfoBar(ParentClass.GuiCounter,ParentClass.UndeadBar,-1.0f,-1.0f);
        }
        if(IsActiveThreadTerminating())goto Terminating;
        Game.RemoveQuestInfoElement(ParentClass.GuiCounter);
        Game.MiniMapRemoveMarker(Me);
        if(!PreviouslyKilledEnoughUndead) {
            if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
            {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                CScriptGameResourceObjectScriptedThingBase seh_hero;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                if(seh_hero.IsValid())seh_hero.Release();
                while(!Game.StartScriptingEntity(Game.GetHero(),seh_hero,(EScriptAIPriority)4)) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                }
                if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    RunCutsceneMacro("CS_GATEWAY_OPEN",mymap,0,0,false,true);
                }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game.FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0));
            Game.ActivateQuestWithoutLoadingResources("Q_EndGame_Barriers");
            Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("MG_EndGameTeleport"),false);
            AddLogbookStoryEntry(240);
        }
        while(true) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        }
    }
    Terminating:
    return _Fail_Val.Return();
}

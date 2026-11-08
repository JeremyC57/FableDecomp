// NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::Main() -- the ghost granny's brain: take her under scripted control, then run the BrainState loops -- GS_INTRO (once the quest card is given and the hero comes within GGN_GhostTalkDistance, play CS_GHOSTGRANNY_GRANNY, set objective 2 and move to GS_GIVING_CLUES), GS_GIVING_CLUES (a timed clue conversation, the intro reprise when talked to, state changes when the bandit dies or the necklace is returned), GS_NECKLACE_NOT_RETURNED (a timed reminder conversation) and GS_GOING_TO_REST (the farewell conversation, the Ghost_Appear_01 effect, fade out and kill)
// Leaf: ?Main@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8DFA0, 4056 bytes, thiscall, plain `ret`, no EH frame. Its last instruction `jmp 0x00E8E7C5`
//   is the farewell wait loop's terminate exit into the shared epilogue (not a loop back-edge, not a tail call).
// Identity (proven): slot 1 (Main) of the CGhostGrannyNecklace entity vtable (the single .rdata reference; the landed Alloc 0x00E8BD30
//   stores that vtable); Ego_r.pdb name set ?Main@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@EAEXXZ (private virtual).
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840, CIsThingAlive vtable 0x012C32F0 / ~ 0x00E241B0, CEntityScriptBase::SetPredicate
//   0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380, scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 /
//   operator= 0x008ABD10 / IsValid 0x00CD23B9 / Release 0x00CD2770, movie ctor 0x006E7B60 / dtor 0x006E7B80, cast map ctor 0x00CDBF70 /
//   dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity
//   0x00CBE716, operator+(const CCharString&, const char*) 0x0099F600, ~CTimer 0x00CD4470, CCharString 0x0099EBF0 / ~ 0x0099EAE0;
//   globals pDSTGame 0x0143E8F8 (timer slots 87-90), pGlobalScriptDef 0x0143E90C (GGN_GhostClueTime +0x65C, GGN_GhostTalkDistance +0x660,
//   TollBanditTimeToFade +0x440).
// Layout: engine/CV_GhostGrannyNecklaceScript.h (BrainState +0x1C; parent flags HeroKnowsAboutNecklace +0x49, NecklaceReturned +0x4A,
//   BanditKilled +0x4B, GhostToRest +0x4E, QuestCardGiven +0x4F).
// Shape: the landed sibling CGhostGrannyDaughterInLaw::Main 0x00E8BEF0 and the cast-map cutscene of COakValeBanditRaidBandit::Main
//   0x00DC1770. CTimer::operator= and GetTime must be __forceinline (the long-Main rule of GOTCHAS chunk 781 / 797): with plain inline
//   VC7.1 calls operator= out of line, and the GS_INTRO loop body is then laid out inline instead of after the epilogue as retail does.
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include <string.h>
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_GhostGrannyNecklaceScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);   // 0x00CBE716
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
void NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetAllStategroupsEnabled(Me, false);
    Game->EntitySetTargetable(Me, false);
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer ClueTimer;
    CTimer TalkTimer;
    long TalkDistance = pGlobalScriptDef->GGN_GhostTalkDistance;
    bool InformationSet = false;
    while (BrainState == GS_INTRO) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!InformationSet && ParentClass.QuestCardGiven) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->SetThingHasInformation(Me, false, false, false);
            InformationSet = true;
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)TalkDistance) && TalkTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.QuestCardGiven) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"] = seh_hero;
                mymap["GRAN"] = seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_GHOSTGRANNY_GRANNY", mymap, 0, 0, false, true);
                Game->FixMovieSequenceCamera(false);
                ParentClass.HeroKnowsAboutNecklace = true;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GHOST_GRANNY_NECKLACE_OBJECTIVE_02", "GreatwoodLake", "");
                BrainState = GS_GIVING_CLUES;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer = pGlobalScriptDef->GGN_GhostClueTime;
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (BrainState == GS_GIVING_CLUES) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ClueTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)TalkDistance)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_026_GHOST_CLUE", false, Me, Game->GetHero());
            while (Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ClueTimer = pGlobalScriptDef->GGN_GhostClueTime;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (ParentClass.NecklaceReturned) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            BrainState = GS_GOING_TO_REST;
        }
        if (ParentClass.BanditKilled) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            BrainState = GS_NECKLACE_NOT_RETURNED;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_026_GHOST_INTRO_BRIEF_REPRISE_01", false, Me, Game->GetHero());
            while (Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer = pGlobalScriptDef->GGN_GhostClueTime;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (BrainState == GS_NECKLACE_NOT_RETURNED) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.NecklaceReturned) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            BrainState = GS_GOING_TO_REST;
        }
        if (TalkTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)TalkDistance)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_026_GHOST_NOT_RETURNED_NECKLACE_10", false, Me, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_026_GHOST_NOT_RETURNED_NECKLACE_20", false, Me, Game->GetHero());
            while (Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer = pGlobalScriptDef->GGN_GhostClueTime;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (BrainState == GS_GOING_TO_REST) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)TalkDistance) && !ParentClass.GhostToRest) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.GhostToRest = true;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, Game->GetValidTextEntryNameWithAttitude(Me, "TEXT_QST_026_GHOST_FAREWELL") + "_10", false, Me, Game->GetHero());
            Game->AddLineToConversation(conversation, Game->GetValidTextEntryNameWithAttitude(Me, "TEXT_QST_026_GHOST_FAREWELL") + "_20", false, Me, Game->GetHero());
            Game->AddLineToConversation(conversation, Game->GetValidTextEntryNameWithAttitude(Me, "TEXT_QST_026_GHOST_FAREWELL") + "_30", false, Me, Game->GetHero());
            while (Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->CreateEffect("Ghost_Appear_01", Me.GetPos(), "", 0.0f, false, false);
            FadeOutAndKillEntity(Me, true, pGlobalScriptDef->TollBanditTimeToFade, true);
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
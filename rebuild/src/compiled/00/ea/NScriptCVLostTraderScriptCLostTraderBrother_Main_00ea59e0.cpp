// NScript::CV_LostTraderScript::CLostTraderBrother::Main() -- the lost trader's brother: scripts himself, spawns the parent's quest-failure watcher, plays the intro cut-scene once (quest start screen, sad mode), walks home to TomHomePos stopping to call the hero on when the hero lags, then waits for the hero (asking for help), answers talk until his brother is found, keeps answering until the trader reaches him, and plays the reunion cut-scene (quest completed, morality, both brothers removed)
// Leaf: ?Main@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA59E0, 6780 bytes (incl. the 4-entry switch table 0x00EA744C), thiscall, plain `ret`.
// Identity (proven): the only reference is .rdata 0x012EE038, slot 1 of the CLostTraderBrother entity vtable (slot 0 the landed scalar
//   deleting dtor 0x00EA5130, slot 2 the landed Init 0x00EA50A0, slot 4 the landed OnPersist 0x00EA5990); Ego_r.pdb and FableWin.pdb
//   name sets both hold ?Main@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXXZ (private virtual).
// Callees: SetPredicate 0x00F35B10 over the CIsThingAlive copy 0x004ABE90 / ~CIsThingAlive 0x00E241B0 (called), IsActiveThreadTerminating
//   0x00F35B30, operator new 0x00BFEA1A, CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc1<CV_LostTraderScript,CScriptThing> vtable
//   0x012EE360, Func = WatchForQuestFailureState 0x00EA7460), CCharString operator+ 0x0099F570, AddSpawnedFunction 0x00CB7E50, the
//   scripted-thing handle (seh_me expanded; ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10 for the other handles, IsValid
//   0x00CD23B9, Release 0x00CD2770, ClearCommands 0x007E7360, MoveToPosition 0x007E72F0, ClearAllActions 0x007E7400, Speak 0x007E7390,
//   IsPerformingScriptTask 0x007E7450, GetScriptThing 0x007E7490), the cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[]
//   0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, IsDistanceBetweenThingsUnder 0x00CBE2FF and
//   GetDistanceBetweenThings 0x00CBE512 (__fastcall), rand 0x00BFEB16, ~CScriptThing 0x004AA840, CScriptThing::NullCoord 0x0143E8E0
//   (inline GetPos), pGlobalScriptDef 0x0143E90C (BrotherWaitCommentRandomValue +0x6F8, HeroGoodMoralityValue +0x700).
// Layout: engine/CV_LostTraderScript.h (stateBrain +0x1C, HasToldHeroWhereBrotherIs +0x20, TalkCounter +0x24, DoneIntro +0x28;
//   parent ReachedTrader +0x4C, DistanceForBrotherToReunite +0x50) with the chunk 812 BROTHER opt-in (WatchForQuestFailureState).
// Shape: seh_me first (its inline ctor expanded; VC7.1 expands the dtor at the first loop exit only), then the predicate (out-of-line
//   ~CIsThingAlive, as the landed CLostTraderTrader::Main). The intro, the state-3 walk home and the state-4 wait are three blocks:
//   the state-4 body must be its own `if (stateBrain == 4) { }` block (a3) so the Trader thing shares HomePos's slot +0x2C and both
//   exit through the shared ~CScriptThing pad (with `if (stateBrain != 4) return;` the trader got +0x40: a2, 16 lines). The hero-lag
//   test is `GetDistance(Me, Home) < GetDistance(Hero, Home)` (left operand first, `test ah,0x41`; a1 had the operands swapped).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#define FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BROTHER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CV_LostTraderScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&);   // 0x00CBE512
extern "C" int __cdecl rand();

void NScript::CV_LostTraderScript::CLostTraderBrother::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_LostTraderScript, CScriptThing>(CCharString("ParentClass.", -1) + CCharString("WatchForQuestFailureState", -1), 0, &ParentClass, &CV_LostTraderScript::WatchForQuestFailureState, Me), CCharString(""));
    if (!DoneIntro) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["TRADER"] = seh_me;
        mymap["HERO"] = seh_hero;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_LOST_TRADER_INTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        stateBrain = 3;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        Game->AddCreatureScriptedMode(Me, "SAD");
        DoneIntro = true;
    }
    if (stateBrain == 3) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing HomePos = Game->GetThingWithScriptName("TomHomePos");
        bool StoppedForHero = false;
        long MoveState = 0;
        seh_me.ClearCommands();
        seh_me.MoveToPosition(HomePos.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
        while (!IsDistanceBetweenThingsUnder(Me, HomePos, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            bool HeroFurther = GetDistanceBetweenThings(Me, HomePos) < GetDistanceBetweenThings(Game->GetHero(), HomePos);
            if (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 9.0f) && HeroFurther) {
                if (IsActiveThreadTerminating())
                    return;
                seh_me.ClearCommands();
                seh_me.ClearAllActions();
                if (!StoppedForHero) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    long HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_042_TRADERBROTHER_FOLLOW_ME", false, Me, Game->GetHero());
                    Game->Pause(1.0f);
                    StoppedForHero = true;
                    MoveState = 2;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                if (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f) && HeroFurther) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (MoveState != 1) {
                        if (IsActiveThreadTerminating())
                            return;
                        seh_me.ClearCommands();
                        seh_me.MoveToPosition(HomePos.GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
                        MoveState = 1;
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    if (MoveState != 0) {
                        if (IsActiveThreadTerminating())
                            return;
                        seh_me.ClearCommands();
                        seh_me.MoveToPosition(HomePos.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
                        MoveState = 0;
                    }
                }
                StoppedForHero = false;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        seh_me.ClearCommands();
        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
        stateBrain = 4;
    }
    if (stateBrain == 4) {
        if (IsActiveThreadTerminating())
            return;
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f) && !ParentClass.ReachedTrader) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!HasToldHeroWhereBrotherIs || rand() % pGlobalScriptDef->BrotherWaitCommentRandomValue == 0) {
            if (IsActiveThreadTerminating())
                return;
            long HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
            if (!HasToldHeroWhereBrotherIs) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_042_TRADERBROTHER_DEEPER", false, Me, Game->GetHero());
                HasToldHeroWhereBrotherIs = true;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_042_TRADERBROTHER_PLEASE", false, Me, Game->GetHero());
            }
        }
        Game->ClearThingHasInformation(Me);
        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
        while (!ParentClass.ReachedTrader) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                switch (TalkCounter) {
                case 0:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_042_TRADERBROTHER_PLEASE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    }
                    break;
                case 1:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_042_TRADERBROTHER_TALK_SECOND", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    }
                    break;
                case 2:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_042_TRADERBROTHER_TALK_THIRD", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    }
                    break;
                case 3:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_042_TRADERBROTHER_TALK_REPEAT_10", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    }
                    break;
                default:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_42_TRADERBROTHER_TALK_REPEAT", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    }
                    break;
                }
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                ++TalkCounter;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
        CScriptThing Trader = Game->GetThingWithScriptName("LostTraderTrader");
        TalkCounter = 0;
        while (!IsDistanceBetweenThingsUnder(Me, Trader, (float)ParentClass.DistanceForBrotherToReunite)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                switch (TalkCounter) {
                case 0:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_042_TRADERBROTHER_PLEASE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (seh_me.IsValid())
                            seh_me.Release();
                        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    break;
                case 1:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_042_TRADERBROTHER_FOUND_NOT_RETURNED_10", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (seh_me.IsValid())
                            seh_me.Release();
                        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    break;
                default:
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_42_TRADERBROTHER_FOUND_NOT_RETURNED", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (seh_me.IsValid())
                            seh_me.Release();
                        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    break;
                }
                ++TalkCounter;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptGameResourceObjectScriptedThingBase seh_trader;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(Trader, seh_trader, SCRIPT_AI_PRIORITY_HIGHEST);
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["TRADER"] = seh_me;
            mymap["LOST"] = seh_trader;
            mymap["HERO"] = seh_hero;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_LOST_TRADER_WON", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
        }
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        Game->GiveHeroMorality(pGlobalScriptDef->HeroGoodMoralityValue);
        Game->FadeScreenIn();
        Game->RemoveThing(Game->GetThingWithScriptName("LostTraderTrader"), false, true);
        Game->RemoveThing(Me, false, true);
    }
}
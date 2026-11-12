// NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main() -- the arena holding-room fan marker: drives the three fans (Oakvale, Bowerstone, Hook Coast). Until the fans may leave or Q_Arena is completed it plays the fans' hit lines when the hero attacks them (a paused movie the first time, a conversation if a hit scene is already running), after which they may leave and are killable; otherwise, with the hero close and the chat timer run down, the fans trade the next early chat line. Then, until they may leave or the arena is over, it plays the same hit scene or the next late chat line (the first gives the Oakvale fan the chainmail helmet, the seventh ends the arena), and finally idles
// Grade: DIFFER(6104v6164) -- Missing native 60-byte duplicate cold Movie cleanup CF65E4..CF661F; remaining switch-table difference and Pause/Bower/Oak stack-slot ordering. Ordinary body agrees under loose comparison; loose ratios are diagnostic, not acceptance.
// Inline-budget: S15 expands implicit Movie cleanup; scoped cold Timer returns preserve native calls. Plain Pause ctor/GetTime suffice; Timer assignment force is required.
// Flags: /d2inlS15
// Leaf: ?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Retail: 0x00CF4EC0, 6164 bytes, thiscall.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ;
//   one .rdata reference, slot 1 of the entity vtable 0x012C657C stored by CFanMarker::Alloc 0x00CF4DF0 (2 CEntityScriptBase::Init
//   0x00CDEBB0, GetParentScript 0x00CF4E90).
// Layout: engine/CQ_ArenaHoldingScriptScript.h with the chunk 812 FANMARKER opt-in (hdr/README.md).
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_FANMARKER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_ArenaHoldingScriptScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CScriptThing::~CScriptThing() { }
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF

void NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptThing OakValeFan = Game.GetThingWithScriptName("OakvaleFan");
    CScriptThing BowerstoneFan = Game.GetThingWithScriptName("BowerstoneFan");
    CScriptThing HookCoastFan = Game.GetThingWithScriptName("HookCoastFan");
    long ChatLevel = 0;
    Game.EntitySetAsKillable(OakValeFan, false, true);
    Game.EntitySetAsKillable(HookCoastFan, false, true);
    Game.EntitySetAsKillable(BowerstoneFan, false, true);
    CScriptGameResourceObjectScriptedThingBase seh_OakValeFan;
    CScriptGameResourceObjectScriptedThingBase seh_BowerstoneFan;
    CScriptGameResourceObjectScriptedThingBase seh_HookCoastFan;
    CTimer ChatTimer;
    ChatTimer = 0;
    while (!ParentClass.FansCanLeaveNow && !Game.IsQuestCompleted("Q_Arena")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.PlayerAttackedFans) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.StartScriptingEntity(OakValeFan, seh_OakValeFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(BowerstoneFan, seh_BowerstoneFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(HookCoastFan, seh_HookCoastFan, SCRIPT_AI_PRIORITY_HIGHEST);
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready = true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_OakValeFan.GetScriptThing()) > 0.0f) {
                    seh_OakValeFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_OakValeFan.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (Game.GetHealth(seh_BowerstoneFan.GetScriptThing()) > 0.0f) {
                    seh_BowerstoneFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_BowerstoneFan.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (Game.GetHealth(seh_HookCoastFan.GetScriptThing()) > 0.0f) {
                    seh_HookCoastFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_HookCoastFan.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                ParentClass.InHitCutsceneAlready = false;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", false, OakValeFan, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", false, HookCoastFan, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", false, BowerstoneFan, Game.GetHero());
            }
            ParentClass.FansCanLeaveNow = true;
            Game.EntitySetAsKillable(OakValeFan, true, true);
            Game.EntitySetAsKillable(HookCoastFan, true, true);
            Game.EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_OakValeFan.IsValid())
                seh_OakValeFan.Release();
            if (seh_HookCoastFan.IsValid())
                seh_HookCoastFan.Release();
            if (seh_BowerstoneFan.IsValid())
                seh_BowerstoneFan.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            switch (ChatLevel) {
            case 0:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIRST", false, BowerstoneFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel = 1;
                break;
            }
            case 1:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SECOND", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 2;
                break;
            }
            case 2:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_THIRD", false, HookCoastFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel = 3;
                break;
            }
            case 3:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FOURTH", false, HookCoastFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel = 4;
                break;
            }
            case 4:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIFTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 5;
                break;
            }
            case 5:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SIXTH", false, OakValeFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 6;
                break;
            }
            case 6:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SEVENTH", false, BowerstoneFan, OakValeFan);
                ChatLevel = 7;
                ChatTimer = 3;
                break;
            }
            case 7:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_EIGTH", false, OakValeFan, BowerstoneFan);
                ChatLevel = 8;
                ChatTimer = 3;
                break;
            }
            case 8:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_NINTH", false, BowerstoneFan, OakValeFan);
                ChatLevel = 9;
                ChatTimer = 3;
                ParentClass.OakvaleInsult = true;
                break;
            }
            case 9:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TENTH", false, OakValeFan, BowerstoneFan);
                ChatLevel = 10;
                ChatTimer = 3;
                break;
            }
            case 10:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_ELEVENTH", false, BowerstoneFan, OakValeFan);
                ChatLevel = 11;
                ChatTimer = 3;
                break;
            }
            case 11:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TWELFTH", false, OakValeFan, BowerstoneFan);
                ChatLevel = 12;
                ChatTimer = 3;
                break;
            }
            case 12:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_OAKVALE_INSULT", false, BowerstoneFan, OakValeFan);
                ChatLevel = 13;
                ChatTimer = 3;
                break;
            }
            case 13:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_BOWERSTONE_INSULT", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 12;
                break;
            }
            }
        }
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    ChatLevel = 0;
    while (!ParentClass.FansCanLeaveNow && !ParentClass.ArenaOver) {
        Game.NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (ParentClass.PlayerAttackedFans) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game.StartScriptingEntity(OakValeFan, seh_OakValeFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(BowerstoneFan, seh_BowerstoneFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(HookCoastFan, seh_HookCoastFan, SCRIPT_AI_PRIORITY_HIGHEST);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_OakValeFan.GetScriptThing()) > 0.0f) {
                seh_OakValeFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_OakValeFan.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (Game.GetHealth(seh_BowerstoneFan.GetScriptThing()) > 0.0f) {
                seh_BowerstoneFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_BowerstoneFan.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (Game.GetHealth(seh_HookCoastFan.GetScriptThing()) > 0.0f) {
                seh_HookCoastFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_HookCoastFan.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            ParentClass.FansCanLeaveNow = true;
            Game.EntitySetAsKillable(OakValeFan, true, true);
            Game.EntitySetAsKillable(HookCoastFan, true, true);
            Game.EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_OakValeFan.IsValid())
                seh_OakValeFan.Release();
            if (seh_HookCoastFan.IsValid())
                seh_HookCoastFan.Release();
            if (seh_BowerstoneFan.IsValid())
                seh_BowerstoneFan.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            switch (ChatLevel) {
            case 0:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIRST", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 1;
                Game.AddItemToContainer(OakValeFan, "OBJECT_HERO_HELMET_CHAINMAIL");
                break;
            }
            case 1:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SECOND", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 2;
                break;
            }
            case 2:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_THIRD", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 3;
                break;
            }
            case 3:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FOURTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 4;
                break;
            }
            case 4:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIFTH", false, OakValeFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 5;
                break;
            }
            case 5:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SIXTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 6;
                break;
            }
            case 6:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SEVENTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 7;
                ParentClass.ArenaOver = true;
                Game.EntitySetAsKillable(OakValeFan, true, true);
                Game.EntitySetAsKillable(HookCoastFan, true, true);
                Game.EntitySetAsKillable(BowerstoneFan, true, true);
                break;
            }
            }
        }
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    while (true) {
        Game.NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}

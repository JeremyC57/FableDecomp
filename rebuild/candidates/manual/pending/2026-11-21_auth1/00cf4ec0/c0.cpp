// NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main() -- the arena holding-room fan marker: drives the three fans (Oakvale, Bowerstone, Hook Coast). Until the fans may leave or Q_Arena is completed it plays the fans' hit lines when the hero attacks them (a paused movie the first time, a conversation if a hit scene is already running), after which they may leave and are killable; otherwise, with the hero close and the chat timer run down, the fans trade the next early chat line. Then, until they may leave or the arena is over, it plays the same hit scene or the next late chat line (the first gives the Oakvale fan the chainmail helmet, the seventh ends the arena), and finally idles
// Grade: DIFFER(6244v6164) -- SDIFF 175 (231 lines): the VC7.1 function-wide inline budget: retail expands ~CIsThingAlive around a called ~CScriptThing 0x004AA840 (CIsThingAlive vtable cached in edi) and expands ~CScriptThing only at the three loop-1 GetScriptThing temporaries; the row-local inline ~CScriptThing those need also expands inside the forced ~CIsThingAlive (a3, plain inline: ~CIsThingAlive called, 268 lines); retail expands the implicit movie dtor at all 4 sites, ours calls the COMDAT 0x006E7B80; the loop-1 movie exit pad and the end-of-function exit pads are laid out differently
// Next idea: the nested-inline lane gap (chunk 808 00de6c20, chunk 810 00d62480): ~CScriptThing out of line inside the inlined ~CIsThingAlive but expanded at the loop-1 temporaries; then the movie-dtor budget
// Leaf: ?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF4EC0, 6164 bytes (incl. two jump tables), thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ;
//   one .rdata reference, slot 1 of the entity vtable 0x012C657C stored by CFanMarker::Alloc 0x00CF4DF0 (2 CEntityScriptBase::Init
//   0x00CDEBB0, GetParentScript 0x00CF4E90).
// Layout: engine/CQ_ArenaHoldingScriptScript.h with the chunk 812 FANMARKER opt-in (hdr/README.md).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
__forceinline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::~CScriptThing() { }
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF

void NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing OakvaleFan = Game->GetThingWithScriptName("OakvaleFan");
    CScriptThing BowerstoneFan = Game->GetThingWithScriptName("BowerstoneFan");
    CScriptThing HookCoastFan = Game->GetThingWithScriptName("HookCoastFan");
    long ChatCount = 0;
    Game->EntitySetAsKillable(OakvaleFan, false, true);
    Game->EntitySetAsKillable(HookCoastFan, false, true);
    Game->EntitySetAsKillable(BowerstoneFan, false, true);
    CScriptGameResourceObjectScriptedThingBase seh_oakvale;
    CScriptGameResourceObjectScriptedThingBase seh_bowerstone;
    CScriptGameResourceObjectScriptedThingBase seh_hookcoast;
    CTimer ChatTimer;
    ChatTimer = 0;
    while (!ParentClass.FansCanLeaveNow && !Game->IsQuestCompleted("Q_Arena")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.PlayerAttackedFans) {
            if (IsActiveThreadTerminating())
                return;
            Game->StartScriptingEntity(OakvaleFan, seh_oakvale, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(BowerstoneFan, seh_bowerstone, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(HookCoastFan, seh_hookcoast, SCRIPT_AI_PRIORITY_HIGHEST);
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass.InHitCutsceneAlready = true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_oakvale.GetScriptThing()) > 0.0f) {
                    seh_oakvale.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_oakvale.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (Game->GetHealth(seh_bowerstone.GetScriptThing()) > 0.0f) {
                    seh_bowerstone.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_bowerstone.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (Game->GetHealth(seh_hookcoast.GetScriptThing()) > 0.0f) {
                    seh_hookcoast.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_hookcoast.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                ParentClass.InHitCutsceneAlready = false;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", false, OakvaleFan, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", false, HookCoastFan, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", false, BowerstoneFan, Game->GetHero());
            }
            ParentClass.FansCanLeaveNow = true;
            Game->EntitySetAsKillable(OakvaleFan, true, true);
            Game->EntitySetAsKillable(HookCoastFan, true, true);
            Game->EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_oakvale.IsValid())
                seh_oakvale.Release();
            if (seh_hookcoast.IsValid())
                seh_hookcoast.Release();
            if (seh_bowerstone.IsValid())
                seh_bowerstone.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            switch (ChatCount) {
            case 0:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, OakvaleFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIRST", false, BowerstoneFan, OakvaleFan);
                ChatTimer = 3;
                ChatCount = 1;
                break;
            }
            case 1:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SECOND", false, OakvaleFan, BowerstoneFan);
                ChatTimer = 3;
                ChatCount = 2;
                break;
            }
            case 2:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, OakvaleFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_THIRD", false, HookCoastFan, OakvaleFan);
                ChatTimer = 3;
                ChatCount = 3;
                break;
            }
            case 3:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, OakvaleFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FOURTH", false, HookCoastFan, OakvaleFan);
                ChatTimer = 3;
                ChatCount = 4;
                break;
            }
            case 4:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIFTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatCount = 5;
                break;
            }
            case 5:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SIXTH", false, OakvaleFan, HookCoastFan);
                ChatTimer = 3;
                ChatCount = 6;
                break;
            }
            case 6:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, OakvaleFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SEVENTH", false, BowerstoneFan, OakvaleFan);
                ChatCount = 7;
                ChatTimer = 3;
                break;
            }
            case 7:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_EIGTH", false, OakvaleFan, BowerstoneFan);
                ChatCount = 8;
                ChatTimer = 3;
                break;
            }
            case 8:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, OakvaleFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_NINTH", false, BowerstoneFan, OakvaleFan);
                ChatCount = 9;
                ChatTimer = 3;
                ParentClass.OakvaleInsult = true;
                break;
            }
            case 9:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TENTH", false, OakvaleFan, BowerstoneFan);
                ChatCount = 10;
                ChatTimer = 3;
                break;
            }
            case 10:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, OakvaleFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_ELEVENTH", false, BowerstoneFan, OakvaleFan);
                ChatCount = 11;
                ChatTimer = 3;
                break;
            }
            case 11:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TWELFTH", false, OakvaleFan, BowerstoneFan);
                ChatCount = 12;
                ChatTimer = 3;
                break;
            }
            case 12:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, OakvaleFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_OAKVALE_INSULT", false, BowerstoneFan, OakvaleFan);
                ChatCount = 13;
                ChatTimer = 3;
                break;
            }
            case 13:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_BOWERSTONE_INSULT", false, OakvaleFan, BowerstoneFan);
                ChatTimer = 3;
                ChatCount = 12;
                break;
            }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    ChatCount = 0;
    while (!ParentClass.FansCanLeaveNow && !ParentClass.ArenaOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.PlayerAttackedFans) {
            if (IsActiveThreadTerminating())
                return;
            Game->StartScriptingEntity(OakvaleFan, seh_oakvale, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(BowerstoneFan, seh_bowerstone, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(HookCoastFan, seh_hookcoast, SCRIPT_AI_PRIORITY_HIGHEST);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_oakvale.GetScriptThing()) > 0.0f) {
                seh_oakvale.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_oakvale.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (Game->GetHealth(seh_bowerstone.GetScriptThing()) > 0.0f) {
                seh_bowerstone.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_bowerstone.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (Game->GetHealth(seh_hookcoast.GetScriptThing()) > 0.0f) {
                seh_hookcoast.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_hookcoast.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            ParentClass.FansCanLeaveNow = true;
            Game->EntitySetAsKillable(OakvaleFan, true, true);
            Game->EntitySetAsKillable(HookCoastFan, true, true);
            Game->EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_oakvale.IsValid())
                seh_oakvale.Release();
            if (seh_hookcoast.IsValid())
                seh_hookcoast.Release();
            if (seh_bowerstone.IsValid())
                seh_bowerstone.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            switch (ChatCount) {
            case 0:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIRST", false, OakvaleFan, BowerstoneFan);
                ChatTimer = 3;
                ChatCount = 1;
                Game->AddItemToContainer(OakvaleFan, "OBJECT_HERO_HELMET_CHAINMAIL");
                break;
            }
            case 1:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SECOND", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatCount = 2;
                break;
            }
            case 2:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_THIRD", false, OakvaleFan, BowerstoneFan);
                ChatTimer = 3;
                ChatCount = 3;
                break;
            }
            case 3:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FOURTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatCount = 4;
                break;
            }
            case 4:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(OakvaleFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIFTH", false, OakvaleFan, HookCoastFan);
                ChatTimer = 3;
                ChatCount = 5;
                break;
            }
            case 5:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SIXTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatCount = 6;
                break;
            }
            case 6:
            {
                long HLStyleConversationIndex = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SEVENTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatCount = 7;
                ParentClass.ArenaOver = true;
                Game->EntitySetAsKillable(OakvaleFan, true, true);
                Game->EntitySetAsKillable(HookCoastFan, true, true);
                Game->EntitySetAsKillable(BowerstoneFan, true, true);
                break;
            }
            }
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}

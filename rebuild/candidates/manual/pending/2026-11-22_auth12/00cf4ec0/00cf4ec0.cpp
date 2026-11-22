// NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main() -- the arena holding-room fan marker: drives the three fans (Oakvale, Bowerstone, Hook Coast). Until the fans may leave or Q_Arena is completed it plays the fans' hit lines when the hero attacks them (a paused movie the first time, a conversation if a hit scene is already running), after which they may leave and are killable; otherwise, with the hero close and the chat timer run down, the fans trade the next early chat line. Then, until they may leave or the arena is over, it plays the same hit scene or the next late chat line (the first gives the Oakvale fan the chainmail helmet, the seventh ends the arena), and finally idles
// Grade: DIFFER(6180v6164) (AUTH12 p6d, stock VC7.1, no flags; mdiff 102) -- the only residue is an inline decision: the implicit ~CScriptGameResourceObjectMovieBase is a COMDAT call 0x006E7B80 at the loop-2 hit-scene sites (c2 scores 74..798 there) where retail expands it, so the loop-2 exit pads and frame slots differ. ~CTimer is scored and reproduces retail's per-site split exactly (expanded at the loop-1 returns and the loop-2 Oakvale wait, called 0x00CD4470 elsewhere). /d2inlS15 (the fan siblings' flag) expands the movie dtor but also always-inlines ~CTimer (both pass the S test at 15, neither at 14): DIFFER(6088v6164), mdiff 270. See ESCALATE.md.
// Inline-budget: scoped `#pragma inline_depth(0)` region round the plain-inline CIsThingAlive dtor/ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (the landed 0x00DE6C20 device): the predicate is expanded at the SetPredicate temporary with its CScriptThing copy 0x004ABE90 and ~CScriptThing 0x004AA840 called inside, no CPredicateBase vtable store, as retail; CTimer operator= / GetTime forced by the overlay opt-in FABLE_RETAIL_CTIMER_FORCEINLINE_SET_GET (no ??4CTimer / GetTime in retail).
// Ego_d (line sizes, odall10 VC10 /Od 11912 v Ego_d 11940; locals) used: TRetVoid _Fail_Val; OakValeFan / seh_*Fan / ChatLevel / Conversation names; ChatLevel++ (9 B lines); the generated ending `if (T) return; while (!false) {...} if (T) return;` (962:118, 963:278). Ego_d MISFIT still open: each fan hit block is 5 B larger in Ego_d (541 v 536 loop 1, 561 v 556 loop 2); negatives: `> 0`, `== true`, `!(<= 0.0f)`, `> 0.0`, `0.0f <`, `!= false` (ESCALATE.md).
// Leaf: ?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF4EC0, 6164 bytes (incl. two jump tables), thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ;
//   one .rdata reference, slot 1 of the entity vtable 0x012C657C stored by CFanMarker::Alloc 0x00CF4DF0 (2 CEntityScriptBase::Init
//   0x00CDEBB0, GetParentScript 0x00CF4E90).
// Layout: engine/CQ_ArenaHoldingScriptScript.h with the chunk 812 FANMARKER opt-in (hdr/README.md).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTIMER_FORCEINLINE_SET_GET
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
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CScriptThing::~CScriptThing() { }
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }

void NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptThing OakValeFan = Game->GetThingWithScriptName("OakvaleFan");
    CScriptThing BowerstoneFan = Game->GetThingWithScriptName("BowerstoneFan");
    CScriptThing HookCoastFan = Game->GetThingWithScriptName("HookCoastFan");
    long ChatLevel = 0;
    Game->EntitySetAsKillable(OakValeFan, false, true);
    Game->EntitySetAsKillable(HookCoastFan, false, true);
    Game->EntitySetAsKillable(BowerstoneFan, false, true);
    CScriptGameResourceObjectScriptedThingBase seh_OakValeFan;
    CScriptGameResourceObjectScriptedThingBase seh_BowerstoneFan;
    CScriptGameResourceObjectScriptedThingBase seh_HookCoastFan;
    CTimer ChatTimer;
    ChatTimer = 0;
    while (!ParentClass.FansCanLeaveNow && !Game->IsQuestCompleted("Q_Arena")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.PlayerAttackedFans) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->StartScriptingEntity(OakValeFan, seh_OakValeFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(BowerstoneFan, seh_BowerstoneFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(HookCoastFan, seh_HookCoastFan, SCRIPT_AI_PRIORITY_HIGHEST);
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready = true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_OakValeFan.GetScriptThing()) > 0.0f) {
                    seh_OakValeFan.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_OakValeFan.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (Game->GetHealth(seh_BowerstoneFan.GetScriptThing()) > 0.0f) {
                    seh_BowerstoneFan.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_BowerstoneFan.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (Game->GetHealth(seh_HookCoastFan.GetScriptThing()) > 0.0f) {
                    seh_HookCoastFan.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_HookCoastFan.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
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
                long HLStyleConversationIndex = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game->AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", false, OakValeFan, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", false, HookCoastFan, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", false, BowerstoneFan, Game->GetHero());
            }
            ParentClass.FansCanLeaveNow = true;
            Game->EntitySetAsKillable(OakValeFan, true, true);
            Game->EntitySetAsKillable(HookCoastFan, true, true);
            Game->EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_OakValeFan.IsValid())
                seh_OakValeFan.Release();
            if (seh_HookCoastFan.IsValid())
                seh_HookCoastFan.Release();
            if (seh_BowerstoneFan.IsValid())
                seh_BowerstoneFan.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Conversation;
            switch (ChatLevel) {
            case 0:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, OakValeFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIRST", false, BowerstoneFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 1:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SECOND", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 2:
            {
                Conversation = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(Conversation, OakValeFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_THIRD", false, HookCoastFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 3:
            {
                Conversation = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(Conversation, OakValeFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FOURTH", false, HookCoastFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 4:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, HookCoastFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIFTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 5:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, HookCoastFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SIXTH", false, OakValeFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 6:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, OakValeFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SEVENTH", false, BowerstoneFan, OakValeFan);
                ChatLevel++;
                ChatTimer = 3;
                break;
            }
            case 7:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_EIGTH", false, OakValeFan, BowerstoneFan);
                ChatLevel++;
                ChatTimer = 3;
                break;
            }
            case 8:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, OakValeFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_NINTH", false, BowerstoneFan, OakValeFan);
                ChatLevel++;
                ChatTimer = 3;
                ParentClass.OakvaleInsult = true;
                break;
            }
            case 9:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TENTH", false, OakValeFan, BowerstoneFan);
                ChatLevel++;
                ChatTimer = 3;
                break;
            }
            case 10:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, OakValeFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_ELEVENTH", false, BowerstoneFan, OakValeFan);
                ChatLevel++;
                ChatTimer = 3;
                break;
            }
            case 11:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TWELFTH", false, OakValeFan, BowerstoneFan);
                ChatLevel++;
                ChatTimer = 3;
                break;
            }
            case 12:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, OakValeFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_OAKVALE_INSULT", false, BowerstoneFan, OakValeFan);
                ChatLevel++;
                ChatTimer = 3;
                break;
            }
            case 13:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_EARLY_CHAT_BOWERSTONE_INSULT", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 12;
                break;
            }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    ChatLevel = 0;
    while (!ParentClass.FansCanLeaveNow && !ParentClass.ArenaOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.PlayerAttackedFans) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->StartScriptingEntity(OakValeFan, seh_OakValeFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(BowerstoneFan, seh_BowerstoneFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(HookCoastFan, seh_HookCoastFan, SCRIPT_AI_PRIORITY_HIGHEST);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_OakValeFan.GetScriptThing()) > 0.0f) {
                seh_OakValeFan.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_OakValeFan.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (Game->GetHealth(seh_BowerstoneFan.GetScriptThing()) > 0.0f) {
                seh_BowerstoneFan.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_BowerstoneFan.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (Game->GetHealth(seh_HookCoastFan.GetScriptThing()) > 0.0f) {
                seh_HookCoastFan.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_HookCoastFan.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            ParentClass.FansCanLeaveNow = true;
            Game->EntitySetAsKillable(OakValeFan, true, true);
            Game->EntitySetAsKillable(HookCoastFan, true, true);
            Game->EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_OakValeFan.IsValid())
                seh_OakValeFan.Release();
            if (seh_HookCoastFan.IsValid())
                seh_HookCoastFan.Release();
            if (seh_BowerstoneFan.IsValid())
                seh_BowerstoneFan.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Conversation;
            switch (ChatLevel) {
            case 0:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIRST", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel++;
                Game->AddItemToContainer(OakValeFan, "OBJECT_HERO_HELMET_CHAINMAIL");
                break;
            }
            case 1:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, HookCoastFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_LATE_CHAT_SECOND", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 2:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_LATE_CHAT_THIRD", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 3:
            {
                Conversation = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_LATE_CHAT_FOURTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 4:
            {
                Conversation = Game->AddNewConversation(OakValeFan, false, false);
                Game->AddPersonToConversation(Conversation, HookCoastFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIFTH", false, OakValeFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 5:
            {
                Conversation = Game->AddNewConversation(HookCoastFan, false, false);
                Game->AddPersonToConversation(Conversation, BowerstoneFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_LATE_CHAT_SIXTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel++;
                break;
            }
            case 6:
            {
                Conversation = Game->AddNewConversation(BowerstoneFan, false, false);
                Game->AddPersonToConversation(Conversation, HookCoastFan);
                Game->AddLineToConversation(Conversation, "TEXT_QST_005_V2_FAN_LATE_CHAT_SEVENTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel++;
                ParentClass.ArenaOver = true;
                Game->EntitySetAsKillable(OakValeFan, true, true);
                Game->EntitySetAsKillable(HookCoastFan, true, true);
                Game->EntitySetAsKillable(BowerstoneFan, true, true);
                break;
            }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}

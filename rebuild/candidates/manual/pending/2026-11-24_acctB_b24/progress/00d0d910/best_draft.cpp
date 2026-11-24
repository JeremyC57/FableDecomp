// NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main() -- second camp gate guard
// Grade: DIFFER(4672v4752) -- Frame0x10c and9movie cleanup sites restored;2native hot inlineTimer cleanup blocks absent under diagnostic noinline destructor. Original timerbody+S15 expands all sites; S10..14 preserves timer split but calls movies. Narrow per-callee expansion split unresolved.
// Flags: /d2inlS15
// Leaf: ?Main@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: 0x00D0D910, 4752 bytes, thiscall.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ;
//   one .rdata reference, slot 1 of the entity vtable 0x012C8E80 stored by the landed CGate2Guard1::Alloc 0x00D071F0.
// Callees: OpenGate 0x00D0EBC0 through CSpawnedFunc2<CQ_BanditCampBossBattleScript,float,CCharString> (vtable 0x012C9918, SuspendableProcess 0x00D0EBA0).
// Layout: engine/CQ_BanditCampBossBattleScript.h with the chunk 812 GATE2GUARD1 opt-in (hdr/README.md).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GATE2GUARD1
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EHeroAbility.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#include "engine/fable_script_distance.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;                  // 0x0143E90C
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#include "engine/CSpawnedFunc2.h"
#include "engine/TRetVoid.h"

void NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.EntitySetAsKillable(Me, false, true);
    CTimer MoanTimer;
    MoanTimer = 0;
    bool TalkedTo = false;
    long HitNumber = 0;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.Gate2Open) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                switch (HitNumber) {
                case 0:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_FIRST", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    ++HitNumber;
                    break;
                case 1:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_SECOND", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    break;
                case 2:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_THIRD", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    break;
                case 3:
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_HIT_FOURTH", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Me.SetFriendsWithEverythingFlag(false);
                    Game.ModifyThingHealth(Me, 10000.0f, false);
                    Game.EntitySetAsKillable(Me, true, true);
                    Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                    Game.ClearThingHasInformation(Me);
                    break;
                }
            }
            if (MoanTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game.GetHero(), Me, 8.0f)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    TalkedTo = true;
                    Game.AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT2_COMMENT_FIRST", false, Me, Game.GetHero());
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT2_COMMENT_SECOND", false, Me, Game.GetHero());
                }
                MoanTimer = 10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.SpokenToSecondGuard = true;
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (!Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game.GetHero())) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_GATE_BLOCKED", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_MEDIUM);
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                    ParentClass.Gate2Open = true;
                    ParentClass.AddSpawnedFunction(new CSpawnedFunc2<CQ_BanditCampBossBattleScript, float, CCharString>(
                        CCharString("ParentClass.", -1) + CCharString("OpenGate", -1), 0, &ParentClass,
                        &CQ_BanditCampBossBattleScript::OpenGate, 2.0f, "Gate2Outer"), CCharString(""));
                    long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_009_BANDIT2_PASS", false, Me, Game.GetHero());
                    Game.ClearThingHasInformation(Me);
                }
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_ASIDE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_009_BANDIT2_ATTACKED_NEW", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                }
                Me.SetFriendsWithEverythingFlag(false);
                Game.EntitySetAsKillable(Me, true, true);
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                Game.ClearThingHasInformation(Me);
                if (seh_me.IsValid())
                    seh_me.Release();
                while (true) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

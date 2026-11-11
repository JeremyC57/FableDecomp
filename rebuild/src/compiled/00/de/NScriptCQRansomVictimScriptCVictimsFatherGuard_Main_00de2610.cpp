// NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Main() -- the chief's guard: alive-predicate, take him under the parent's seh_Guard, and until extra assistance is given answer the hero's talk (TEXT_QST_B06_CHIEFSGUARD_PROTECTING_CHIEF), count the hero's hits (ON_HIT_10/20/30; the third turns him hostile, otherwise he stays the hero's ally) and turn hostile when the chief was hit three times; then follow the hero (minimap orb, health bar, hero faction) and keep answering talk (waiting outside / guards killed / on talk), hits (as above, under scripted control) and the wait-outside / rescued follow states
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: scoped predicate members retain the nested CScriptThing calls at 0x00DE2630 and 0x00DE2649; prior hand799/835 exhausted plain forms and row budgets.
// Leaf: ?Main@CVictimsFatherGuard@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: 0x00DE2610, 4420 bytes, thiscall.
// Identity (proven): slot 1 (Main) of the CVictimsFatherGuard entity vtable stored by the landed Alloc 0x00DE2500 (Init 0x00DE25D0 /
//   GetParentScript 0x00DE25A0 landed); Ego_r.pdb ?Main@CVictimsFatherGuard@CQ_RansomVictimScript@NScript@@EAEXXZ; TimesHit +0x1C is the PDB member;
//   every literal is a B06 chief's-guard line (TEXT_QST_B06_CHIEFSGUARD_*).
// Callees: CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CScriptThing copy 0x004ABE90 / ~ 0x004AA840,
//   ~CBaseClass 0x0099A2E0, handle IsValid 0x00CD23B9 / Release 0x00CD2770 (the parent's seh_Guard +0x6C: GetScriptThing slot 12, Speak(const char*)
//   slot 13, IsPerformingScriptTask slot 26 through its vtable), CCharString 0x0099EBF0 / default 0x0099E4B0 / operator=(const char*) 0x0099EFE0 /
//   operator const char* 0x0099E4C0 / ~ 0x0099EAE0, movie CBaseObject 0x0099A380 / ~CBaseObject 0x0099A430 / ~CCountedPointer<movie> 0x006E7AB0 /
//   ctor 0x006E7B60 / dtor 0x006E7B80 (cold sites), operator delete 0x00BFE9BC; the shared 0.0f 0x0122DEDC; CScriptThing slots 21 MsgIsHitBy,
//   27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom, 75 IsAlive, 76 IsDead.
// Layout: engine/CQ_RansomVictimScript.h (repo, unchanged): parent RescuedVictim +0x4B, RansomHelperStarted +0x4D, RansomHelperSpoken +0x4E,
//   ExtraAssistanceGiven +0x4F, ToldGuardToWaitOutside +0x51, HitChief +0x54, seh_Guard +0x6C.
// Shape: generated entity-script code (the landed sibling CVictimsBrotherHelper::Main 0x00DEB280 TU). IsValid / Release go through a local
//   parent reference (the 0x00E776F0 precedent: `mov eax,[esi+14h]; lea ebp,[eax+6Ch]`); the hit line is a switch with `case 3` and `default`
//   both ON_HIT_30 (VC7.1 merges them and keeps the dead `dec eax`); SetEntityAsRegionFollowing takes (hero, Me, flag).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/ECutsceneBehaviour.h"
inline CScriptThing::~CScriptThing() { }
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#include "engine/CQ_SunnyvaleMasterData.h"

void NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
    while (!Game.StartScriptingEntity(Me, ParentClass.seh_Guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!ParentClass.ExtraAssistanceGiven) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Guard.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Guard.Speak(Game.GetHero(), "TEXT_QST_B06_CHIEFSGUARD_PROTECTING_CHIEF", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (ParentClass.seh_Guard.IsPerformingScriptTask()) {
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
            ++TimesHit;
            CCharString line;
            switch (TimesHit) {
            case 1: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_10"; break;
            case 2: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_20"; break;
            case 3: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            default: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            }
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(ParentClass.seh_Guard.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Guard.Speak(Game.GetHero(), line, GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Guard.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (TimesHit >= 3) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.EntityUnsetThingAsAllyOfThing(Me, Game.GetHero());
                Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(), Me);
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
                for (;;) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
        if (ParentClass.HitChief > 2) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(conversation, Game.GetHero());
            Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_HIT_CHIEF_THIRD_TIME_10", false, Me, Game.GetHero());
            Game.EntityUnsetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(), Me);
            Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    Game.DisplayQuestInfo(true);
    Game.AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_GUARD", 1.0f);
    Game.EntityFollowThing(Me, Game.GetHero(), 6.0f, true);
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
    Game.EntitySetInFaction(Me, "FACTION_HERO");
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
    Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
    long FollowState = 0;
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (;;) {
        if (ParentClass.RansomHelperStarted && !ParentClass.RansomHelperSpoken) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            while (!ParentClass.RansomHelperSpoken) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(conversation, Game.GetHero());
            if (ParentClass.ToldGuardToWaitOutside && !ParentClass.RescuedVictim && Game.GetNearestWithScriptName(Me, "RansomHelper").IsAlive()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_WAITIING_OUTSIDE_10", false, Me, Game.GetHero());
            } else if (ParentClass.ToldGuardToWaitOutside && !ParentClass.RescuedVictim && Game.GetNearestWithScriptName(Me, "RansomHelper").IsDead()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_WAITING_OUTSIDE_KILLED_GUARDS_10", false, Me, Game.GetHero());
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_ON_TALK_10", false, Me, Game.GetHero());
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            ++TimesHit;
            CCharString line;
            switch (TimesHit) {
            case 1: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_10"; break;
            case 2: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_20"; break;
            case 3: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            default: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            }
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(ParentClass.seh_Guard.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Guard.Speak(Game.GetHero(), line, GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Guard.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
                Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
            }
            if (TimesHit >= 3) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.EntityUnsetThingAsAllyOfThing(Me, Game.GetHero());
                Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(), Me);
                Game.EntityStopFollowing(Me);
                Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
                for (;;) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
        if (FollowState == 0 && ParentClass.ToldGuardToWaitOutside && !ParentClass.RescuedVictim) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntityStopFollowing(Me);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
            Game.EntitySetAsRespondingToFollowAndWaitExpressions(Me, false);
            FollowState = 1;
        }
        if (FollowState == 1 && ParentClass.RescuedVictim) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntityFollowThing(Me, Game.GetHero(), 1.0f, true);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
            Game.EntitySetAsRespondingToFollowAndWaitExpressions(Me, true);
            FollowState = 2;
        }
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
}
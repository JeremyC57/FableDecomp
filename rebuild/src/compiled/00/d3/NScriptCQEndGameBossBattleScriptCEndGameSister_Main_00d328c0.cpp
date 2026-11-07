// NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main() -- the hero's sister in the final boss battle: live while alive; wait for the intro, take scripted control, make her untargetable, undamageable, unpushable, unkillable, not epic-spellable, unconscious (FALL_UNCONSCIOUS), not force-pushable and evading every hit; once the final decision starts make her targetable / damageable again (+10000 health), take the hero, fade out, wake her, clear her actions, play CS_JACK_DEATH with her held at MK_JACK1_HOLDJACK, then move her to MK_END1_SISTER_STOP, remove CS_AEONS and give the hero the Sword of Aeons; until the decision is made: if the hero hits her with the sword play CS_JACK_BOSS_ENDING_EVIL (JackBossBattleResult 1), or give her up while a cutscene needs her and take her back after; then idle
// Leaf: ?Main@CEndGameSister@CQ_EndGameBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D328C0, 2484 bytes, thiscall, plain `ret` (three epilogue copies), frame 0x6c.
// Identity (proven): Ego_r.pdb ?Main@CEndGameSister@CQ_EndGameBossBattleScript@NScript@@EAEXXZ; slot 1 of the CEndGameSister entity
//   vtable stored by the landed Alloc 0x00D2FC00.
// Shape: the class model is the new engine/CQ_EndGameBossBattleScript.h (opt-in FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CENDGAMESISTER);
//   MOVIEBASE_FORCEINLINE_CTOR + _OUTOFLINE_DTOR (both movie ctors expanded, both dtors call 0x006E7B80), STB OUTOFLINE_DTOR / _ASSIGN
//   (seh_me / seh_Hero dtors and the map value assignments are calls), the declared-only map<CCharString, STB> ctor / dtor; the final
//   idle loop in the chunk 817 else-wait form `if (T) return; for (;;) { NSF; if (T) return; }`, which VC7.1 tail-merges into the
//   loop test (`jmp test`) with the seh_Hero return pad before the loop, as retail.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CENDGAMESISTER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_EndGameBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/fable_script_cutscene.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.IntroFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
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
    Game->EntitySetTargetable(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->SetIsThingEpicSpellable(Me, false);
    Game->SetThingAsConscious(Me, false, "FALL_UNCONSCIOUS");
    Game->SetIsThingForcePushable(Me, false);
    Game->EntitySetEvadeAllHits(Me, true);
    while (!ParentClass.FinalDecision) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetTargetable(Me, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->EntitySetEvadeAllHits(Me, false);
    Game->ModifyThingHealth(Me, 10000.0f, false);
    Game->SetIsThingEpicSpellable(Me, true);
    {
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid())
            seh_Hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["Hero"] = seh_Hero;
        Game->FadeScreenOut(1.3f, 0.5f, CRGBColour(0, 0, 0, 255));
        Game->Pause(0.5f);
        Game->SetThingAsConscious(Me, true, "");
        seh_me.ClearAllActionsIncludingLoopingAnimations();
        seh_me.ClearAllActions();
        seh_me.ClearCommands();
        Game->EntityTeleportToThing(Game->GetThingWithScriptName("EndGameSister"), Game->GetThingWithScriptName("MK_JACK1_HOLDJACK"), false);
        Game->EntitySetAsDrawable(Game->GetThingWithScriptName("EndGameSister"), false);
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_JACK_DEATH", mymap, 0, 0, false, true);
        Game->EntityTeleportToThing(Game->GetThingWithScriptName("EndGameSister"), Game->GetThingWithScriptName("MK_END1_SISTER_STOP"), false);
        Game->EntitySetAsDrawable(Game->GetThingWithScriptName("EndGameSister"), true);
        Game->RemoveThing(Game->GetThingWithScriptName("CS_AEONS"), false, true);
        Game->GiveHeroWeapon("OBJECT_SWORD_OF_AEONS", false);
        Game->SetWeaponAsHerosActiveWeapon("OBJECT_SWORD_OF_AEONS");
    }
    while (!ParentClass.FinalDecisionMade) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO", "OBJECT_SWORD_OF_AEONS")) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass.FinalDecisionMade = true;
            PMasterData->JackBossBattleResult = 1;
            CScriptGameResourceObjectScriptedThingBase seh_Hero;
            if (seh_Hero.IsValid())
                seh_Hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["Hero"] = seh_Hero;
            mymap["Sister"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_JACK_BOSS_ENDING_EVIL", mymap, 0, 0, false, true);
            ParentClass.FinalCutscenePlayed = true;
        }
        else if (ParentClass.SisterNeededForCutscene) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (ParentClass.SisterNeededForCutscene) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
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
        }
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
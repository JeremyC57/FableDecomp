// NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main() -- bandit-camp boss battle, a crowd bandit round the arena: untargetable spectator until the king fight starts, then cheers, jeers and boos the hero by distance and the king's health until the fight ends, then joins the fight against the hero when attacked or an angry bandit is needed
// Grade: DIFFER(3802v3802) -- md=36: retail expands the by-value SetIsPushableByHero CScriptThing copy but calls 0x004ABE90 inside the CIsThingAlive temporary (the documented per-site copy-inline case), and its loop-bottom terminate test falls into the expanded ~CTimer block shared with the pre-loop test where ours jumps to the out-of-line call
// Next idea: a loop form whose bottom exit is the same CFG edge as the pre-loop exit (do/while with the terminate test as the condition) while the copy-inline split stays a documented lane gap
// Leaf: ?Main@CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CTimer.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/EHeroAbility.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
long __fastcall GFCharStringToInt(const CCharString& text);      // 0x0099E7F0
extern CGameScriptInterfaceBase* pDSTGame;
__declspec(noinline) CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
class CQ_BanditCampBossBattleScript::CCROWDBANDITS : public CEntityScriptBase {
public:
    CQ_BanditCampBossBattleScript* ParentClass;                 // +0x14
    CQ_SunnyvaleMasterData*        PMasterData;                 // +0x18
    virtual CScriptBase* GetParentScript() const;
private:
    virtual void Main();
    virtual void OnPredicateFail();
};
}

#define CB_T if (IsActiveThreadTerminating()) return
#define CB_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        CB_T; \
    } \
    CB_T
#define CB_CUTSCENE_WAIT \
    if (ParentClass->BanditsNeededForCutscene) { \
        CB_T; \
        Game->EntitySetAsUseMovementInActions(Me, true); \
        if (seh_me.IsValid()) seh_me.Release(); \
        while (ParentClass->BanditsNeededForCutscene) { \
            Game->NewScriptFrame(); \
            CB_T; \
        } \
        CB_T; \
        CB_START; \
        Game->EntitySetAsUseMovementInActions(Me, false); \
    }
#define CB_ANIM(name) seh_me.PlayAnimation(name, false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false)

void NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    CB_T;
    Game->EntitySetTargetable(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAbleToBeEngagedInCombat(Me, false);
    while (!ParentClass->BanditKingFightStarted) {
        Game->NewScriptFrame();
        CB_T;
    }
    CB_T;
    Game->EntitySetAsUseMovementInActions(Me, false);
    CScriptThing village = Game->GetNearestWithDefName(Me, "VILLAGE_BANDIT_CAMP_BOSS");
    Game->EntityAttachToVillage(Me, village);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CB_START;
    seh_me.ClearCommands();
    long kingHealth = ParentClass->KingHealth;
    CScriptThing king = Game->GetThingWithScriptName("BanditKing");
    CTimer timer;
    bool inCutscene = false;
    timer = 0;
    CB_T;
    for (;;) {
        CB_CUTSCENE_WAIT;
        if (!inCutscene && ParentClass->ItsAllOver) {
            CB_T;
            Game->EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
            inCutscene = true;
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 3.5f)) {
            CB_T;
            Game->EntitySetAttackThingImmediately(Me, Game->GetHero(), false, true);
        } else {
            if (IsDistanceBetweenThingsOver(Me, Game->GetHero(), 6.5f) && timer.GetTime() <= 0) {
                CB_T;
                if (!seh_me.IsPerformingScriptTask()) {
                    CB_T;
                    if (ParentClass->KingHealth < kingHealth) {
                        CB_T;
                        kingHealth = ParentClass->KingHealth;
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
                        if (rand() % 2 == 0) {
                            CB_T;
                            switch (rand() % 3) {
                            case 0: CB_ANIM("SPECIAL_BOAST"); break;
                            case 1: CB_ANIM("ST_THREATEN"); break;
                            case 2: CB_ANIM("SCRIPT_SHOUT"); break;
                            }
                            if (rand() % 4 == 0) {
                                CB_T;
                                switch (rand() % 2) {
                                case 0: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_01"); break;
                                case 1: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_02"); break;
                                }
                            }
                        } else {
                            CB_T;
                            timer = 2;
                        }
                    } else if (IsDistanceBetweenThingsOver(Game->GetHero(), king, 8.0f)) {
                        CB_T;
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
                        if (rand() % 2 == 0) {
                            CB_T;
                            switch (rand() % 2) {
                            case 0: CB_ANIM("SPECIAL_BOAST"); break;
                            case 1: CB_ANIM("ST_WAVE_SPECIAL_01"); break;
                            }
                            if (rand() % 8 == 0) {
                                CB_T;
                                long conversation = Game->AddNewConversation(Me, false, false);
                                Game->AddPersonToConversation(conversation, Game->GetHero());
                                Game->AddLineToConversation(conversation, "TEXT_QST_009_BOSSFIGHT_TAUNTING", false, Me, Game->GetHero());
                            }
                            if (rand() % 4 == 0) {
                                CB_T;
                                switch (rand() % 2) {
                                case 0: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_01"); break;
                                case 1: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_02"); break;
                                }
                            }
                        } else {
                            CB_T;
                            timer = 2;
                        }
                    } else {
                        CB_T;
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
                        if (rand() % 2 == 0) {
                            CB_T;
                            switch (rand() % 2) {
                            case 0: CB_ANIM("COCKY"); break;
                            case 1: CB_ANIM("ST_UNPERTURBED_GESTURE"); break;
                            }
                            switch (rand() % 2) {
                            case 0: Game->PlaySoundOnThing(Me, "SND_CROWDLARGEBOASTREACT_01"); break;
                            case 1: Game->PlaySoundOnThing(Me, "SND_CROWDLARGEBOASTREACT_02"); break;
                            }
                        } else {
                            CB_T;
                            timer = 2;
                        }
                    }
                }
            }
            if (ParentClass->BanditKingFightEnded)
                break;
        }
        Game->NewScriptFrame();
        CB_T;
    }
    CB_T;
    if (GFCharStringToInt(Me.GetDataString()) == 1) {
        CB_T;
        Game->RemoveThing(Me, false, true);
    }
    Game->EntitySetTargetable(Me, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->EntitySetAsUseMovementInActions(Me, true);
    CB_T;
    for (;;) {
        CB_CUTSCENE_WAIT;
        if (!ParentClass->TwinBladeKilled) {
            CB_T;
            if (timer.GetTime() <= 0) {
                CB_T;
                if (rand() % 2 == 0) {
                    CB_T;
                    switch (rand() % 3) {
                    case 0: CB_ANIM("SPECIAL_BOAST"); break;
                    case 1: CB_ANIM("ST_THREATEN"); break;
                    case 2: CB_ANIM("SCRIPT_SHOUT"); break;
                    }
                    if (rand() % 4 == 0) {
                        CB_T;
                        switch (rand() % 2) {
                        case 0: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_01"); break;
                        case 1: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_02"); break;
                        }
                    }
                } else {
                    CB_T;
                    timer = 2;
                }
            }
        }
        if (ParentClass->AngryBanditNeeded > 0 || Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            break;
        Game->NewScriptFrame();
        CB_T;
    }
    CB_T;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAbleToBeEngagedInCombat(Me, true);
    if (seh_me.IsValid()) seh_me.Release();
    if (ParentClass->AngryBanditNeeded != 0) {
        CB_T;
        --ParentClass->AngryBanditNeeded;
    }
    while (true) {
        Game->NewScriptFrame();
        CB_T;
    }
}

// NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing npc) -- the fisherman's attitude thread: with an is-alive predicate on the NPC, wait until the hero hits him (or hits him with Heal Life) while he still has more than 1 health, then mark the relation counter, make the fisherman run away, take him under script control and play the "hate" cutscene (stand points, camera, speech, walk to MK_FI_HERO, fade out, remove the NPC, fade in)
// Grade: DIFFER(1624v1623) -- the two prelude terminate checks (0x00E7C054 / 0x00E7C063) and the hit-loop bottom check (0x00E7C170) share ONE inline ~npc exit block in retail (0x00E7C17F); VC7.1 gives the first check its own inline copy, sends the second to the out-of-line tail and orders the hit-test / exit blocks differently (every REST exit, the handle / movie / pause tails and the cold tail match; variant e, probe/e7c010_e.cpp, keeps retail's plain flow at 1636 bytes)
// Next idea: a spelling whose three exits are one IL block: the hit wait as a rotated `while (!IsActiveThreadTerminating())` loop whose condition exit returns while the hit path breaks, with the two prelude checks folded into its head (`||`), or the SetPredicate / NewScriptFrame prelude generated inside the same loop
// Leaf: ?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7C010, 1623 bytes, thiscall, `ret 0Ch` (the by-value CScriptThing), no EH frame.
// Identity (proven): FableWin.pdb ?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z; spawned by the landed
//   CFisherman::Init 0x00E7BD60 under "ParentClass." + "NPCAttitudeProcess" through CSpawnedFunc1<CV_FishermanScript,
//   CScriptThing> (its SuspendableProcess 0x00E7C670 is the next row); literals "TEXT_QST_023_IDLE_03_HATE" 0x012EA1B0,
//   "MK_FI_HERO" 0x012EA1A4, "NPC_NPCStand" 0x012EA1E8, "NPC_HeroStand" 0x012EA1D8, "NPC_Camera" 0x012EA1CC.
// Callees: CScriptThing copy 0x004ABE90 (the predicate's Thing) / ~CScriptThing 0x004AA840 / GetAngleXY 0x004AAA40 / IsAlive
//   0x004AB130 (devirtualised on locals), CScriptBase::SetPredicate 0x00CB7920, ~CIsThingAlive 0x00E241B0,
//   IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseObject() 0x0099A380 / ~CBaseClass
//   0x0099A2E0 / operator delete 0x00BFE9BC (the handle ctor and the expanded dtors), IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   ~ScriptedThingBase 0x007E74D0, GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   MoveToThing 0x007E7300 (direct calls on the handle local), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); CScriptThing implementation slots 21 MsgIsHitBy, 42
//   MsgIsHitByAnySpecialAbilityFrom, 41 MsgIsHitBySpecialAbilityFrom behind the null-safe wrappers; CGameScriptInterfaceBase
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName, 77 GetNearestWithScriptName,
//   108 RemoveThing, 130 SheatheHeroWeapons, 264 GetHealth, 370 StartMovieSequence, 373 FadeScreenOut, 374 FadeScreenIn,
//   376 Pause, 379 PauseAllNonScriptedEntities (CWideScreenMagicPauseEntities), 411 CameraUseCameraPoint, 416 CameraDefault,
//   472 EntityTeleportToThing, 474 EntitySetFacingAngle, 475 EntitySetFacingAngleTowardsThing.
// Data: CQ_SunnyvaleMasterData::RelationCounters[2] 0x0143E930 (hdr overlay opt-in); float pool 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout (retail = Ego_r.pdb - 0x14, as the landed CFisherman::Init): FishermanMustRunAway +0x50.
// Shape: the landed CV_ChapelOfEvilScript::WatchForAcolyteHit 0x00E6AF20 predicate/wrapper model, the CV_HeroDuelScript
//   cutscene model (handle `IsValid/Release` + StartScriptingEntity wait, MovieBase + RAII CWideScreenMagicPauseEntities) with
//   the stand-point handles in an inner block so FadeScreenIn runs after the movie and pause are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnySpecialAbilityFrom(attacker); }
inline bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBySpecialAbilityFrom(ability, attacker); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool IntroPlayed;                                   // +0x48
    bool HeroHasSavedFishermanFromWasps;                // +0x49
    long WaspsAlive;                                    // +0x4c
    bool FishermanMustRunAway;                          // +0x50
    bool QuestCardGiven;                                // +0x51
    long FishermanTasks;                                // +0x54
    void NPCAttitudeProcess(CScriptThing npc);
};
}
void NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing npc)
{
    SetPredicate(CIsThingAlive(npc));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    do {
        if (npc.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (npc.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && npc.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            if (!(Game->GetHealth(npc) <= 1.0f)) {
            if (IsActiveThreadTerminating()) return;
            CQ_SunnyvaleMasterData::RelationCounters[2] = 2;
            FishermanMustRunAway = true;
            CScriptGameResourceObjectScriptedThingBase fisherman;
            if (fisherman.IsValid())
                fisherman.Release();
            while (!Game->StartScriptingEntity(npc, fisherman, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities pause(*Game);
                Game->SheatheHeroWeapons();
                CScriptThing npcStand = Game->GetNearestWithScriptName(npc, "NPC_NPCStand");
                if (IsDistanceBetweenThingsUnder(npc, npcStand, 10.0f)) {
                    if (IsActiveThreadTerminating()) return;
                    Game->EntityTeleportToThing(npc, npcStand, false);
                    Game->EntitySetFacingAngle(npc, npcStand.GetAngleXY(), true);
                    CScriptThing heroStand = Game->GetNearestWithScriptName(npc, "NPC_HeroStand");
                    if (IsDistanceBetweenThingsUnder(Game->GetHero(), heroStand, 10.0f)) {
                        if (IsActiveThreadTerminating()) return;
                        Game->EntityTeleportToThing(npc, heroStand, false);
                        Game->EntitySetFacingAngle(npc, heroStand.GetAngleXY(), true);
                    }
                    CScriptThing camera = Game->GetNearestWithScriptName(npc, "NPC_Camera");
                    if (camera.IsAlive()) {
                        if (IsActiveThreadTerminating()) return;
                        Game->CameraUseCameraPoint(camera, 0, 0, -1.0f, 0, -1);
                    }
                }
                Game->EntitySetFacingAngleTowardsThing(npc, Game->GetHero(), false);
                if (Game->GetHealth(fisherman.GetScriptThing()) > 0.0f) {
                    fisherman.Speak(Game->GetHero(), "TEXT_QST_023_IDLE_03_HATE", GROUP_SELECT_NONE, false, true, false);
                    while (fisherman.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                fisherman.MoveToThing(Game->GetThingWithScriptName("MK_FI_HERO"), 3.0f, (EScriptEntityMoveType)1, false, false, false, true);
                Game->Pause(1.5f);
                Game->FadeScreenOut(1.5f, 1.5f, CRGBColour(0, 0, 0, 255));
                Game->Pause(2.0f);
                Game->RemoveThing(npc, false, false);
                Game->CameraDefault();
            }
            Game->FadeScreenIn();
                return;
            }
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

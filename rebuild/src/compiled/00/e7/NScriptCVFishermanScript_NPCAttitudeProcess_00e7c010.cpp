// NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing Me) -- the fisherman's attitude thread: with an is-alive predicate on Me, each frame watch for the hero hitting him (with a weapon, or with any special ability other than Heal Life); once he is hit with 1 health or less, set relation counter 2, make the fisherman run away, take him under script control and play the "hate" scene (stand points, camera, speech, run to MK_FI_HERO, fade out, remove him, fade in)
// Inline-budget: header-contract inline devices, no flag and no pragma (ablation/, each removed in turn) -- FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (retail expands the pause ctor in the hate scene; plain inline calls it: DIFFER(1627v1623)); inline CScriptThing::~CScriptThing (retail expands ~Me at the shared exit 0x00E7C17F; out of line DIFFER(1586v1623)); inline CScriptThing::MsgIsHitBy (the null-safe wrapper, expanded in the watch; out of line DIFFER(1609v1623)); inline CIsThingAlive ctor (expanded at the predicate; out of line DIFFER(1623v1623)). Per-row flags were not needed: the row matches at default flags.
// Leaf: ?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7C010, 1623 bytes, thiscall, `ret 0Ch` (the by-value CScriptThing), no EH frame, default flags.
// Identity (two sources): FableWin.pdb / Ego_r.pdb ?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z and the Ego_d.pdb proc of
//   the same name; spawned by the landed CFisherman::Init 0x00E7BD60 as "ParentClass." + "NPCAttitudeProcess" through CSpawnedFunc1<CV_FishermanScript,
//   CScriptThing> (SuspendableProcess 0x00E7C670).
// Source shape: Ego_d.pdb line table (lines 68..125, ref egod_lines/) and locals: parameter `Me`; function scope `_Fail_Val`; one block {seh_me,
//   {NPCPlacementMarker, speech_movie, PauseMyEntitiesPlease, {NPCHeroMarker, NPCCamera}}}. Generated quest-script text as the landed Mains
//   (0x00EBFD10, 0x00D706E0): `for(;!false;Game->NewScriptFrame()){if(T)return..;` for the endless watch (retail duplicates its head check:
//   0x00E7C063 and 0x00E7C170), the seh macro and the speak-and-wait macro each on one line, each terminate check on its statement's line.
// Condition (retail, not the parked draft's): `MsgIsHitBy(HERO) || (MsgIsHitByAnySpecialAbilityFrom(HERO) && !MsgIsHitBySpecialAbilityFrom(
//   HERO_ABILITY_HEAL_LIFE_SPELL, HERO))` -- the third test's null-Data path and false result both reach the `mov [esp+13h],1` true store
//   (0x00E7C0E0 / 0x00E7C0F3); then `GetHealth(Me)<=1.0f` (`fcomp 1.0 ; test ah,41h ; jnp` into the scene, 0x00E7C166).
// Callees: CScriptThing copy 0x004ABE90 (the predicate's Thing) / ~CScriptThing 0x004AA840 / GetAngleXY 0x004AAA40 / IsAlive 0x004AB130 (devirtualised
//   on locals); CScriptBase::SetPredicate 0x00CB7920 / IsActiveThreadTerminating 0x00CB7940; ~CIsThingAlive 0x00E241B0; CCharString 0x0099EBF0 / ~
//   0x0099EAE0; CBaseObject() 0x0099A380; ~CBaseClass 0x0099A2E0; operator delete 0x00BFE9BC; ScriptedThingBase IsValid 0x00CD23B9 / Release
//   0x00CD2770 / ~ 0x007E74D0 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / MoveToThing 0x007E7300; movie
//   ctor 0x006E7B60 / ~ 0x006E7B80; IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); Me's implementation slots 21 MsgIsHitBy, 42
//   MsgIsHitByAnySpecialAbilityFrom, 41 MsgIsHitBySpecialAbilityFrom; Game (+0x40) slots 7, 8, 70, 72, 77, 108, 130, 264, 370, 373, 374, 376, 379,
//   411, 416, 472, 474, 475. Data: NPCAttitudes[2] 0x0143E930 (the global ?NPCAttitudes@@3PAJA, not the invented CQ_SunnyvaleMasterData::RelationCounters opt-in;
//   NAME_FIXES.tsv); literals SCRIPT_NAME_HERO 0x0125D1C8, NPC_NPCStand
//   0x012EA1E8, NPC_HeroStand 0x012EA1D8, NPC_Camera 0x012EA1CC, TEXT_QST_023_IDLE_03_HATE 0x012EA1B0, MK_FI_HERO 0x012EA1A4.
// Layout: engine/CV_FishermanScript.h (FishermanMustRunAway +0x50).
// Header contract bodies: inline ~CScriptThing (expanded at the shared terminate exit 0x00E7C17F), the null-safe Msg* wrappers, the CIsThingAlive
//   ctor; FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (as the landed generated Mains).
// No `// Inferred:` lines.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_FishermanScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EScriptEntityMoveType.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
extern long NPCAttitudes[3];                                 // 0x0143E928 ?NPCAttitudes@@3PAJA (Ego_d.pdb data name; landed 0x00CDBA10 / 0x00E7C8E0)
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnySpecialAbilityFrom(attacker); }
inline bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBySpecialAbilityFrom(ability, attacker); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }

void NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing Me)
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
    for(;!false;Game->NewScriptFrame()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO")||(Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")&&!Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(Game->GetHealth(Me)<=1.0f){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                NPCAttitudes[2]=2;
                FishermanMustRunAway=true;
                CScriptGameResourceObjectScriptedThingBase seh_me;
                if(seh_me.IsValid())seh_me.Release();while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->SheatheHeroWeapons();
                    CScriptThing NPCPlacementMarker=Game->GetNearestWithScriptName(Me,"NPC_NPCStand");
                    if(IsDistanceBetweenThingsUnder(Me,NPCPlacementMarker,10.0f)){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                        Game->EntityTeleportToThing(Me,NPCPlacementMarker,false);
                        Game->EntitySetFacingAngle(Me,NPCPlacementMarker.GetAngleXY(),true);
                        CScriptThing NPCHeroMarker=Game->GetNearestWithScriptName(Me,"NPC_HeroStand");
                        if(IsDistanceBetweenThingsUnder(Game->GetHero(),NPCHeroMarker,10.0f)){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                            Game->EntityTeleportToThing(Me,NPCHeroMarker,false);
                            Game->EntitySetFacingAngle(Me,NPCHeroMarker.GetAngleXY(),true);
                        }
                        CScriptThing NPCCamera=Game->GetNearestWithScriptName(Me,"NPC_Camera");
                        if(NPCCamera.IsAlive()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                            Game->CameraUseCameraPoint(NPCCamera,0,0,-1.0f,0,-1);
                        }
                    }
                    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_023_IDLE_03_HATE",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    seh_me.MoveToThing(Game->GetThingWithScriptName("MK_FI_HERO"),3.0f,ENTITY_MOVE_RUN,false,false,false,true);
                    Game->Pause(1.5f);
                    Game->FadeScreenOut(1.5f,1.5f,CRGBColour(0,0,0,255));
                    Game->Pause(2.0f);
                    Game->RemoveThing(Me,false,false);
                    Game->CameraDefault();
                }
                Game->FadeScreenIn();
                return _Fail_Val.Return();
            }
        }
    }
}
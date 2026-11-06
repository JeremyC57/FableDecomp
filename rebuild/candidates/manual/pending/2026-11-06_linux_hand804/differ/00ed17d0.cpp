// NScript::CV_SingingStonesScript::CManWithDoorName::Main() -- the man who knows the door's name: wanders near his spot; when hit says HIT and flags DoorManAttackedByHero; once bribed (and not yet done) says SPEAKS, flags DoorManComplete and runs to the nearest REGION_EXIT_POINT and fades out; when talked to says INTRO (then REMINDER, or ATTACKED once hit); when shown an item takes OBJECT_GEMSTONE_RUBY as the bribe, else says WRONG_ITEM
// Grade: DIFFER(4149v4279) -- 224 difflines, 4 event hunks: (1) SetPredicate(CIsThingAlive(Me)): retail expands the predicate ctor but CALLS the CScriptThing copy 0x004ABE90 inside it (GOTCHAS 1913/2131), VC7.1 here expands the copy; (2) per-site movie split: retail expands the bribe-site (SPEAKS) movie ctor (CBaseObject ctor 0x0099A380) and calls Construct 0x006E7B60 / ~ 0x006E7B80 at the other five, the OUTOFLINE opt-ins call it everywhere (implicit ctor expands all six: man_a3); (3) two exit cascades retail expands (~CCharString + operator delete + ~CBaseObject; slot 379 + ~CCountedPointer + ~CBaseObject) where ours calls -- hence 130 bytes short.
// Next idea: a per-site movie spelling (the bribe block written so VC7.1 sees the implicit ctor there only, e.g. a FORCEINLINE_CTOR opt-in used through a second movie local type that is the same class -- not a local copy) plus the open GOTCHAS 1913 predicate-copy recipe; man_a2 (while-loop head) is the other near shape at 297 lines.
// Leaf: ?Main@CManWithDoorName@CV_SingingStonesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED17D0, 4279 bytes, thiscall, plain `ret`, no EH frame (sub esp,0x104; every exit runs its own
//   destructor cascade).
// Identity (proven): slot 1 (Main) of the CManWithDoorName entity vtable 0x012F2C98 (the only reference to the row is .rdata 0x012F2C9C),
//   stored by the ctor inside CManWithDoorName::Alloc 0x00ED16E0 (0x00ED1716); slot 0 sdd 0x00ED17B0, 2 Init 0x00ED1790, 3 GetParentScript
//   0x00ED1780; registration "ManWithDoorName" in the landed CV_SingingStonesScript::Main 0x00ED10A0; Ego_r.pdb / FableWin.pdb name set
//   ?Main@CManWithDoorName@CV_SingingStonesScript@NScript@@EAEXXZ; Anniversary RTTI label agrees.
// Callees (retail census): CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30 (x41); CScriptThing copy
//   0x004ABE90 / ~ 0x004AA840 / IsAlive 0x004AB130; ~CIsThingAlive 0x00E241B0; CScriptGameResourceObjectMovieBase ctor 0x006E7B60 (x5) /
//   ~ 0x006E7B80 (x15), ~CCountedPointer<CScriptGameResourceObjectMovieBase> 0x006E7AB0 (declared-only specialization); ScriptedThingBase
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   GetScriptThing 0x007E7490 / ~ 0x007E74D0; CBaseObject 0x0099A380 / ~ 0x0099A430; CCharString 0x0099EBF0 / 0x0099E4B0 / ~ 0x0099EAE0;
//   CBasicString<char>::Compare 0x00411570 (PresentedItem == "OBJECT_GEMSTONE_RUBY"); IsDistanceBetweenThingsUnder 0x00CBE2FF;
//   FadeOutAndKillEntity 0x00CBE716 (?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z); operator delete 0x00BFE9BC;
//   pGlobalScriptDef->SingingStonesDoorManWanderMax (fable_script_def_global.h).
// Layout: engine/CV_SingingStonesScript.h (new, chunk 804, hdr/): parent DoorManIntroComplete +0x98, DoorManAttackedByHero +0x99,
//   DoorManHasBribe +0x9a, DoorManComplete +0x9b; the entity is 0x1c bytes. FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR /
//   _OUTOFLINE_DTOR (5 of 6 movie sites call) + FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR.
// Shape: predicate, seh_me, wander setup; `if (T) return; do { hit / bribe / talk / presented-item arms; NewScriptFrame(); } while (!T);`
//   the PresentedItem string lives in its own block that closes before NewScriptFrame (man_a11: -4 lines).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/CV_SingingStonesScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
inline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&,float,bool,bool);        // 0x00CBE716

void NScript::CV_SingingStonesScript::CManWithDoorName::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->SingingStonesDoorManWanderMax);
    Game->SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    CCharString RegionName;
    if (IsActiveThreadTerminating()) return;
    do {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            if (Game->GetHealth(Me) > 0.0001f) {
                if (IsActiveThreadTerminating()) return;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_060_MAN_WITH_DOOR_NAME_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
            ParentClass.DoorManAttackedByHero = true;
        }
        if (ParentClass.DoorManHasBribe && !ParentClass.DoorManComplete) {
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_060_MAN_WITH_DOOR_NAME_SPEAKS", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            ParentClass.DoorManComplete = true;
            CScriptThing ExitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (ExitPoint.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating()) return;
                    seh_me.MoveToPosition(ExitPoint.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, ExitPoint, 2.0f) && !Game->MsgOnRegionLoaded(RegionName)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (!ParentClass.DoorManIntroComplete) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_060_MAN_WITH_DOOR_NAME_INTRO", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                if (seh_me.IsValid()) seh_me.Release();
                ParentClass.DoorManIntroComplete = true;
            } else if (!ParentClass.DoorManAttackedByHero) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_060_MAN_WITH_DOOR_NAME_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                if (seh_me.IsValid()) seh_me.Release();
                ParentClass.DoorManIntroComplete = true;
            } else {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_060_MAN_WITH_DOOR_NAME_ATTACKED", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
        }
        {
            CCharString PresentedItem;
            if (!ParentClass.DoorManHasBribe && Me.MsgIsPresentedWithItem(PresentedItem)) {
                if (IsActiveThreadTerminating()) return;
                if (PresentedItem == "OBJECT_GEMSTONE_RUBY") {
                    if (IsActiveThreadTerminating()) return;
                    Game->AddItemToContainer(Me, "OBJECT_GEMSTONE_RUBY");
                    ParentClass.DoorManHasBribe = true;
                } else {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_060_MAN_WITH_DOOR_WRONG_ITEM", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (seh_me.IsValid()) seh_me.Release();
                }
            }
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

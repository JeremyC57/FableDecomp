// NScript::CV_DemonDoorsScript::CDF_Dragon::Main() -- demon-door dragon face: take scripted control of Me, make it persistent and immune, shut the nearest REGION_EXIT_POINT; each frame, when the hero talks to the face, play its renown line (ZERO..FOUR by the hero's renown level) and, at the top renown, fade the face out, open the dragon door, reopen the exit and activate Q_DragonBossFight
// Leaf: ?Main@CDF_Dragon@CV_DemonDoorsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6FC40, 1664 bytes = the body 0x00E6FC40..0x00E7029F (plain `ret` at 0x00E70290, the
//   cold unpause exit 0x00E70291, one nop) + its 8-entry switch jump table 0x00E702A0..0x00E702BF (GOTCHAS "ledger chunk 910":
//   the table belongs to the function). Split from the 1828-byte ledger row (chunk 781); CDF_SummonsCreatures::Alloc
//   0x00E702C0 and GetParentScript 0x00E70360 follow it. Grade with verify_land_jumptable.py.
// Identity: slot 1 of the CDF_Dragon vtable 0x012E8DB4 (stored by the landed CDF_Dragon::Alloc 0x00E6FB70, the "DF_Dragon"
//   factory of CV_DemonDoorsScript::Main 0x00E6E380); Ego_r.pdb / FableWin.pdb ?Main@CDF_Dragon@CV_DemonDoorsScript@NScript@@EAEXXZ
//   (private virtual), the TU's "DD_Dragon" door literal 0x012E8DD0 and the TEXT_QST_088_DRAGON_RENOWN_* lines.
// Switch (renown = Game->GetHeroRenownLevel(), slot 162; verified case by case against the retail table and the built
//   object's  relocations): 0,1 -> 0x00E6FE99 RENOWN_ZERO 0x012E8E60; 2,3 -> 0x00E6FF39 RENOWN_ONE 0x012E8E40; 4,5 ->
//   0x00E6FFD9 RENOWN_TWO 0x012E8E20; 6 -> 0x00E70079 RENOWN_THREE 0x012E8DFC; 7 -> 0x00E70162 RENOWN_FOUR 0x012E8DDC + the door
//   ("DD_Dragon" 0x012E8DD0), SetRegionExitAsActive(exit, true), ActivateQuest("Q_DragonBossFight" 0x012C45B4), return;
//   default (`ja`) -> 0x00E7011B (the break target: unpause, movie dtor).
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 (the handle and movie bases inline), CScriptThing copy 0x004ABE90 /
//   ~ 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, ~CIsThingAlive
//   0x00E241B0 (vtable 0x012C32F0), scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 /
//   Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / dtor COMDAT 0x007E74D0 (vtable 0x0127094C), movie handle (vtable
//   0x01260EF4) dtor COMDAT 0x006E7B80 and its PImp dtor ~CCountedPointer<CScriptGameResourceObjectMovieBase> 0x006E7AB0,
//   operator delete 0x00BFE9BC, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CV_DemonDoorsScript::FadeOutFaceAndOpenDoor
//   0x00E6F040; CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 81 GetNearestWithDefName,
//   162 GetHeroRenownLevel, 264 GetHealth, 276 ActivateQuest, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 477
//   SetThingPersistent, 516 EntitySetAsDamageable, 517 EntitySetAsKillable, 526 EntitySetAsToAddToComboMultiplierWhenHit, 567
//   SetRegionExitAsActive; CScriptThing slot 27 MsgIsTalkedToBy; 0.0f 0.0f 0x0122DEDC (the TU's sibling spelling).
// Model: shared headers only (engine/CV_DemonDoorsScript.h: CDF_Dragon with the CV_DemonDoorsScript& ParentClass +0x14;
//   CEntityScriptBase Game +0x04 / Me +0x08; the CBaseObject-family handle headers with their implicit ctors / dtors;
//   CWideScreenMagicPauseEntities, the RAII pause whose Game lives in edi). Shape = the landed sibling CDF_ShineLight::Main
//   0x00E6EA30 (same prelude, the lazy `push ebx` after the first exits is VC7.1's own). The one lever: the movie PImp dtor
//   declared as an out-of-line explicit specialization (17 landed rows, chunk 746 lesson) -- retail calls 0x006E7AB0 at the
//   loop-continue movie dtor (0x00E7012E) and the whole COMDAT 0x006E7B80 at the return exits; the in-class body expands it
//   (attempt a, 1712v1664). FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (the header's per-TU opt-in) is
//   defined for one model across the TU: CDF_SummonsCreatures::Main 0x00E74D70 needs it (its second pause ctor stays a call
//   otherwise); this row grades the same with or without it.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2,
                         SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)

void NScript::CV_DemonDoorsScript::CDF_Dragon::Main()
{
    CScriptGameResourceObjectScriptedThingBase dragon;
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (dragon.IsValid())
        dragon.Release();
    while (!Game->StartScriptingEntity(Me, dragon, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingPersistent(Me, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(exitPoint, false);
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            switch (Game->GetHeroRenownLevel()) {
            case 0:
            case 1:
                if (Game->GetHealth(dragon.GetScriptThing()) > 0.0f) {
                    dragon.Speak(Game->GetHero(), "TEXT_QST_088_DRAGON_RENOWN_ZERO", GROUP_SELECT_NONE, false, true, false);
                    while (dragon.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                break;
            case 2:
            case 3:
                if (Game->GetHealth(dragon.GetScriptThing()) > 0.0f) {
                    dragon.Speak(Game->GetHero(), "TEXT_QST_088_DRAGON_RENOWN_ONE", GROUP_SELECT_NONE, false, true, false);
                    while (dragon.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                break;
            case 4:
            case 5:
                if (Game->GetHealth(dragon.GetScriptThing()) > 0.0f) {
                    dragon.Speak(Game->GetHero(), "TEXT_QST_088_DRAGON_RENOWN_TWO", GROUP_SELECT_NONE, false, true, false);
                    while (dragon.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                break;
            case 6:
                if (Game->GetHealth(dragon.GetScriptThing()) > 0.0f) {
                    dragon.Speak(Game->GetHero(), "TEXT_QST_088_DRAGON_RENOWN_THREE", GROUP_SELECT_NONE, false, true, false);
                    while (dragon.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                break;
            case 7:
                if (Game->GetHealth(dragon.GetScriptThing()) > 0.0f) {
                    dragon.Speak(Game->GetHero(), "TEXT_QST_088_DRAGON_RENOWN_FOUR", GROUP_SELECT_NONE, false, true, false);
                    while (dragon.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                ParentClass.FadeOutFaceAndOpenDoor(Me, "DD_Dragon");
                Game->SetRegionExitAsActive(exitPoint, true);
                Game->ActivateQuest("Q_DragonBossFight");
                return;
            }
        }
        Game->NewScriptFrame();
    }
}
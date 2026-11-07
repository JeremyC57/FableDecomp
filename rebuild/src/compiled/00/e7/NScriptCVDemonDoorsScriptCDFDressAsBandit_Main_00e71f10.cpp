// NScript::CV_DemonDoorsScript::CDF_DressAsBandit::Main() -- demon-door face (dress as bandit): take scripted control of Me, make it persistent and immune, find the nearest REGION_EXIT_POINT and shut it; each time the hero talks to it, under a paused speech movie: if the hero wears all five bandit-camp clothes (shirt, trousers, hat, boots, gloves) say TEXT_QST_088_DRESS_AS_BANDIT_OPENING under a second paused movie, fade the face out, open DD_DressAsBandit and reactivate the exit; otherwise say TEXT_QST_088_DRESS_AS_BANDIT_INTRO
// Leaf: ?Main@CDF_DressAsBandit@CV_DemonDoorsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E71F10, 1605 bytes, thiscall, plain `ret` (two epilogue copies), frame 0x68.
// Identity (proven): Ego_r.pdb + FableWin.pdb ?Main@CDF_DressAsBandit@CV_DemonDoorsScript@NScript@@EAEXXZ; the single .rdata reference is
//   slot 1 of the CDF_DressAsBandit entity vtable 0x012E90A8 (slot 0 scalar dtor 0x00E71EF0, slot 3 GetParentScript 0x00E71EE0).
// Callees: CBaseObject 0x0099A380 / ~ 0x0099A430, CIsThingAlive (vtable 0x012C32F0, dtor 0x00E241B0) + CScriptThing copy 0x004ABE90,
//   SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor
//   0x007E74D0 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450, MovieBase ctor 0x006E7B60 / dtor
//   0x006E7B80, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator delete 0x00BFE9BC,
//   CV_DemonDoorsScript::FadeOutFaceAndOpenDoor 0x00E6F040; CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 81 GetNearestWithDefName,
//   187 IsWearingClothingItem, 264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 477 SetThingPersistent,
//   516 / 517 / 526, 567 SetRegionExitAsActive; CScriptThing slot 27 MsgIsTalkedToBy.
// Layout: engine/CV_DemonDoorsScript.h CDF_DressAsBandit (Ego_r.pdb 28 bytes): ParentClass +0x14, PMasterData +0x18.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CV_DemonDoorsScript::CDF_DressAsBandit::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetThingPersistent(Me,true);
    Game->EntitySetAsDamageable(Me,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->EntitySetAsKillable(Me,false,false);
    CScriptThing region_exit=Game->GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(region_exit,false);
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->IsWearingClothingItem(Game->GetHero(),"OBJECT_HERO_SHIRT_BANDITCAMP") &&
                Game->IsWearingClothingItem(Game->GetHero(),"OBJECT_HERO_TROUSERS_BANDITCAMP") &&
                Game->IsWearingClothingItem(Game->GetHero(),"OBJECT_HERO_HAT_BANDITCAMP") &&
                Game->IsWearingClothingItem(Game->GetHero(),"OBJECT_HERO_BOOTS_BANDITCAMP") &&
                Game->IsWearingClothingItem(Game->GetHero(),"OBJECT_HERO_GLOVES_BANDITCAMP")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase opening_movie;
                Game->StartMovieSequence("",opening_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease2(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_088_DRESS_AS_BANDIT_OPENING",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_DressAsBandit");
                Game->SetRegionExitAsActive(region_exit,true);
                break;
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_088_DRESS_AS_BANDIT_INTRO",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        Game->NewScriptFrame();
    }
}
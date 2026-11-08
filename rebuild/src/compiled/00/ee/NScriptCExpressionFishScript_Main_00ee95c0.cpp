// NScript::CExpression_FishScript::Main() -- the Expression_Fish script (the hero goes fishing): guard on the hero being conscious, make the quest persistent, wait for player control, take the hero under scripted control in a paused movie sequence, fade out, put the last-used rod (or the upgraded / plain fishing rod) in his hand and loop the fishing idle, face the first enabled MARKER_FISHING_SPOT within 10 units of the hero (water height), switch to the fishing camera, run the fishing game, play the success / failure animation, fade, disable the spot after a catch there, restore the camera and the hero, then give the catch (the spot's container items and the fish weight, or the fish itself) and deactivate the quest
// Leaf: ?Main@CExpression_FishScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE95C0, 2357 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00EE9EF4.
// Identity (proven): slot 2 (Main) of the RTTI-less script vtable 0x012F54E4 (its only reference, .rdata 0x012F54EC) that the landed
//   ctor 0x00EE94A0 and the "Expression_Fish" Alloc 0x00EE9F00 store; Ego_r.pdb / FableWin.pdb ?Main@CExpression_FishScript@NScript@@UAEXXZ.
//   PDB scope: namespace NScript, class V, base CScriptBase (the PDB ctor takes only CGameScriptInterfaceBase&: no master data).
// Callees: CScriptBase::SetPredicate 0x00CB7920 / IsActiveThreadTerminating 0x00CB7940, CScriptThing() 0x006E7B40 / copy 0x004ABE90 /
//   operator= 0x008AB980 / ~ 0x004AA840 / IsAlive 0x004AB130 (devirtualised on the locals), CScriptThing::NullCoord 0x0143E8E0 (inline
//   GetPos), CBaseObject() 0x0099A380, CScriptGameResourceObjectScriptedThingBase ClearAllActionsIncludingLoopingAnimations 0x007E7410 /
//   PlayAnimation 0x007E73D0 / PlayLoopingAnimation 0x007E73F0 / ~ 0x007E74D0, CScriptGameResourceObjectMovieBase() 0x006E7B60 / ~ 0x006E7B80,
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / operator!=(const char*) 0x0099E960, std::vector<CScriptThing>::~vector
//   0x008AC970, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748; CGameScriptInterfaceBase slots 7, 8
//   StartScriptingEntity, 55 MsgOnFishingGameFinished, 70 GetHero, 72 GetThingWithScriptName, 79 GetAllThingsWithDefNameByDistanceFrom,
//   108 RemoveThing, 123 GiveHeroItem, 124 GiveHeroItemsFromContainer, 130 SheatheHeroWeapons, 143 MakeHeroCarryItemInHand, 184
//   IsObjectInThingsPossession, 242 GetWaterHeightAtPosition, 243 IsFishingSpotEnabled, 244 DisableFishingSpot, 245 UpdateFishWeight,
//   247 HeroGoFishing, 281 DeactivateQuestLater, 290 SetQuestAsPersistent, 320 GetMostRecentValidUsedTargetName, 370 StartMovieSequence,
//   373 FadeScreenOut, 374 FadeScreenIn, 376 Pause, 380 PauseAllEntities, 384 SetToDisplayTutorialsDuringCutscenes, 394
//   IsHeroControlledByPlayer, 400 SetEnvironmentalEffectsAlwaysUpdate, 413 CameraUseCameraPoint, 416 CameraDefault, 417
//   CameraResetToViewBehindHero, 475 EntitySetFacingAngleTowardsThing, 500 EntityForceToLookAtNothing, 501 EntityResetForceToLookAt,
//   655 GetActiveQuestName. The 13 string literals were paired with retail by relocation (strchk).
// Layout: engine/CExpression_FishScript.h (new, chunk 790): Game +0x40, LastFishingLevel +0x44.
// Shape: the landed CExpression_FollowScript::Main 0x00EEA020 model (CIsThingConscious predicate with inline ctor / dtor, the hero
//   scripted thing with an inline ctor and the out-of-line dtor). Spellings that matter: GetPos is a __forceinline body with the
//   NullCoord fall-through (GOTCHAS chunks 781 / 783; plain inline leaves a call); the distance test is the explicit sum
//   `X*X + Y*Y + Z*Z` (retail squares Z, Y, X: C3DVector::GetSquaredMagnitude's X/Z/Y order and Dot both differ); the rod fallback
//   tests OBJECT_FISHING_ROD and carries it in both arms (retail keeps the call, drops the dead test: `mov bl,al` with no use);
//   the vector's destructor is declared out of line (GOTCHAS chunk 789).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CExpression_FishScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "stlport/fable_containers.h"
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
template <> std::vector<CScriptThing, std::allocator<CScriptThing> >::~vector();   // 0x008AC970

void NScript::CExpression_FishScript::Main()
{
    SetPredicate(CIsThingConscious(Game->GetHero()));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->SetQuestAsPersistent(Game->GetActiveQuestName(), false);
    while (!Game->IsHeroControlledByPlayer()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase hero;
    while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetEnvironmentalEffectsAlwaysUpdate(true);
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    Game->SetToDisplayTutorialsDuringCutscenes(true);
    Game->EntityForceToLookAtNothing(Game->GetHero());
    Game->PauseAllEntities(true);
    Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    Game->Pause(1.0f);
    Game->SheatheHeroWeapons();
    hero.ClearAllActionsIncludingLoopingAnimations();
    CCharString target = Game->GetMostRecentValidUsedTargetName();
    if (target != "") {
        if (IsActiveThreadTerminating()) return;
        Game->MakeHeroCarryItemInHand(target);
    } else {
        if (IsActiveThreadTerminating()) return;
        if (Game->IsObjectInThingsPossession("OBJECT_FISHING_ROD_UPGRADED", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            Game->MakeHeroCarryItemInHand("OBJECT_FISHING_ROD_UPGRADED");
        } else if (Game->IsObjectInThingsPossession("OBJECT_FISHING_ROD", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            Game->MakeHeroCarryItemInHand("OBJECT_FISHING_ROD");
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->MakeHeroCarryItemInHand("OBJECT_FISHING_ROD");
        }
    }
    hero.PlayLoopingAnimation("ST_FISHING_IDLE", -1, false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
    bool caught = false;
    bool atSpot = false;
    CScriptThing spot;
    std::vector<CScriptThing> spots;
    long count = Game->GetAllThingsWithDefNameByDistanceFrom(Game->GetHero(), "MARKER_FISHING_SPOT", spots);
    if (count > 0) {
        if (IsActiveThreadTerminating()) return;
        for (long i = 0; i < count; ++i) {
            if (IsActiveThreadTerminating()) return;
            if (Game->IsFishingSpotEnabled(spots[i])) {
                if (IsActiveThreadTerminating()) return;
                spot = spots[i];
                break;
            }
        }
        if (IsActiveThreadTerminating()) return;
        if (spot.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            C3DVector pos = spot.GetPos();
            pos.Z = Game->GetWaterHeightAtPosition(pos);
            C3DVector offset = Game->GetHero().GetPos() - pos;
            if (offset.X * offset.X + offset.Y * offset.Y + offset.Z * offset.Z < 10.0f * 10.0f) {
                if (IsActiveThreadTerminating()) return;
                atSpot = true;
                if (IsActiveThreadTerminating()) return;
                Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), spot, true);
            }
        }
    }
    CScriptThing camera1 = Game->GetThingWithScriptName("GlobalFishingCamera1");
    CScriptThing camera2 = Game->GetThingWithScriptName("GlobalFishingCamera2");
    Game->Pause(1.0f);
    Game->CameraUseCameraPoint(camera1, Game->GetHero(), -1.0f, 0, -1);
    Game->FadeScreenIn();
    Game->Pause(2.0f);
    CScriptThing fish;
    Game->HeroGoFishing(atSpot);
    while (!Game->MsgOnFishingGameFinished(fish)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->CameraUseCameraPoint(camera2, Game->GetHero(), -1.0f, 0, -1);
    if (fish.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        caught = true;
        hero.PlayAnimation("ST_FISHING_SUCCESS", false, true, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
        Game->Pause(0.2f);
    } else {
        if (IsActiveThreadTerminating()) return;
        hero.PlayAnimation("ST_FISHING_FAILURE", false, true, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
    }
    Game->Pause(3.75f);
    Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    Game->Pause(1.0f);
    if (caught && atSpot) {
        if (IsActiveThreadTerminating()) return;
        Game->DisableFishingSpot(spot);
    }
    Game->MakeHeroCarryItemInHand("");
    Game->EntityResetForceToLookAt(Game->GetHero());
    Game->CameraResetToViewBehindHero(0.0f);
    Game->CameraDefault();
    Game->Pause(1.0f);
    Game->PauseAllEntities(false);
    Game->FadeScreenIn();
    Game->Pause(0.5f);
    if (caught && atSpot) {
        if (IsActiveThreadTerminating()) return;
        Game->GiveHeroItemsFromContainer(spot, false);
        Game->UpdateFishWeight(fish);
        Game->RemoveThing(fish, false, true);
    } else if (fish.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->UpdateFishWeight(fish);
        Game->GiveHeroItem(fish);
    }
    Game->SetEnvironmentalEffectsAlwaysUpdate(false);
    Game->SetToDisplayTutorialsDuringCutscenes(false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
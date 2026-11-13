// NScript::CExpression_PickpocketScript::Main() -- the hero "pickpocket" expression quest: register a conscious-hero predicate, mark the quest persistent, take the hero's targeted thing and quit unless it is pickpocketable and no movie is running; otherwise open a movie sequence, pause the other entities, show the pickpocket mini-game bar and freeze the victim; scale the def's pickpocket time by the hero's skill level/max and the frame rate into a frame count; then each frame stop on releasing the d-pad button (an unexpired spot is forgiven), fail on being hit, stop when a spot runs out, update the bar with 1 - frames/time, and while the victim lives and the bar is not full give it a PickpocketSpottedChancePerSecond% chance per frame step to spot the hero (turn to face him for the spotted duration); afterwards resume the entities, release the victim and hide the bar, and unless failed send the spotted event or steal with progress% chance; finally deactivate this quest later
// Grade: DIFFER(1860v1906) -- Blocker 1 dtor variant (CIsThingConscious case): retail expands ~CIsThingConscious with its nested ~CScriptThing called and expands ~CScriptThing for the hot GetHeroTargetedThing temporary; with the inline ~CScriptThing VC7.1 calls the whole ~CIsThingConscious, which shifts the frame by 4 and flips the early-exit cross-jump and the post-loop zero register (101 lines)
// Next idea: none in plain C++ found: an out-of-line ~CScriptThing fixes the prologue, the cross-jump and the post-loop pushes but calls the loop temp dtor (h6, md112); __forceinline ~CIsThingConscious expands the nested dtor too (md376)
// Leaf: ?Main@CExpression_PickpocketScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEAEF0, 1906 bytes, thiscall, plain `ret` (eight epilogue copies), frame 0x48, no EH frame.
// Identity (proven): Ego_r.pdb + FableWin.pdb ?Main@CExpression_PickpocketScript@NScript@@UAEXXZ; script vtable 0x012F56A0 slot 2 (the landed
//   Alloc 0x00EEB730 stores it; slot 1 RegisterMain 0x00EEB670 spawns "Main").
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840 / IsAlive 0x004AB130 (also expanded once), CScriptBase::SetPredicate 0x00CB7920 /
//   IsActiveThreadTerminating 0x00CB7940, CBaseObject 0x0099A380, MovieBase dtor 0x006E7B80, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   rand 0x00BFEB16, _ftol 0x00BFEA70; CIsThingConscious vtable 0x012C2FE8; CScriptThing slots 11 GetPThing, 21 MsgIsHitBy, 75 IsAlive;
//   CGameScriptInterfaceBase slots 7, 70, 71, 168, 169, 228, 241, 281, 290, 342-344, 347, 370, 379, 395, 475, 512, 654, 655.
// Strings: "" 0x0122D70E, "EXPRESSION_PICKPOCKET" 0x0125A47C; 1.0f 0x0122DED8, 100.0f 0x01230A08.
// Data: pGlobalScriptDef 0x0143E90C PickpocketDurationSeconds +0x1010, PickpocketSpottedDurationSeconds +0x1014,
//   PickpocketSpottedChancePerSecond +0x1018.
// Layout: engine/CExpression_PickpocketScript.h (new, chunk 841): CScriptBase 0x40, Game +0x40.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CExpression_PickpocketScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_def_global.h"
enum EHeroTrainableStatType { HERO_STAT_SKILL = 5 };
enum EMiniGameType { MINI_GAME_PICKPOCKET = 3 };
enum ECutsceneBehaviour { CUTSCENE_BEHAVIOUR_NONE = 0, CUTSCENE_BEHAVIOUR_FREEZE = 1 };
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data != 0 && PImp.Data->IsAlive(); }
inline CThing* CScriptThing::GetPThing() const { if (PImp.Data == 0) return 0; return PImp.Data->GetPThing(); }

void NScript::CExpression_PickpocketScript::Main()
{
    SetPredicate(CIsThingConscious(Game->GetHero()));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsPersistent(Game->GetActiveQuestName(), false);
    CScriptThing target = Game->GetHeroTargetedThing();
    if (!Game->IsEntityPickPocketable(target)) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        return;
    }
    if (Game->IsInMovieSequence()) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        return;
    }
    {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->DisplayMiniGameInfo(true, MINI_GAME_PICKPOCKET);
        Game->EntitySetCutsceneBehaviour(target, CUTSCENE_BEHAVIOUR_FREEZE);
        Game->PauseAllNonScriptedEntities(true);
        long spotted_timer = 0;
        bool done = false;
        bool failed = false;
        bool spotted = false;
        float time = pGlobalScriptDef->PickpocketDurationSeconds;
        time /= (Game->GetHeroStatLevel(HERO_STAT_SKILL) + 1.0f) / (Game->GetHeroStatMax(HERO_STAT_SKILL) + 1.0f);
        time *= Game->GetConstantFPS();
        long count = (long)time;
        long last_count = count;
        float progress;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (!Game->IsDPadButtonHeldForExpression("EXPRESSION_PICKPOCKET")) {
                if (IsActiveThreadTerminating())
                    return;
                if (spotted) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (spotted_timer > 0) {
                        if (IsActiveThreadTerminating())
                            return;
                        spotted = false;
                    }
                }
                done = true;
            }
            if (Game->GetHero().MsgIsHitBy("")) {
                if (IsActiveThreadTerminating())
                    return;
                done = true;
                failed = true;
            }
            if (spotted) {
                if (IsActiveThreadTerminating())
                    return;
                if (spotted_timer == 0) {
                    if (IsActiveThreadTerminating())
                        return;
                    done = true;
                }
            }
            progress = 1.0f - count / time;
            if (progress > 1.0f) {
                if (IsActiveThreadTerminating())
                    return;
                progress = 1.0f;
            }
            Game->UpdateMiniGameInfoBar(progress);
            target = Game->GetHeroTargetedThing();
            if (target.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                if (!(progress >= 1.0f)) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (count < last_count) {
                        if (IsActiveThreadTerminating())
                            return;
                        long roll = rand() % 100;
                        if (roll < pGlobalScriptDef->PickpocketSpottedChancePerSecond) {
                            if (IsActiveThreadTerminating())
                                return;
                            spotted = true;
                            spotted_timer = (long)(pGlobalScriptDef->PickpocketSpottedDurationSeconds * Game->GetConstantFPS());
                            Game->EntitySetFacingAngleTowardsThing(target, Game->GetHero(), true);
                        }
                        last_count = count;
                    }
                }
                else {
                    if (IsActiveThreadTerminating())
                        return;
                    done = true;
                }
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                done = true;
            }
            if (count > 0) {
                if (IsActiveThreadTerminating())
                    return;
                --count;
            }
            if (spotted_timer > 0) {
                if (IsActiveThreadTerminating())
                    return;
                --spotted_timer;
            }
        } while (!done);
        if (IsActiveThreadTerminating())
            return;
        Game->PauseAllNonScriptedEntities(false);
        Game->EntitySetCutsceneBehaviour(target, CUTSCENE_BEHAVIOUR_NONE);
        Game->DisplayMiniGameInfo(false, MINI_GAME_PICKPOCKET);
        if (!failed) {
            if (IsActiveThreadTerminating())
                return;
            if (target.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetCutsceneBehaviour(target, CUTSCENE_BEHAVIOUR_NONE);
                if (spotted) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->SendEntityEvent((NEntityEvents::EEventType)0x13, Game->GetHero(), target, target.GetPThing());
                }
                else {
                    if (IsActiveThreadTerminating())
                        return;
                    if (rand() % 100 < progress * 100.0f) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->EntitySetAsPickPocketed(target);
                    }
                }
            }
        }
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}

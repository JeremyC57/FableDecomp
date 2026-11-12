// NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::Main() -- demon-door summoning face: take scripted control of Me, make it persistent and immune, shut the nearest REGION_EXIT_POINT; when the hero talks to it, warn, ask the yes/no question and, on yes, play the intro, then for each creature type spawn one creature at every DemonDoor_NastyGen marker and wait until all are dead; if the face is still alive play the opening, fade the face out, open its door and reopen the exit
// Leaf: ?Main@CDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E74D70, 2080 bytes (+ int3 padding), plain `ret`, no EH frame.
// Identity: slot 1 of the CDF_SummonsCreatures vtable 0x012E8E80 (0x012E8E84 is the only reference to this address; the
//   vtable is stored by Alloc 0x00E702C0, the "DF_SummonsCreatures" factory of CV_DemonDoorsScript::Main 0x00E6E559);
//   Ego_r.pdb / FableWin.pdb ?Main@CDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@EAEXXZ (private virtual); literals
//   TEXT_QST_088_SUMMON_WARNING 0x012E96B0 / _QUESTION 0x012E9690 / _INTRO 0x012E9674 / _OPENING 0x012E9630,
//   TEXT_OBJECT_HERO_ANSWER_YES 0x012C2188 / _NO 0x012C216C, "DemonDoor_NastyGen" 0x012E9660, "DD_GeneratedNasty" 0x012E964C,
//   the door "DD_SummonsCreatures" 0x012E961C.
// Callees: as CDF_Dragon::Main 0x00E6FC40 (handle, movie, CIsThingAlive, CCharString, SetPredicate, IsActiveThreadTerminating,
//   FadeOutFaceAndOpenDoor 0x00E6F040) plus the movie ctor COMDAT 0x006E7B60 (called at the second movie, expanded at the
//   first), vector<CScriptThing>::erase(iterator, iterator) 0x008AC9D0 (clear()), push_back 0x008ADF90, ~vector 0x008AC970,
//   AreAllThingsInVectorDead 0x00CBED00 (__fastcall), pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>:
//   DemonDoor_SummonCreature_NumCreatureTypes +0x1068, DemonDoor_SummonCreature_CreatureType +0x106C = Ego_r - 0xC8, the
//   offsets of engine/CScriptDef.h); CGameScriptInterfaceBase slots 7, 8, 39 MsgIsQuestionAnsweredYesOrNo, 70 GetHero, 75
//   GetAllThingsWithScriptName, 81 GetNearestWithDefName, 91 CreateCreature, 114 GiveHeroYesNoQuestion, 264 GetHealth, 370
//   StartMovieSequence, 376 Pause(2.0f), 379 PauseAllNonScriptedEntities, 477 / 516 / 517 / 526, 567 SetRegionExitAsActive;
//   CScriptThing slots 6 GetPos, 27 MsgIsTalkedToBy, 75 IsAlive.
// Model: shared headers only (the CDF_Dragon::Main model of this TU: engine/CV_DemonDoorsScript.h, CEntityScriptBase,
//   the CBaseObject-family handle/movie headers with implicit members, CWideScreenMagicPauseEntities with the header's
//   per-TU FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (retail expands both pause ctors; the plain inline
//   ctor stays a call at the second, attempt g), STLport vector<CScriptThing> with the per-TU push_back / erase(range)
//   out-of-line opt-ins, CScriptDef + fable_script_def_global.h).
// Shape: one `long answer` outlives the first movie block (the pause/movie dtors run before `if (answer == 1)`); the summon
//   arm ends in `return` (the loop's normal exit jumps straight to the exit chain), and the dead-face test is written with
//   the `!IsAlive()` arm first: retail lays that arm out as the fall-through (`call IsActiveThreadTerminating; jmp exit`, its
//   result unused because both outcomes leave), attempt b had it cold. Unlike CDF_Dragon::Main, no out-of-line
//   ~CCountedPointer<MovieBase> declaration: retail calls the whole movie dtor COMDAT 0x006E7B80 at every site here, which
//   VC7.1 gives from the in-class PImp release (the declaration makes the dtor small enough to expand: attempt c).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/fable_script_def_global.h"
#include "stlport/fable_containers.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2,
                         SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
bool FABLE_FASTCALL AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);   // 0x00CBED00

void NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::Main()
{
    CScriptGameResourceObjectScriptedThingBase face;
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (face.IsValid())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, SCRIPT_AI_PRIORITY_HIGHEST)) {
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
            long answer;
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities pause(*Game);
                if (Game->GetHealth(face.GetScriptThing()) > 0.0f) {
                    face.Speak(Game->GetHero(), "TEXT_QST_088_SUMMON_WARNING", GROUP_SELECT_NONE, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_088_SUMMON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer < 0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                    answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating())
                    return;
                if (answer == 1) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(face.GetScriptThing()) > 0.0f) {
                        face.Speak(Game->GetHero(), "TEXT_QST_088_SUMMON_INTRO", GROUP_SELECT_NONE, false, true, false);
                        while (face.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
            }
            if (answer == 1) {
                if (IsActiveThreadTerminating())
                    return;
                std::vector<CScriptThing> generators;
                std::vector<CScriptThing> creatures;
                for (long i = 0; i < pGlobalScriptDef->DemonDoor_SummonCreature_NumCreatureTypes; ++i) {
                    if (IsActiveThreadTerminating())
                        return;
                    generators.clear();
                    Game->GetAllThingsWithScriptName("DemonDoor_NastyGen", generators);
                    for (unsigned long j = 0; j < generators.size(); ++j)
                        creatures.push_back(Game->CreateCreature(pGlobalScriptDef->DemonDoor_SummonCreature_CreatureType[i],
                                                                 generators[j].GetPos(), "DD_GeneratedNasty", false));
                    while (!AreAllThingsInVectorDead(creatures)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                if (!Me.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                } else {
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities pause(*Game);
                    Game->Pause(2.0f);
                    if (Game->GetHealth(face.GetScriptThing()) > 0.0f) {
                        face.Speak(Game->GetHero(), "TEXT_QST_088_SUMMON_OPENING", GROUP_SELECT_NONE, false, true, false);
                        while (face.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    ParentClass.FadeOutFaceAndOpenDoor(Me, "DD_SummonsCreatures");
                    Game->SetRegionExitAsActive(exitPoint, true);
                }
                return;
            }
        }
        Game->NewScriptFrame();
    }
}
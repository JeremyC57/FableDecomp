// NScript::CQ_ArenaScript::PlayWave() -- spawn, track and finish one arena wave.
// Grade: DIFFER(9262v9262), one raw SIB byte remains at 0x00F1FFBA; all branch targets and relocated operands audited.
// Leaf: ?PlayWave@CQ_ArenaScript@NScript@@QAEXXZ
// Flags: (none)
// Evidence: original Game reference, one countdown Conversation, return helper and pre-header for-scope workaround.
// Inline-budget: four constructor regions, two one-statement cold return regions, and shared forced accessor/timer/colour bodies.
// Inferred: integer-zero spelling of the observed constant-false for wrapper; native float RandAngle division and one gift retain TLC behavior.
#if defined(_MSC_VER) && _MSC_VER < 1400
#define for                                                                                                  \
    if (0) {                                                                                                 \
    } else for
#endif
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_CTOR3_PASS25
#define FABLE_RETAIL_ARENA_ORIGINAL_GETTER_CHAIN_PASS25
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NSCRIPT
// Header binding correction: NScript expands the conversion and calls GetString at 0x00415D70.
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/EMusicSetType.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_ArenaScript.h"
#include "engine/CArenaRoundDef.h"
#include "engine/CArenaWaveDef.h"
#include "engine/CArenaCreatureDef.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority {
    SCRIPT_AI_PRIORITY_LOWEST = 0,
    SCRIPT_AI_PRIORITY_LOW = 1,
    SCRIPT_AI_PRIORITY_MEDIUM = 2,
    SCRIPT_AI_PRIORITY_HIGH = 3,
    SCRIPT_AI_PRIORITY_HIGHEST = 4
};
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
long FABLE_FASTCALL GFCharStringToInt(const CCharString&); // 0x0099E7F0
#define FABLE_RETAIL_SCRIPT_INLINE_ARENA_PASS25
#include "engine/FableScriptInlineArenaPass25.h"
#define T IsActiveThreadTerminating()
#define NSF Game.NewScriptFrame()
#define AR_START(THING, HANDLE)                                                                              \
    if (HANDLE.IsValid())                                                                                    \
        HANDLE.Release();                                                                                    \
    while (!Game.StartScriptingEntity(THING, HANDLE, SCRIPT_AI_PRIORITY_HIGHEST)) {                          \
        NSF;                                                                                                 \
        if (T)                                                                                               \
            return _Fail_Val.Return();                                                                       \
    }                                                                                                        \
    if (T)                                                                                                   \
        return _Fail_Val.Return();
#define AR_KEEPER(TEXT)                                                                                      \
    Conversation = Game.AddNewConversation(Game.GetHero(), false, false);                                    \
    Game.AddLineToConversation(Conversation, TEXT, false, Game.GetHero(), Game.GetHero());
#define AR_COUNT(N)                                                                                          \
    DoorTimer = N;                                                                                           \
    while (DoorTimer.GetTime() > 0) {                                                                        \
        NSF;                                                                                                 \
        if (T)                                                                                               \
            return _Fail_Val.Return();                                                                       \
    }                                                                                                        \
    if (T)                                                                                                   \
        return _Fail_Val.Return();
#define AR_CREATURE(I) Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures[I]
// Original for-scope workaround: donor for headers have a constant-false empty arm before the loop.
void NScript::CQ_ArenaScript::PlayWave() {
    TRetVoid _Fail_Val;
    CScriptThing SmallEnemySpawnPoint = Game.GetThingWithScriptName("ARENA_SpawnPoint");
    CScriptThing BigEnemySpawnPoint = Game.GetThingWithScriptName("BigCreatureSpawnPoint");
    CTimer DoorTimer;
    PauseCrowdChecker = false;

    if (!Rounds[ArenaRound].Waves[ArenaRoundWave].ShortWave && !(ArenaRound == 0 && ArenaRoundWave == 0)) {
        if (T)
            return _Fail_Val.Return();
        long Conversation;
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_THREE")
        DoorTimer = 2;
        while (DoorTimer.GetTime() > 0) {
            NSF;
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_TWO")
        DoorTimer = 2;
        while (DoorTimer.GetTime() > 0) {
            NSF;
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_ONE")
        DoorTimer = 2;
        while (DoorTimer.GetTime() > 0) {
            NSF;
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_GO")
    }
    std::vector<CScriptThing> AllBigSpawnMarkers;
    long MultipleBigEnemySpawnPoints =
        Game.GetAllThingsWithScriptName("MultipleBigCreatureSpawnPoint", AllBigSpawnMarkers);
    std::vector<CScriptThing> AllSpawnMarkers;
    long NumSpawns = Game.GetAllThingsWithScriptName("ArenaSpawn", AllSpawnMarkers);
    CScriptThing FurthestArenaSpawn = Game.GetFurthestWithScriptName(Game.GetHero(), "ArenaSpawn");
    long PositiveSpawn = GFCharStringToInt(FurthestArenaSpawn.GetDataString());
    long NegativeSpawn = PositiveSpawn - 1;
    bool NextSpawnPositive = true;
    if (NegativeSpawn == -1) {
        if (T)
            return _Fail_Val.Return();
        NegativeSpawn = NumSpawns - 1;
    }
    std::vector<CScriptThing> CurrentEnemies;
    Game.GetAllThingsWithScriptName("ArenaEnemy", CurrentEnemies);
    if (ArenaRound == 0 && ArenaRoundWave == 0 && CurrentEnemies.size() != 0) {
        if (T)
            return _Fail_Val.Return();
        TotalCreatures[0] = CurrentEnemies.size();
    } else {
        if (T)
            return _Fail_Val.Return();
        for (long i = 0; i < Rounds[ArenaRound].Waves[ArenaRoundWave].NumWaveCreatures; ++i) {
            if (T)
                return _Fail_Val.Return();
            TotalCreatures[i] = 0;
            for (long j = 0; j < AR_CREATURE(i).NumCreatures; ++j) {
                if (T)
                    return _Fail_Val.Return();
                if (ArenaRound > 4) {
                    if (T)
                        return _Fail_Val.Return();
                    CScriptThing NextCreature;
                    if (NumSpawns == 1) {
                        if (T)
                            return _Fail_Val.Return();
                        Game.SetCreatureCreationDelayFrames(1);
                        NextCreature =
                            Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                      BigEnemySpawnPoint.GetPos(), 1.0f, "ArenaEnemy", false);
                        Game.ResetCreatureCreationDelayFrames();
                    } else {
                        if (T)
                            return _Fail_Val.Return();
                        for (long idx = 0; idx < MultipleBigEnemySpawnPoints; ++idx) {
                            if (GFCharStringToInt(AllBigSpawnMarkers[idx].GetDataString()) == j) {

                                if (T)
                                    return _Fail_Val.Return();

                                Game.SetCreatureCreationDelayFrames(1);
                                NextCreature = Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                                         AllBigSpawnMarkers[idx].GetPos(),
                                                                         1.0f, "ArenaEnemy", false);
                                Game.ResetCreatureCreationDelayFrames();
                            }
                        }
                    }
                    Game.EntitySetCutsceneBehaviour(NextCreature, CUTSCENE_BEHAVIOUR_PAUSED);
                    Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                } else {
                    if (T)
                        return _Fail_Val.Return();
                    if (NextSpawnPositive) {

                        if (T)
                            return _Fail_Val.Return();

                        for (long idx = 0; idx < NumSpawns; ++idx) {
                            if (GFCharStringToInt(AllSpawnMarkers[idx].GetDataString()) == PositiveSpawn) {

                                if (T)
                                    return _Fail_Val.Return();

#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
                                CScriptThing NextCreature;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                                if (ArenaRound == 3) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreature(AR_CREATURE(i).CreatureType,
                                                                       AllSpawnMarkers[idx].GetPos(),
                                                                       "ArenaEnemy", true);
                                    Game.ResetCreatureCreationDelayFrames();
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                                             AllSpawnMarkers[idx].GetPos(),
                                                                             1.0f, "ArenaEnemy", false);
                                    Game.ResetCreatureCreationDelayFrames();
                                }
                                Game.EntitySetCutsceneBehaviour(NextCreature, CUTSCENE_BEHAVIOUR_PAUSED);
                                if (Game.GetThingWithScriptName("WhisperAlly").IsAlive()) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    if (rand() % 2 == 0) {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(
                                            NextCreature, Game.GetThingWithScriptName("WhisperAlly"));
                                    } else {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                    }
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                }
                                ArenaSpawnNeeded[PositiveSpawn] = true;
                            }
                        }
                        PositiveSpawn = (PositiveSpawn + 1) % NumSpawns;
                        NextSpawnPositive = false;
                    } else {

                        if (T)
                            return _Fail_Val.Return();

                        for (long idx = 0; idx < NumSpawns; ++idx) {
                            if (GFCharStringToInt(AllSpawnMarkers[idx].GetDataString()) == NegativeSpawn) {

                                if (T)
                                    return _Fail_Val.Return();

#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
                                CScriptThing NextCreature;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                                if (ArenaRound == 3) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreature(AR_CREATURE(i).CreatureType,
                                                                       AllSpawnMarkers[idx].GetPos(),
                                                                       "ArenaEnemy", true);
                                    Game.ResetCreatureCreationDelayFrames();
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                                             AllSpawnMarkers[idx].GetPos(),
                                                                             1.0f, "ArenaEnemy", false);
                                    Game.ResetCreatureCreationDelayFrames();
                                }
                                Game.EntitySetCutsceneBehaviour(NextCreature, CUTSCENE_BEHAVIOUR_PAUSED);
                                if (Game.GetThingWithScriptName("WhisperAlly").IsAlive()) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    if (rand() % 2 == 0) {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(
                                            NextCreature, Game.GetThingWithScriptName("WhisperAlly"));
                                    } else {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                    }
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                }
                                ArenaSpawnNeeded[NegativeSpawn] = true;
                            }
                        }
                        --NegativeSpawn;
                        if (NegativeSpawn == -1) {

                            if (T)
                                return _Fail_Val.Return();

                            NegativeSpawn = NumSpawns - 1;
                        }
                        NextSpawnPositive = true;
                    }
                }
                ++TotalCreatures[i];
            }
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
    }
    long CreatureCounter[3];
    long MaxCreatures[3];
    for (long i = 0; i < 3; ++i) {
        if (T)
            return _Fail_Val.Return();
        MaxCreatures[i] = TotalCreatures[i];
        CreatureCounter[i] = -1;
        if (TotalCreatures[i] > 0) {
            if (T)
                return _Fail_Val.Return();
            CreatureCounter[i] =
                Game.AddQuestInfoCounterList(AR_CREATURE(i).HUDType, TotalCreatures[i], 1.0f);
        }
        if (MaxCreatures[i] > 0) {
            if (T)
                return _Fail_Val.Return();
            Game.UpdateQuestInfoCounterList(CreatureCounter[i], TotalCreatures[i], -1);
        }
    }
    if (T)
        return _Fail_Val.Return();
    Game.DisplayQuestInfo(true);
    long AllCreatures = 0;
    for (long i = 0; i < 3; ++i) {
        if (T)
            return _Fail_Val.Return();
        AllCreatures += TotalCreatures[i];
    }
    if (T)
        return _Fail_Val.Return();
    while (AllCreatures > 0) {
        NSF;
        if (T)
            return _Fail_Val.Return();
        AllCreatures = 0;
        for (long i = 0; i < 3; ++i) {
            if (T)
                return _Fail_Val.Return();
            AllCreatures += TotalCreatures[i];
            if (MaxCreatures[i] > 0) {
                if (T)
                    return _Fail_Val.Return();
                Game.UpdateQuestInfoCounterList(CreatureCounter[i], TotalCreatures[i], -1);
            }
        }
        if (T)
            return _Fail_Val.Return();
        if (ExtraCreatures != 0) {
            if (T)
                return _Fail_Val.Return();
            Game.RemoveQuestInfoElement(CreatureCounter[0]);
            MaxCreatures[0] += ExtraCreatures;
            CreatureCounter[0] = Game.AddQuestInfoCounterList(AR_CREATURE(0).HUDType, MaxCreatures[0], 1.0f);
            ExtraCreatures = 0;
        }
        if (!Game.IsLevelLoaded("Arena")) {
            if (T)
                return _Fail_Val.Return();
            PlayerLeaving = true;
            for (long i = 0; i < 3; ++i) {
                if (T)
                    return _Fail_Val.Return();
                TotalCreatures[i] = 0;
            }
            if (T)
                return _Fail_Val.Return();
        }
    }
    if (T)
        return _Fail_Val.Return();
    for (long i = 0; i < 3; ++i) {
        if (T)
            return _Fail_Val.Return();
        if (MaxCreatures[i] > 0) {
            if (T)
                return _Fail_Val.Return();
            Game.RemoveQuestInfoElement(CreatureCounter[i]);
        }
    }
    if (T)
        return _Fail_Val.Return();
    Game.DisplayQuestInfo(false);
    ++ArenaRoundWave;
    PauseCrowdChecker = true;
    if (ArenaRoundWave == Rounds[ArenaRound].NumWaves) {
        if (T)
            return _Fail_Val.Return();
        ++ArenaRound;
        ArenaRoundWave = 0;
        long NumPresents = NewCrowdPoints / 100;
        if (NumPresents > 5) {
            if (T)
                return _Fail_Val.Return();
            NumPresents = 5;
        }
        if (NumPresents > 0) {

#ifdef _MSC_VER
// Inline-budget: retail shares an out-of-line cleanup at the cold exit.
#pragma inline_depth(0)
#endif
            if (T)
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            unsigned long random = rand() % pGlobalScriptDef->RewardObjects.size();
#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
            CScriptThing CreatedObject;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            C3DVector random_vector(14.0f, 0.0f, 5.0f);
            float RandAngle = (float)(rand() % 360) * (1.0f / 360.0f);
            random_vector.RotateXY(RandAngle);
            C3DVector spawn_pos = Game.GetThingWithScriptName("ARENA_CentrePoint").GetPos() + random_vector;
            CreatedObject = Game.CreateObject(pGlobalScriptDef->RewardObjects[random], spawn_pos, "");
        }
        if (T)
            return _Fail_Val.Return();
        Game.Pause(1.0f);
        Game.OverrideMusic(GetFanfareMusic(rand() % 10), false, false);
        Game.Pause(5.0f);
        Game.StopOverrideMusic(false);
        GivePrizeFund();
        while (!Game.IsHeroControlledByPlayer()) {
            NSF;

#ifdef _MSC_VER
// Inline-budget: retail shares an out-of-line cleanup at the cold exit.
#pragma inline_depth(0)
#endif
            if (T)
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        }

        if (T)
            return _Fail_Val.Return();

        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid())
            seh_Hero.Release();
        while (!Game.StartScriptingEntity(Game.GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            NSF;

            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        if (ArenaRound < 8) {
            if (T)
                return _Fail_Val.Return();
            Game.GiveHeroYesNoQuestion(GetEndRoundQuestion(), "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES",
                                       "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", true);
            long answer;
            while ((answer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
                NSF;
                if (T)
                    return _Fail_Val.Return();
            }
            if (T)
                return _Fail_Val.Return();
            if (answer == 1) {
                if (T)
                    return _Fail_Val.Return();
                ++GoldMultiplier;
            } else {
                if (T)
                    return _Fail_Val.Return();
                PlayerLeaving = true;
                Game.GiveHeroGold(GoldTotal);
                GoldTotal = 0;
                long Conversation = Game.AddNewConversation(Game.GetHero(), false, false);
                Game.AddLineToConversation(Conversation, "TEXT_QST_005_V2_ARENA_KEEPER_RETURN", false,
                                           Game.GetHero(), Game.GetHero());
                while (Game.IsConversationActive(Conversation)) {
                    NSF;
                    if (T)
                        return _Fail_Val.Return();
                }
                if (T)
                    return _Fail_Val.Return();
            }
        }
        Game.FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0));
        Game.Pause(0.5f);
    }
}

#if defined(_MSC_VER) && _MSC_VER < 1400
#undef for
#endif

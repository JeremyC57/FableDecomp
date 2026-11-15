// NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena() -- the arena rounds thread: collect the HSEnemyMarker things, then for each round play its waves (Jack's speech line before every wave after the first, then spawn each creature def's NumCreatures "HeroSoulsEnemy" creatures at the def's markers or its single marker), taunt the hero every 20 s while CreatureCount > 0 (health, combat-multiplier or card taunt), and after rounds 1 and 2 play the wave-three / wave-five cut-scenes; after round 3 set CollectedSoulFromArena
// Inline-budget: flag /d2inlT57 -- the only residue of the shape below was the per-site inline split of the plain STLport
//   std::vector<CScriptThing>::~vector (COMDAT 0x008AC970): the c2 inline trace scores the 47 sites at 43..56 for the exits retail
//   expands (every exit before the CreatureCount wait, L119 / L122 / L129 exits, the function end) and 64..73 for the sites retail
//   calls (the 11 exits inside the CreatureCount wait, the exit after it, the wave-scope end, the round==2 cut-scene exit).
//   RELOCATION_MATCH for T57..T64 (T56: L45 / L122 exits, score 56, called; T65+: the wait-loop exits, score 64, expanded); T57 is
//   the smallest. The earlier lane's fitted `template<> vector<CScriptThing>::~vector();` declaration (always called) is dropped.
// Shape (EVIDENCED by the inline trace): the rounds loop is `while (round < 3)` (generator wait-loop form); `do { } while (round < 3)`
//   compiles to the same code but gives every site a lower static frequency, so no threshold separates retail's split.
// Leaf: ?WatchForRoundsInArena@CQ_HeroSoulsArenaScript@NScript@@QAEXXZ
// Flags: /d2inlT57
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7FE90, 3148 bytes, thiscall, three plain `ret`s (0x00D804E7, 0x00D80A35, 0x00D80ADB), no EH
//   frame; the expanded vector-destructor exits after the second ret belong to the row (next function GetJackSpeech 0x00D80AE0 landed).
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb name sets hold ?WatchForRoundsInArena@CQ_HeroSoulsArenaScript@NScript@@QAEXXZ;
//   one reference, from the landed CQ_HeroSoulsArenaScript::Main 0x00D7C890 (0x00D7D5E9, reloc_bindings.tsv), which spawns it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HeroSoulsArenaScript.h"
#include "engine/CHeroSoulsRoundDef.h"
#include "engine/CHeroSoulsWaveDef.h"
#include "engine/CHeroSoulsCreatureDef.h"
#include "engine/CTimer.h"
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::~CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }

void NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena()
{
    std::vector<CScriptThing> markers;
    long round = 0;
    long jack_speech = 1;
    CTimer taunt_timer;
    long taunt_count = 0;
    Game->GetAllThingsWithScriptName("HSEnemyMarker", markers);
    while (round < 3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        for (long wave = 0; wave < Rounds[round].NumWaves; ++wave) {
            if (IsActiveThreadTerminating()) return;
            if (wave != 0) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
                Game->AddLineToConversation(conversation, GetJackSpeech(jack_speech), false, Game->GetHero(), Game->GetHero());
                Game->Pause(3.0f);
            }
            taunt_timer = 20;
            std::vector<CScriptThing> enemy_markers;
            for (long def = 0; def < Rounds[round].Waves[wave].NumWaveCreatures; ++def) {
                if (IsActiveThreadTerminating()) return;
                if (Rounds[round].Waves[wave].Creatures[def].NumCreatures > 1) {
                    if (IsActiveThreadTerminating()) return;
                    enemy_markers.clear();
                    Game->GetAllThingsWithScriptName(Rounds[round].Waves[wave].Creatures[def].Location, enemy_markers);
                }
                for (long i = 0; i < Rounds[round].Waves[wave].Creatures[def].NumCreatures; ++i) {
                    if (IsActiveThreadTerminating()) return;
                    if (Rounds[round].Waves[wave].Creatures[def].NumCreatures > 1) {
                        if (IsActiveThreadTerminating()) return;
                        Game->SetCreatureCreationDelayFrames(1);
                        Game->CreateCreature(Rounds[round].Waves[wave].Creatures[def].CreatureType, enemy_markers[i].GetPos(), "HeroSoulsEnemy", true);
                        Game->ResetCreatureCreationDelayFrames();
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        CScriptThing marker = Game->GetThingWithScriptName(Rounds[round].Waves[wave].Creatures[def].Location);
                        Game->SetCreatureCreationDelayFrames(1);
                        Game->CreateCreature(Rounds[round].Waves[wave].Creatures[def].CreatureType, marker.GetPos(), "HeroSoulsEnemy", true);
                        Game->ResetCreatureCreationDelayFrames();
                    }
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            while (CreatureCount > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (taunt_count > 2 && taunt_timer.GetTime() == 0) {
                    if (IsActiveThreadTerminating()) return;
                    if (rand() % 4 == 0) {
                        if (IsActiveThreadTerminating()) return;
                        taunt_count = 0;
                    }
                }
                if (taunt_timer.GetTime() == 0) {
                    if (IsActiveThreadTerminating()) return;
                    CCharString taunt("");
                    if (Game->GetHeroHealth() < 60.0 && rand() % 2 == 0) {
                        if (IsActiveThreadTerminating()) return;
                        taunt = "TEXT_QST_B04_JACK_HEALTH_TAUNT";
                    } else if (Game->GetPlayerCreatureCombatMultiplier() < 8 && rand() % 4 == 0) {
                        if (IsActiveThreadTerminating()) return;
                        taunt = "TEXT_QST_B04_JACK_MULTIPLIER_TAUNT";
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (taunt_count == 0) {
                            if (IsActiveThreadTerminating()) return;
                            taunt = "TEXT_QST_B04_JACK_CARD_TAUNT";
                        } else if (taunt_count == 1) {
                            if (IsActiveThreadTerminating()) return;
                        } else if (taunt_count == 2) {
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                    if (taunt != "") {
                        if (IsActiveThreadTerminating()) return;
                        long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
                        Game->AddLineToConversation(conversation, taunt, false, Game->GetHero(), Game->GetHero());
                    }
                    taunt_timer = 20;
                    ++taunt_count;
                }
            }
            if (IsActiveThreadTerminating()) return;
            ++jack_speech;
        }
        if (IsActiveThreadTerminating()) return;
        ++round;
        if (round == 1) {
            if (IsActiveThreadTerminating()) return;
            PlayCutscene("CS_HEROSOULS_SOUL1_ARENA_WAVE_THREE");
        } else if (round == 2) {
            if (IsActiveThreadTerminating()) return;
            PlayCutscene("CS_HEROSOULS_SOUL1_ARENA_WAVE_FIVE");
        }
    }
    if (IsActiveThreadTerminating()) return;
    MasterScriptData->CollectedSoulFromArena = true;
}
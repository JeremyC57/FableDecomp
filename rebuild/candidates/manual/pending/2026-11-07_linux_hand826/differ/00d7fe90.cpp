// NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena() -- the arena rounds thread: collect the HSEnemyMarker things, then for each round play its waves (Jack's speech line before every wave after the first, then spawn each creature def's NumCreatures "HeroSoulsEnemy" creatures at the def's markers or its single marker), taunt the hero every 20 s while CreatureCount > 0 (health, combat-multiplier or card taunt), and after rounds 1 and 2 play the wave-three / wave-five cut-scenes; after round 3 set CollectedSoulFromArena
// Grade: DIFFER(2105v3148) -- every non-exit instruction, frame slot and register matches; the residue is VC7.1's per-site inline split of std::vector<CScriptThing>::~vector 0x008AC970 (retail expands it at the 13 terminate exits and calls it at the wave-scope ends); chunk 826: the a12 plain-inline form expands every exit (4460, md 723); /d2inlT20..40 move the split but never to retail's pattern and /d2inlS10 changes nothing, so it is the threshold side with our per-site frequencies ordered differently (diagnostic only)
// Next idea: the per-site frequency lever (INLINE_DEPTH_REVISIT 2026-11-03: loop guards / do-while(0) only make sites hotter) on the CreatureCount loop and the wave-scope end; known residue class (per-exit ~vector<CScriptThing> split): park
// Leaf: ?WatchForRoundsInArena@CQ_HeroSoulsArenaScript@NScript@@QAEXXZ
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
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::~CScriptThing() {}
template<> std::vector<CScriptThing, std::allocator<CScriptThing> >::~vector();
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }

void NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena()
{
    std::vector<CScriptThing> markers;
    long round = 0;
    long jack_speech = 1;
    CTimer taunt_timer;
    long taunt_count = 0;
    Game->GetAllThingsWithScriptName("HSEnemyMarker", markers);
    do {
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
    } while (round < 3);
    if (IsActiveThreadTerminating()) return;
    MasterScriptData->CollectedSoulFromArena = true;
}

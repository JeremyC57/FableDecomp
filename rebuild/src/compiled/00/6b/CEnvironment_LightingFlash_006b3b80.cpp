// CEnvironment::LightingFlash() -- the storm's lightning: while it rains above the threshold, roll the flash chance; on a flash grow the chance by the lightning frequency, fade the lightning theme in on priority 4 of every channel and play the thunder near the hero (unless the hero is inside a volume); after the flash duration fade it back out; otherwise let the chance decay with time
// Leaf: ?LightingFlash@CEnvironment@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B3B80, 734 bytes (736 with padding); thiscall, plain `ret`.
// Identity: FableWin.pdb ?LightingFlash@CEnvironment@@AAEXXZ (0x0244E310, directly before ?Update@CEnvironment@@QAEXM@Z 0x0244E500;
//   Ego_r.pdb 0x006EF6A0): private, no arguments; the only caller is CEnvironment::Update 0x006B3FF0 (call at 0x006B42B2, after
//   UpdateSunAndMoonPositions 0x006B2CA0 and UpdateWind 0x006B3030). The catalog's "CEnvironment_UpdateThunder" is a placeholder.
// Callees: GFGetTime 0x009E1BC0, GFFloatRandom(float, unsigned long*) 0x004978A0 (__fastcall: seed in ecx), CEnvironment::
//   TransitionToThemeAllChannels 0x006B38D0, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CVectorMap::LowerBound 0x0040F020 (GetTC<CTCVolumeContainmentTracker>, interface 0x75), CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CSoundInit() 0x0041C930, CThing::GetPos 0x004C73D0, CThingSoundEmitter::Create 0x008369F0 / PlaySoundEvent 0x00836840,
//   IS_THING_ALIVE<CThingSoundEmitter *> 0x0065D55F (__fastcall, out of line), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CEnvironmentThemePriorityStackEntry::TransitionToDefaultTheme 0x00886400.
// Globals: PSoundManager 0x013B8394, CMainGameComponent::PMainGameComponent 0x013B86A0 (sample bank +0x10, world seed +0x1613C),
//   the function-local static last_flash_update_time 0x013BA8E0 (guard bit 0 of 0x013BA8E8; descriptive name).
// Shape: GetRainStrength and TransitionToDefaultThemeAllChannels (0x006B3950) are same-TU bodies expanded here (the priority-4
//   transition folds to the six-channel loop over entry 4, +0xC0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_THEMESTACKS
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#define FABLE_RETAIL_CENVIRONMENT_RAINSTRENGTH
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CENVIRONMENT_WEATHERSCALERS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_CREATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CEnvironment.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CThingSoundEmitter.h"
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFGetTime.h"
#include "engine/ThingAlive.h"

// Same-TU bodies (Ego_r.pdb / FableWin.pdb members of the CEnvironment TU; retail keeps their out-of-line copies at 0x006B2BD0 and
// 0x006B3950 and expands them here).
float CEnvironment::GetRainStrength() const
{
    return RainStrength;
}

void CEnvironment::TransitionToDefaultThemeAllChannels(long priority, float duration)
{
    if (ForcedEnvironmentTheme)
        return;
    if (priority < 1 || priority >= 5)
        return;
    for (long channel = 0; channel < 6; ++channel)
        PriorityStackChannels[channel].GetEntry(priority).TransitionToDefaultTheme(duration);
}

void CEnvironment::LightingFlash()
{
    if (!(LightningFrequency > 0.0f || LightningStartTime >= 0.0))
        return;
    float rain_strength = GetRainStrength();
    double time = GFGetTime();
    static double last_flash_update_time = time;
    if (rain_strength > LightningRainThreshold && LightningStartTime < 0.0
        && GFFloatRandom(LightningCurrentFlashChance, &LightningRandomSeed) < 1.0f) {
        LightningCurrentFlashChance += LightningFrequency;
        TransitionToThemeAllChannels(4, LightningTheme, LightningFadeInDuration);
        LightningStartTime = time;
        if (PSoundManager) {
            CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCVolumeContainmentTracker* tracker;
            if (hero && !hero->IsDead() && hero->GetTC(&tracker) && !tracker->GetPThingInside()) {
                CSoundInit init;
                init.GainFaderBits = 0x8080;
                C3DVector pos = hero->GetPos();
                pos.X += GFFloatRandom(20.0f, CMainGameComponent::Get()->GetWorldSeed()) - 10.0f;
                pos.Y += GFFloatRandom(20.0f, CMainGameComponent::Get()->GetWorldSeed()) - 10.0f;
                pos.Z += 30.0f;
                CThingSoundEmitter* emitter = CThingSoundEmitter::Create(pos, 0, false);
                if (IS_THING_ALIVE(emitter)) {
                    CCharString sound_name("CS_LIGHTNING_THUNDER");
                    emitter->PlaySoundEvent(CMainGameComponent::Get()->GetSampleBank(), sound_name, false, init, false);
                }
            }
        }
    } else if (LightningStartTime >= 0.0 && time - LightningStartTime > LightningFlashDuration) {
        TransitionToDefaultThemeAllChannels(4, LightningFadeOutDuration);
        LightningStartTime = -1.0;
    } else {
        double chance = LightningCurrentFlashChance - (time - last_flash_update_time);
        if (0.0 > chance)
            chance = 0.0;
        LightningCurrentFlashChance = (float)chance;
    }
    last_flash_update_time = time;
}
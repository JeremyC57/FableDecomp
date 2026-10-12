// Host-side graphics settings: the handheld overlay and the Android launcher own them, the
// game's own Video Options page is inert (FABLE_HOST_GFX). They mirror the game's
// CEngineVideoOptions (landscape / model / texture / shadow / effects quality, each
// Low..Highest, plus the resolution) and add a few direct levers (draw distance, glow,
// water reflection, weather). Applied at CEngine::SetVideoOptions (0xB24690, wrapped): the
// quality fields and the resolution are rewritten in the options the game passes, the
// extras act on the engine's four CEngineVideoOptionsDef objects. Persisted in
// <data dir>/graphics.txt; a change is re-applied on the game thread at the next input
// update (the engine resets the device itself).
#pragma once

#include <cstdint>

namespace host::gfx {

enum Quality { LOW, NORMAL, HIGH, HIGHEST };
const char* qualityName(int q);  // "Low" .. "Highest"

struct Settings {
    int resolutionScale = 100;  // percent of the game's own back buffer (50..100), 3:2 kept
    int landscape = NORMAL, model = NORMAL, texture = NORMAL, shadow = NORMAL, effects = NORMAL;  // Quality
    int drawDistance = 100;     // percent of the def's draw distance (30..150)
    bool glow = true;           // false: the glow/bloom chain is off (def EnableGlow) and its passes are skipped
    bool water = true;          // false: no water reflection pass (def EnableWaterReflection)
    bool weather = true;        // false: no weather effects (def EnableWeatherEffects)
};

const Settings& get();
void set(const Settings& s);   // saves and schedules a re-apply
void load();                   // from graphics.txt (called once by the first get())
bool applyPending();           // on the game thread: re-runs SetVideoOptions if a change waits
uint32_t shadowBufferSize();   // the active def's ShadowBufferSize (for the renderer), 0 unknown/off
bool seeded();                 // the settings came from a file or the game's own first options

}  // namespace host::gfx

// Host-side graphics settings: the handheld overlay and the Android launcher own them, the
// game's own Video Options page is inert (FABLE_HOST_GFX). They mirror the game's
// CEngineVideoOptions (landscape / model / texture / shadow / effects quality, each
// Low..Highest, plus the resolution) and add a few direct levers (draw distance, glow,
// water reflection, weather, distortion, character LOD). Applied at
// CEngine::SetVideoOptions (0xB24690, wrapped): the
// quality fields and the resolution are rewritten in the options the game passes, the
// extras act on the engine's four CEngineVideoOptionsDef objects. Persisted in
// <data dir>/graphics.txt; a change is re-applied on the game thread at the next input
// update (the engine resets the device itself).
#pragma once

#include <cstdint>

namespace host::gfx {

enum Quality { LOW, NORMAL, HIGH, HIGHEST };
const char* qualityName(int q);  // "Low" .. "Highest"

enum Aspect { ASPECT_AUTO, ASPECT_4_3, ASPECT_16_9, ASPECT_16_10, ASPECT_3_2, ASPECT_5_4, ASPECT_COUNT };
const char* aspectName(int a);
void aspectValue(int a, int w, int h, float& x, float& y);  // the ratio as the pair the engine wants (auto: from w, h)

struct Settings {
    // the game's own CEngineVideoOptions
    int resolutionScale = 100;  // render resolution: percent of the window (50..100), its aspect kept
    int refreshRate = 0;        // Hz written into the options; 0 = the game's / the display's
    int antialiasing = 0;       // multisamples: 0 off, 2, 4
    int aspect = ASPECT_AUTO;   // Aspect: the projection's screen ratio
    int landscape = NORMAL, model = NORMAL, texture = NORMAL, shadow = NORMAL, effects = NORMAL;  // Quality
    int brightness = 50;        // 0..100, 50 = neutral (a gamma ramp: the GLES present pass, SetGammaRamp on Direct3D)
    int vsync = 1;              // swap interval: 0 off, 1 every refresh, 2 every second (a 30 fps lock at 60 Hz)
    int fpsCap = 0;             // frames per second the game thread is held to (0 = none): steadier pacing, less heat and battery on the handheld
    // direct levers on the engine's video option defs
    int drawDistance = 100;     // percent of the def's draw distance (30..150)
    bool glow = true;           // false: the glow/bloom chain is off (def EnableGlow) and its passes are skipped
    bool water = true;          // false: no water reflection pass (def EnableWaterReflection)
    bool weather = true;        // false: no weather effects (def EnableWeatherEffects)
    bool distortion = true;     // false: no screen distortion pass (defs Enable2DDisplacement / Enable3DDisplacement; 2D is on in every retail def)
    int characterLod = 0;       // 0..3: floor for the defs' CharacterLODErrorTolerance (0 in every retail def: animated meshes never leave LOD 0)
};
float characterLodTolerance(int level);  // the tolerance a characterLod level writes (0, 1, 1.5, 3)
float gammaExponent(int brightness);  // the exponent the present pass applies (1 at 50)

const Settings& get();
void set(const Settings& s);   // saves and schedules a re-apply
void load();                   // from graphics.txt (called once by the first get())
bool applyPending();           // on the game thread: re-runs SetVideoOptions if a change waits
uint32_t shadowBufferSize();   // the active def's ShadowBufferSize (for the renderer), 0 unknown/off
bool seeded();                 // the settings came from a file or the game's own first options

}  // namespace host::gfx

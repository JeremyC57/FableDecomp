// Host-side graphics settings (the handheld overlay and the Android launcher own them, not
// the game's Video Options page). They act on the engine's video option defs
// (CEngineVideoOptionsDef: draw distance, LOD tolerances, shadow buffer) at
// CEngine::SetVideoOptions (0xB24690, wrapped), and on the renderer (shadow passes skipped).
// Persisted in <data dir>/graphics.txt. A change is re-applied on the game thread at the
// next input update.
#pragma once

#include <cstdint>

namespace host::gfx {

struct Settings {
    int drawDistance = 100;   // percent of the def's draw distance (30..150)
    int modelDetail = 100;    // percent: LOD error tolerance grows as this shrinks (25..100)
    int shadowSize = 0;       // shadow buffer edge: 0 = the def's, else 128..1024
    bool shadows = true;      // false: the shadow buffer stays cleared (its passes are skipped)
    bool glow = true;         // false: the glow/bloom chain is skipped
};

const Settings& get();
void set(const Settings& s);   // saves and schedules a re-apply
void load();                   // from graphics.txt (called once by the first get())
bool applyPending();           // on the game thread: re-runs SetVideoOptions if a change waits
uint32_t shadowBufferSize();   // the active def's ShadowBufferSize (for the renderer), 0 unknown

}  // namespace host::gfx

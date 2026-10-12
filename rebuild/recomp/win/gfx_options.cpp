// See gfx_options.hpp.
#include "gfx_options.hpp"

#include "host.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <mutex>
#include <string>

using namespace host;

extern "C" void F_00B24690_orig(Ctx* c);

namespace host::gfx {
namespace {

std::mutex g_lock;
Settings g_settings;
bool g_loaded = false, g_pending = false;
uint32_t g_engine = 0;       // the CEngine the game configured
uint32_t g_optsCopy = 0;     // a guest copy of the last CVideoOptions passed in (13 dwords)
uint32_t g_shadowSize = 0;

// CEngineVideoOptionsDef fields (Ego_r layout; the def object the engine holds)
constexpr uint32_t kLODErrorTolerance = 68, kCharacterLODErrorTolerance = 72, kDrawDistanceMultiplier = 76, kDrawDistanceMinimum = 80,
                   kDrawDistanceMaximum = 84, kRepeatedMeshDrawDistanceFactor = 88, kZSpriteDrawDistanceMultiplier = 100, kShadowBufferSize = 104,
                   kShadowDistanceScale = 108;
constexpr uint32_t kDefSlots[4] = {0x120, 0x124, 0x128, 0x12C};  // CEngine: LOW, NORMAL, HIGH, HIGHEST

struct Original {  // a def's values before the host touched them
    float lod, charLod, ddMul, ddMin, ddMax, repeated, zsprite, shadowScale;
    uint32_t shadowSize;
};
std::map<uint32_t, Original> g_originals;

std::string path() {
    const char* d = std::getenv("FABLE_RECOMP_DATA");
    return std::string(d ? d : ".") + "/graphics.txt";
}

void save() {
    if (FILE* f = std::fopen(path().c_str(), "w")) {
        std::fprintf(f, "draw_distance=%d\nmodel_detail=%d\nshadow_size=%d\nshadows=%d\nglow=%d\n", g_settings.drawDistance, g_settings.modelDetail,
                     g_settings.shadowSize, g_settings.shadows ? 1 : 0, g_settings.glow ? 1 : 0);
        std::fclose(f);
    }
}

void applyToDef(uint32_t def, const Settings& s) {
    if (!def) return;
    auto it = g_originals.find(def);
    if (it == g_originals.end()) {
        Original o{rdf32(def + kLODErrorTolerance), rdf32(def + kCharacterLODErrorTolerance), rdf32(def + kDrawDistanceMultiplier),
                   rdf32(def + kDrawDistanceMinimum), rdf32(def + kDrawDistanceMaximum), rdf32(def + kRepeatedMeshDrawDistanceFactor),
                   rdf32(def + kZSpriteDrawDistanceMultiplier), rdf32(def + kShadowDistanceScale), rd32(def + kShadowBufferSize)};
        it = g_originals.emplace(def, o).first;
        log("gfx: def 0x%08X: lod %.3f charlod %.3f dd x%.2f [%.0f..%.0f] repeated %.2f zsprite %.2f shadow %u scale %.2f", def, o.lod, o.charLod, o.ddMul, o.ddMin,
            o.ddMax, o.repeated, o.zsprite, o.shadowSize, o.shadowScale);
    }
    const Original& o = it->second;
    const float dd = s.drawDistance / 100.0f, lod = 100.0f / static_cast<float>(std::max(10, s.modelDetail));
    wrf32(def + kDrawDistanceMultiplier, o.ddMul * dd);
    wrf32(def + kDrawDistanceMaximum, o.ddMax * dd);
    wrf32(def + kDrawDistanceMinimum, o.ddMin * dd);
    wrf32(def + kRepeatedMeshDrawDistanceFactor, o.repeated * dd);
    wrf32(def + kZSpriteDrawDistanceMultiplier, o.zsprite * dd);
    wrf32(def + kLODErrorTolerance, o.lod * lod);
    wrf32(def + kCharacterLODErrorTolerance, o.charLod * lod);
    wr32(def + kShadowBufferSize, s.shadowSize ? static_cast<uint32_t>(s.shadowSize) : o.shadowSize);
}

}  // namespace

void load() {
    if (g_loaded) return;
    g_loaded = true;
    if (FILE* f = std::fopen(path().c_str(), "r")) {
        char line[128];
        while (std::fgets(line, sizeof line, f)) {
            int v = 0;
            if (std::sscanf(line, "draw_distance=%d", &v) == 1) g_settings.drawDistance = std::clamp(v, 30, 150);
            else if (std::sscanf(line, "model_detail=%d", &v) == 1) g_settings.modelDetail = std::clamp(v, 25, 100);
            else if (std::sscanf(line, "shadow_size=%d", &v) == 1) g_settings.shadowSize = v;
            else if (std::sscanf(line, "shadows=%d", &v) == 1) g_settings.shadows = v != 0;
            else if (std::sscanf(line, "glow=%d", &v) == 1) g_settings.glow = v != 0;
        }
        std::fclose(f);
    }
}

const Settings& get() {
    std::lock_guard<std::mutex> l(g_lock);
    load();
    return g_settings;
}

void set(const Settings& s) {
    std::lock_guard<std::mutex> l(g_lock);
    load();
    g_settings = s;
    g_pending = true;
    save();
}

uint32_t shadowBufferSize() { return g_shadowSize; }

bool applyPending() {
    uint32_t engine, opts;
    {
        std::lock_guard<std::mutex> l(g_lock);
        if (!g_pending || !g_engine || !g_optsCopy) return false;
        g_pending = false;
        engine = g_engine, opts = g_optsCopy;
    }
    log("gfx: re-applying the video options");
    guestCallThis(0xB24690, engine, {opts});
    return true;
}

}  // namespace host::gfx

// 0xB24690: bool __thiscall CEngine::SetVideoOptions(const CVideoOptions* o)
extern "C" void host_video_options(Ctx* c) {
    using namespace host::gfx;
    const uint32_t engine = c->ecx, opts = rd32(c->esp + 4);
    Settings s;
    {
        std::lock_guard<std::mutex> l(g_lock);
        load();
        s = g_settings;
        g_engine = engine;
        if (!g_optsCopy) g_optsCopy = gcalloc(1, 64);
        if (opts != g_optsCopy) std::memcpy(gp(g_optsCopy), gp(opts), 52);
    }
    HLOG(1, "gfx: SetVideoOptions(engine 0x%08X): %ux%u, dwords %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X", engine, rd32(opts), rd32(opts + 4),
         rd32(opts + 8), rd32(opts + 12), rd32(opts + 16), rd32(opts + 20), rd32(opts + 24), rd32(opts + 28), rd32(opts + 32), rd32(opts + 36),
         rd32(opts + 40), rd32(opts + 44), rd32(opts + 48));
    for (uint32_t slot : kDefSlots) applyToDef(rd32(engine + slot), s);
    // the renderer identifies shadow passes by the buffer size the active defs name
    uint32_t size = 0;
    for (uint32_t slot : kDefSlots)
        if (const uint32_t def = rd32(engine + slot)) size = rd32(def + kShadowBufferSize);
    g_shadowSize = size;
    F_00B24690_orig(c);
}

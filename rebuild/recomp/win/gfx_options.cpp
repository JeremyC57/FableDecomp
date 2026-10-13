// See gfx_options.hpp.
#include "gfx_options.hpp"

#include "host.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <mutex>
#include <string>

using namespace host;

namespace w32 { bool screenSize(int& w, int& h); }  // posix/w32/user.cpp: the window / framebuffer size
namespace host { void d3d9ApplyGamma(float exponent); }  // win/d3d9.cpp: SetGammaRamp on the native device (DXVK); GLES applies it in Present

extern "C" void F_00B24690_orig(Ctx* c);

namespace host::gfx {
namespace {

std::mutex g_lock;
Settings g_settings;
bool g_loaded = false, g_seeded = false, g_pending = false;
uint32_t g_engine = 0;       // the CEngine the game configured
uint32_t g_optsCopy = 0;     // a guest copy of the last CVideoOptions passed in (13 dwords)
uint32_t g_shadowSize = 0;
uint32_t g_gameW = 0, g_gameH = 0;  // the resolution the game itself asked for (the scale's base)

// CEngineVideoOptions (what SetVideoOptions receives), dword offsets
constexpr uint32_t kWidth = 0, kHeight = 4, kAspectX = 8, kAspectY = 12, kRefresh = 20, kVsync = 24, kLandscape = 28, kMesh = 32, kTexture = 36, kShadow = 40, kEffects = 44, kAntialias = 48;

// CEngineVideoOptionsDef fields. Retail is the Ego_r layout shifted down by four bytes
// (confirmed from a raw dump: HiresTextureMemory 0/6/10/16 MB, LODErrorTolerance 3/2/1/0,
// DrawDistanceMultiplier 0.75/1/1.25/1.5, ShadowBufferSize 0/1024/2048/4096 for Low..Highest).
constexpr uint32_t kLODErrorTolerance = 64, kCharacterLODErrorTolerance = 68, kDrawDistanceMultiplier = 72, kDrawDistanceMinimum = 76,
                   kDrawDistanceMaximum = 80, kRepeatedMeshDrawDistanceFactor = 84, kZSpriteDrawDistanceMultiplier = 96, kShadowBufferSize = 100,
                   kEnableGlow = 0x6E, kEnableWaterReflection = 0x70, kEnableWeatherEffects = 0x71;
constexpr uint32_t kDefSlots[4] = {0x120, 0x124, 0x128, 0x12C};  // CEngine: LOW, NORMAL, HIGH, HIGHEST

struct Original {  // a def's values before the host touched them
    float lod, charLod, ddMul, ddMin, ddMax, repeated, zsprite;
    uint8_t glow, water, weather;
};
std::map<uint32_t, Original> g_originals;

std::string path() {
    const char* d = std::getenv("FABLE_RECOMP_DATA");
    return std::string(d ? d : ".") + "/graphics.txt";
}

int clampQ(int v) { return std::clamp(v, 0, 3); }

void parseLine(const char* line, Settings& s) {
    int v = 0;
    if (std::sscanf(line, "resolution_scale=%d", &v) == 1) s.resolutionScale = std::clamp(v, 50, 100);
    else if (std::sscanf(line, "refresh_rate=%d", &v) == 1) s.refreshRate = v > 0 ? std::clamp(v, 30, 240) : 0;
    else if (std::sscanf(line, "antialiasing=%d", &v) == 1) s.antialiasing = v >= 4 ? 4 : v >= 2 ? 2 : 0;
    else if (std::sscanf(line, "aspect_ratio=%d", &v) == 1) s.aspect = std::clamp(v, 0, ASPECT_COUNT - 1);
    else if (std::sscanf(line, "brightness=%d", &v) == 1) s.brightness = std::clamp(v, 0, 100);
    else if (std::sscanf(line, "vsync=%d", &v) == 1) s.vsync = std::clamp(v, 0, 2);
    else if (std::sscanf(line, "fps_cap=%d", &v) == 1) s.fpsCap = v > 0 ? std::clamp(v, 15, 240) : 0;
    else if (std::sscanf(line, "landscape_detail=%d", &v) == 1) s.landscape = clampQ(v);
    else if (std::sscanf(line, "model_detail=%d", &v) == 1) s.model = clampQ(v);
    else if (std::sscanf(line, "texture_detail=%d", &v) == 1) s.texture = clampQ(v);
    else if (std::sscanf(line, "shadow_quality=%d", &v) == 1) s.shadow = clampQ(v);
    else if (std::sscanf(line, "effects_quality=%d", &v) == 1) s.effects = clampQ(v);
    else if (std::sscanf(line, "draw_distance=%d", &v) == 1) s.drawDistance = std::clamp(v, 30, 150);
    else if (std::sscanf(line, "glow=%d", &v) == 1) s.glow = v != 0;
    else if (std::sscanf(line, "water_reflection=%d", &v) == 1) s.water = v != 0;
    else if (std::sscanf(line, "weather=%d", &v) == 1) s.weather = v != 0;
}

void save() {
    if (FILE* f = std::fopen(path().c_str(), "w")) {
        const Settings& s = g_settings;
        std::fprintf(f, "resolution_scale=%d\nrefresh_rate=%d\nantialiasing=%d\naspect_ratio=%d\nlandscape_detail=%d\nmodel_detail=%d\ntexture_detail=%d\n"
                        "shadow_quality=%d\neffects_quality=%d\nbrightness=%d\nvsync=%d\ndraw_distance=%d\nglow=%d\nwater_reflection=%d\nweather=%d\nfps_cap=%d\n",
                     s.resolutionScale, s.refreshRate, s.antialiasing, s.aspect, s.landscape, s.model, s.texture, s.shadow, s.effects, s.brightness, s.vsync,
                     s.drawDistance, s.glow ? 1 : 0, s.water ? 1 : 0, s.weather ? 1 : 0, s.fpsCap);
        std::fclose(f);
    }
}

void applyToDef(uint32_t def, const Settings& s) {
    if (!def) return;
    auto it = g_originals.find(def);
    if (it == g_originals.end()) {
        Original o{rdf32(def + kLODErrorTolerance), rdf32(def + kCharacterLODErrorTolerance), rdf32(def + kDrawDistanceMultiplier),
                   rdf32(def + kDrawDistanceMinimum), rdf32(def + kDrawDistanceMaximum), rdf32(def + kRepeatedMeshDrawDistanceFactor),
                   rdf32(def + kZSpriteDrawDistanceMultiplier), rd8(def + kEnableGlow), rd8(def + kEnableWaterReflection), rd8(def + kEnableWeatherEffects)};
        it = g_originals.emplace(def, o).first;
        log("gfx: def 0x%08X: lod %.2f/%.2f draw x%.2f [%.0f..%.0f] repeated %.2f zsprite %.2f shadow %u glow %u water %u weather %u", def, o.lod, o.charLod,
            o.ddMul, o.ddMin, o.ddMax, o.repeated, o.zsprite, rd32(def + kShadowBufferSize), o.glow, o.water, o.weather);
    }
    const Original& o = it->second;
    const float dd = s.drawDistance / 100.0f;
    wrf32(def + kDrawDistanceMultiplier, o.ddMul * dd);
    wrf32(def + kDrawDistanceMaximum, o.ddMax * dd);
    wrf32(def + kDrawDistanceMinimum, o.ddMin * dd);
    wrf32(def + kRepeatedMeshDrawDistanceFactor, o.repeated * dd);
    wrf32(def + kZSpriteDrawDistanceMultiplier, o.zsprite * dd);
    wr8(def + kEnableGlow, o.glow && s.glow ? 1 : 0);
    wr8(def + kEnableWaterReflection, o.water && s.water ? 1 : 0);
    wr8(def + kEnableWeatherEffects, o.weather && s.weather ? 1 : 0);
}

// FABLE_GFX_DEFAULTS="key=value,key=value" (graphics.txt keys) seeds the settings when no file exists
void seedFromEnv() {
    const char* d = std::getenv("FABLE_GFX_DEFAULTS");
    if (!d || !*d) return;
    std::string all(d);
    size_t p = 0;
    while (p < all.size()) {
        size_t q = all.find(',', p);
        if (q == std::string::npos) q = all.size();
        parseLine(all.substr(p, q - p).c_str(), g_settings);
        p = q + 1;
    }
    g_seeded = true;
}

}  // namespace

const char* qualityName(int q) {
    static const char* names[] = {"Low", "Normal", "High", "Highest"};
    return names[clampQ(q)];
}

const char* aspectName(int a) {
    static const char* names[] = {"auto", "4:3", "16:9", "16:10", "3:2", "5:4"};
    return names[std::clamp(a, 0, ASPECT_COUNT - 1)];
}

void aspectValue(int a, int w, int h, float& x, float& y) {
    static const float pairs[][2] = {{0, 0}, {4, 3}, {16, 9}, {16, 10}, {3, 2}, {5, 4}};
    a = std::clamp(a, 0, ASPECT_COUNT - 1);
    if (a != ASPECT_AUTO) { x = pairs[a][0], y = pairs[a][1]; return; }
    int g = w > 0 && h > 0 ? std::gcd(w, h) : 0;  // the window's own ratio, reduced (720x480 -> 3:2)
    x = g ? static_cast<float>(w / g) : 4.0f, y = g ? static_cast<float>(h / g) : 3.0f;
}

float gammaExponent(int brightness) { return std::exp2((50 - std::clamp(brightness, 0, 100)) / 50.0f); }  // 100: 0.5 (brighter), 0: 2 (darker)

void load() {
    if (g_loaded) return;
    g_loaded = true;
    if (FILE* f = std::fopen(path().c_str(), "r")) {
        char line[128];
        while (std::fgets(line, sizeof line, f)) parseLine(line, g_settings);
        std::fclose(f);
        g_seeded = true;
    } else {
        if (const char* e = std::getenv("FABLE_VSYNC")) g_settings.vsync = std::clamp(std::atoi(e), 0, 2);
        seedFromEnv();
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
    g_seeded = true;
    g_pending = true;
    save();
}

bool seeded() { std::lock_guard<std::mutex> l(g_lock); load(); return g_seeded; }

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

// 0xB24690: bool __thiscall CEngine::SetVideoOptions(const CEngineVideoOptions* o)
extern "C" void host_video_options(Ctx* c) {
    using namespace host::gfx;
    const uint32_t engine = c->ecx, opts = rd32(c->esp + 4);
    Settings s;
    {
        std::lock_guard<std::mutex> l(g_lock);
        load();
        g_engine = engine;
        if (!g_optsCopy) g_optsCopy = gcalloc(1, 64);
        if (opts != g_optsCopy) std::memcpy(gp(g_optsCopy), gp(opts), 52), g_gameW = rd32(opts + kWidth), g_gameH = rd32(opts + kHeight);
        if (!g_seeded) {  // first run without a file or preset: start from the game's own choice
            g_settings.landscape = clampQ(static_cast<int>(rd32(opts + kLandscape)));
            g_settings.model = clampQ(static_cast<int>(rd32(opts + kMesh)));
            g_settings.texture = clampQ(static_cast<int>(rd32(opts + kTexture)));
            g_settings.shadow = clampQ(static_cast<int>(rd32(opts + kShadow)));
            g_settings.effects = clampQ(static_cast<int>(rd32(opts + kEffects)));
            g_seeded = true;
            save();
        }
        s = g_settings;
    }
    HLOG(1, "gfx: SetVideoOptions(engine 0x%08X): game asks %ux%u, quality %u %u %u %u %u, aa %u", engine, rd32(opts), rd32(opts + 4), rd32(opts + kLandscape),
         rd32(opts + kMesh), rd32(opts + kTexture), rd32(opts + kShadow), rd32(opts + kEffects), rd32(opts + kAntialias));
    const bool hostGfx = std::getenv("FABLE_HOST_GFX") != nullptr;
    if (hostGfx) {
        // the host's values replace the game's (its Video Options page is inert); the game's
        // resolution is the window, the host renders a 3:2-preserving fraction of it
        // native = the window (the game's own choice may be an older profile's 640x480, which
        // would be stretched over a 3:2 screen); a re-apply must not compound the scale
        uint32_t w = g_gameW, h = g_gameH;
        int sw0 = 0, sh0 = 0;
        if (w32::screenSize(sw0, sh0) && sw0 > 0 && sh0 > 0) w = static_cast<uint32_t>(sw0), h = static_cast<uint32_t>(sh0);
        const uint32_t sw = std::max(320u, (w * static_cast<uint32_t>(s.resolutionScale) / 100) & ~3u);
        const uint32_t sh = std::max(200u, (h * static_cast<uint32_t>(s.resolutionScale) / 100) & ~3u);
        wr32(g_optsCopy + kWidth, sw), wr32(g_optsCopy + kHeight, sh);
        wr32(g_optsCopy + kLandscape, static_cast<uint32_t>(s.landscape));
        wr32(g_optsCopy + kMesh, static_cast<uint32_t>(s.model));
        wr32(g_optsCopy + kTexture, static_cast<uint32_t>(s.texture));
        wr32(g_optsCopy + kShadow, static_cast<uint32_t>(s.shadow));
        wr32(g_optsCopy + kEffects, static_cast<uint32_t>(s.effects));
        wr32(g_optsCopy + kAntialias, static_cast<uint32_t>(s.antialiasing));  // ESurfaceMultisampleType = D3DMULTISAMPLE_TYPE
        if (s.resolutionScale == 100) wr32(g_optsCopy + kWidth, w), wr32(g_optsCopy + kHeight, h);
        float ax = 0, ay = 0;
        aspectValue(s.aspect, static_cast<int>(w), static_cast<int>(h), ax, ay);
        wrf32(g_optsCopy + kAspectX, ax), wrf32(g_optsCopy + kAspectY, ay);
        if (s.refreshRate) wr32(g_optsCopy + kRefresh, static_cast<uint32_t>(s.refreshRate));
        wr32(g_optsCopy + kVsync, s.vsync ? 1 : 0);
        HLOG(1, "gfx: host settings: %ux%u (%d%%) %.0f:%.0f %u Hz aa %d vsync %d, landscape %s, model %s, texture %s, shadow %s, effects %s, brightness %d, draw %d%%, glow %d, water %d, weather %d",
             rd32(g_optsCopy), rd32(g_optsCopy + 4), s.resolutionScale, ax, ay, rd32(g_optsCopy + kRefresh), s.antialiasing, s.vsync, qualityName(s.landscape),
             qualityName(s.model), qualityName(s.texture), qualityName(s.shadow), qualityName(s.effects), s.brightness, s.drawDistance, s.glow, s.water, s.weather);
        wr32(c->esp + 4, g_optsCopy);
    }
    for (uint32_t slot : kDefSlots) applyToDef(rd32(engine + slot), s);
    // the renderer identifies shadow passes by the buffer size the shadow-quality def names
    const uint32_t used = hostGfx ? g_optsCopy : opts;
    const uint32_t shadowDef = rd32(engine + kDefSlots[clampQ(static_cast<int>(rd32(used + kShadow)))]);
    g_shadowSize = shadowDef ? rd32(shadowDef + kShadowBufferSize) : 0;
    F_00B24690_orig(c);
    if (hostGfx) d3d9ApplyGamma(gammaExponent(s.brightness));  // the device exists (or was reset) now
}

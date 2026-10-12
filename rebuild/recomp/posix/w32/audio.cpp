// The audio output: one SDL device (stereo float), mixing every registered source.
#include "w32sdl.hpp"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>

namespace w32 {
const std::string& dataDir();  // ole.cpp
namespace {
std::recursive_mutex g_lock;
std::vector<std::shared_ptr<AudioSource>> g_sources;
SDL_AudioDeviceID g_dev;
int g_rate = 48000;
std::atomic<float> g_volume{-1.0f};  // the master volume (FABLE_RECOMP_DATA/volume.txt), -1: not loaded

std::string volumeFile() { return dataDir() + "/volume.txt"; }
float loadVolume() {
    float v = 1.0f;
    if (FILE* f = std::fopen(volumeFile().c_str(), "r")) {
        if (std::fscanf(f, "%f", &v) != 1) v = 1.0f;
        std::fclose(f);
    }
    return std::clamp(v, 0.0f, 1.0f);
}

void callback(void*, Uint8* stream, int len) {
    auto* out = reinterpret_cast<float*>(stream);
    const int frames = len / static_cast<int>(2 * sizeof(float));
    std::memset(stream, 0, static_cast<size_t>(len));
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto& s : g_sources) s->mix(out, frames, g_rate);
    const float vol = masterVolume();
    for (int i = 0; i < frames * 2; ++i) out[i] = std::clamp(out[i] * vol, -1.0f, 1.0f);
}

void ensureDevice() {
    if (g_dev) return;
    if (!SDL_WasInit(SDL_INIT_AUDIO) && SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        std::fprintf(stderr, "SDL audio init failed: %s\n", SDL_GetError());
        return;
    }
    SDL_AudioSpec want{}, have{};
    want.freq = 48000;
    want.format = AUDIO_F32SYS;
    want.channels = 2;
    want.samples = 1024;
    want.callback = callback;
    g_dev = SDL_OpenAudioDevice(nullptr, 0, &want, &have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    if (!g_dev) {
        std::fprintf(stderr, "SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
        return;
    }
    g_rate = have.freq;
    SDL_PauseAudioDevice(g_dev, 0);
}
}  // namespace

float masterVolume() {
    float v = g_volume.load();
    if (v < 0) { v = loadVolume(); g_volume.store(v); }
    return v;
}
void setMasterVolume(float v) {
    v = std::clamp(v, 0.0f, 1.0f);
    g_volume.store(v);
    if (FILE* f = std::fopen(volumeFile().c_str(), "w")) { std::fprintf(f, "%.2f\n", v); std::fclose(f); }
}

std::recursive_mutex& audioLock() { return g_lock; }
int audioRate() { return g_rate; }
void audioAdd(std::shared_ptr<AudioSource> s) {
    ensureDevice();
    std::lock_guard<std::recursive_mutex> l(g_lock);
    g_sources.push_back(std::move(s));
}
void audioRemove(AudioSource* s) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    g_sources.erase(std::remove_if(g_sources.begin(), g_sources.end(), [s](auto& p) { return p.get() == s; }), g_sources.end());
}

}  // namespace w32

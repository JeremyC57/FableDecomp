// The audio output: one SDL device (stereo float), mixing every registered source.
#include "w32sdl.hpp"

#include <algorithm>
#include <cstring>
#include <vector>

namespace w32 {
namespace {
std::recursive_mutex g_lock;
std::vector<std::shared_ptr<AudioSource>> g_sources;
SDL_AudioDeviceID g_dev;
int g_rate = 48000;

void callback(void*, Uint8* stream, int len) {
    auto* out = reinterpret_cast<float*>(stream);
    const int frames = len / static_cast<int>(2 * sizeof(float));
    std::memset(stream, 0, static_cast<size_t>(len));
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto& s : g_sources) s->mix(out, frames, g_rate);
    for (int i = 0; i < frames * 2; ++i) out[i] = std::clamp(out[i], -1.0f, 1.0f);
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

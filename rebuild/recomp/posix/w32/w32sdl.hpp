// Services shared by the SDL-backed parts of the Win32 layer.
#pragma once
#include <SDL.h>

#include <functional>
#include <memory>

#include "w32.hpp"

namespace w32 {

// user.cpp
void addSdlEventHook(std::function<void(const SDL_Event&)> fn);  // sees every SDL event (on the SDL thread)
void pumpSdlEvents();                                             // no-op off the SDL thread
HWND activeWindow();

// audio.cpp: one SDL output device; sources add stereo float samples at audioRate().
struct AudioSource {
    virtual ~AudioSource() = default;
    virtual void mix(float* out, int frames, int rate) = 0;  // called on the audio thread, under the mixer lock
};
void audioAdd(std::shared_ptr<AudioSource> s);
void audioRemove(AudioSource* s);
int audioRate();
std::recursive_mutex& audioLock();  // hold while changing state a source reads in mix()

// Linear-interpolating PCM reader used by waveOut and DirectSound: 8/16-bit, 1/2 channels.
struct PcmFormat { int rate = 44100, channels = 2, bits = 16; };
inline void pcmFrame(const PcmFormat& f, const uint8_t* p, float& l, float& r) {
    if (f.bits == 16) {
        const int16_t* s = reinterpret_cast<const int16_t*>(p);
        l = s[0] / 32768.0f;
        r = f.channels > 1 ? s[1] / 32768.0f : l;
    } else {
        l = (p[0] - 128) / 128.0f;
        r = f.channels > 1 ? (p[1] - 128) / 128.0f : l;
    }
}

}  // namespace w32

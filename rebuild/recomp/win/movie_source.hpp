// A movie file decoder for the host's DirectShow graph (video.cpp): Media Foundation on
// Windows (movie_mf.cpp), FFmpeg on Linux and Android (posix/movie_ffmpeg.cpp).
#pragma once
#include <cstdint>
#include <memory>
#include <vector>

namespace host::movie {

struct Source {
    virtual ~Source() = default;
    // Filled by open: frame size, whether each stream exists, duration (100 ns units) and
    // the PCM format of the audio (always 16-bit).
    uint32_t width = 0, height = 0;
    bool hasVideo = false, hasAudio = false;
    int64_t duration = 0;
    uint32_t audioRate = 0, audioChannels = 0;
    const char* videoFormat = "";  // for the log

    enum Kind { Video, Audio, End };
    // Next decoded unit: a video frame as a bottom-up RGB24 DIB (rows of `stride` bytes,
    // stride = (width * 3 + 3) & ~3) or a block of interleaved 16-bit PCM. `time` is the
    // presentation time in 100 ns units. End means both streams are finished (or failed).
    virtual Kind read(int64_t& time, std::vector<uint8_t>& data) = 0;
    virtual void seek(int64_t time) = 0;
    uint32_t stride() const { return (width * 3 + 3) & ~3u; }
};

// Opens a movie (a Windows path); null if it cannot be decoded.
std::unique_ptr<Source> open(const wchar_t* path);

}  // namespace host::movie

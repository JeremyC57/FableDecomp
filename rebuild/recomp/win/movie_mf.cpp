// Movie decoding with Media Foundation (Windows; see movie_source.hpp).
#ifndef FABLE_POSIX
#include "host.hpp"
#include "movie_source.hpp"

#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include <mutex>

namespace host::movie {
namespace {

uint8_t clamp8(int v) { return static_cast<uint8_t>(v < 0 ? 0 : v > 255 ? 255 : v); }
void yuvToBgr(int y, int u, int v, uint8_t* o) {
    const int c = y - 16, d = u - 128, e = v - 128;
    o[2] = clamp8((298 * c + 409 * e + 128) >> 8);
    o[1] = clamp8((298 * c - 100 * d - 208 * e + 128) >> 8);
    o[0] = clamp8((298 * c + 516 * d + 128) >> 8);
}

struct MfSource final : Source {
    IMFSourceReader* reader = nullptr;
    enum { SRC_RGB32, SRC_NV12, SRC_YUY2 } srcFormat = SRC_RGB32;
    bool videoEos = false, audioEos = false;

    ~MfSource() override {
        if (reader) reader->Release();
    }

    bool configureVideo() {
        const GUID tries[] = {MFVideoFormat_RGB32, MFVideoFormat_NV12, MFVideoFormat_YUY2};
        for (const GUID& sub : tries) {
            IMFMediaType* t = nullptr;
            MFCreateMediaType(&t);
            t->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
            t->SetGUID(MF_MT_SUBTYPE, sub);
            const HRESULT hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, t);
            t->Release();
            if (SUCCEEDED(hr)) {
                srcFormat = sub == MFVideoFormat_RGB32 ? SRC_RGB32 : sub == MFVideoFormat_NV12 ? SRC_NV12 : SRC_YUY2;
                videoFormat = srcFormat == SRC_RGB32 ? "RGB32" : srcFormat == SRC_NV12 ? "NV12" : "YUY2";
                IMFMediaType* cur = nullptr;
                reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &cur);
                UINT32 w = 0, h = 0;
                MFGetAttributeSize(cur, MF_MT_FRAME_SIZE, &w, &h);
                width = w, height = h;
                cur->Release();
                return width && height;
            }
        }
        return false;
    }
    bool configureAudio() {
        IMFMediaType* t = nullptr;
        MFCreateMediaType(&t);
        t->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
        t->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
        t->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
        HRESULT hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, t);
        t->Release();
        if (FAILED(hr)) return false;
        IMFMediaType* cur = nullptr;
        reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &cur);
        WAVEFORMATEX* w = nullptr;
        UINT32 size = 0;
        hr = MFCreateWaveFormatExFromMFMediaType(cur, &w, &size);
        cur->Release();
        if (FAILED(hr)) return false;
        audioRate = w->nSamplesPerSec;
        audioChannels = w->nChannels;
        CoTaskMemFree(w);
        return true;
    }

    // Decoded frame -> RGB24 bottom-up DIB.
    void convertFrame(IMFSample* s, std::vector<uint8_t>& out) {
        IMFMediaBuffer* b = nullptr;
        if (FAILED(s->ConvertToContiguousBuffer(&b))) return;
        BYTE* p = nullptr;
        DWORD len = 0;
        b->Lock(&p, nullptr, &len);
        const UINT32 w = width, h = height, st = stride();
        out.assign(static_cast<size_t>(st) * h, 0);
        if (srcFormat == SRC_RGB32) {
            const DWORD pitch = len / h >= w * 4 ? len / h : w * 4;
            for (UINT32 y = 0; y < h && (y + 1) * pitch <= len; ++y) {
                const uint8_t* src = p + y * pitch;
                uint8_t* dst = out.data() + static_cast<size_t>(h - 1 - y) * st;
                for (UINT32 x = 0; x < w; ++x) std::memcpy(dst + 3 * x, src + 4 * x, 3);
            }
        } else if (srcFormat == SRC_NV12) {
            const DWORD pitch = w;  // contiguous NV12: Y plane then interleaved UV
            if (len >= pitch * h * 3 / 2)
                for (UINT32 y = 0; y < h; ++y) {
                    const uint8_t* yr = p + y * pitch;
                    const uint8_t* uv = p + pitch * h + (y / 2) * pitch;
                    uint8_t* dst = out.data() + static_cast<size_t>(h - 1 - y) * st;
                    for (UINT32 x = 0; x < w; ++x) yuvToBgr(yr[x], uv[x & ~1u], uv[x | 1u], dst + 3 * x);
                }
        } else {
            const DWORD pitch = w * 2;
            if (len >= pitch * h)
                for (UINT32 y = 0; y < h; ++y) {
                    const uint8_t* r = p + y * pitch;
                    uint8_t* dst = out.data() + static_cast<size_t>(h - 1 - y) * st;
                    for (UINT32 x = 0; x < w; ++x) yuvToBgr(r[2 * x], r[(4 * (x / 2)) + 1], r[(4 * (x / 2)) + 3], dst + 3 * x);
                }
        }
        b->Unlock();
        b->Release();
    }

    Kind read(int64_t& time, std::vector<uint8_t>& data) override {
        for (;;) {
            if (videoEos && audioEos) return End;
            DWORD stream = 0, flags = 0;
            LONGLONG ts = 0;
            IMFSample* s = nullptr;
            const HRESULT hr = reader->ReadSample(MF_SOURCE_READER_ANY_STREAM, 0, &stream, &flags, &ts, &s);
            if (FAILED(hr)) { videoEos = audioEos = true; return End; }
            // map the actual stream index to video/audio by its major type
            IMFMediaType* t = nullptr;
            bool isVideo = false;
            if (SUCCEEDED(reader->GetCurrentMediaType(stream, &t))) {
                GUID major{};
                t->GetGUID(MF_MT_MAJOR_TYPE, &major);
                isVideo = major == MFMediaType_Video;
                t->Release();
            }
            if (flags & MF_SOURCE_READERF_ERROR) videoEos = audioEos = true;
            if (flags & MF_SOURCE_READERF_ENDOFSTREAM) (isVideo ? videoEos : audioEos) = true;
            if (!s) continue;
            time = ts;
            if (isVideo) {
                convertFrame(s, data);
                s->Release();
                return Video;
            }
            IMFMediaBuffer* b = nullptr;
            data.clear();
            if (SUCCEEDED(s->ConvertToContiguousBuffer(&b))) {
                BYTE* p = nullptr;
                DWORD n = 0;
                b->Lock(&p, nullptr, &n);
                data.assign(p, p + n);
                b->Unlock();
                b->Release();
            }
            s->Release();
            return Audio;
        }
    }

    void seek(int64_t to) override {
        videoEos = !hasVideo, audioEos = !hasAudio;
        PROPVARIANT v;
        PropVariantInit(&v);
        v.vt = VT_I8;
        v.hVal.QuadPart = to;
        reader->SetCurrentPosition(GUID_NULL, v);
    }
};

}  // namespace

std::unique_ptr<Source> open(const wchar_t* file) {
    static std::once_flag once;
    std::call_once(once, [] { MFStartup(MF_VERSION, MFSTARTUP_FULL); });
    wchar_t full[MAX_PATH * 2];
    GetFullPathNameW(file, MAX_PATH * 2, full, nullptr);
    auto s = std::make_unique<MfSource>();
    IMFAttributes* a = nullptr;
    MFCreateAttributes(&a, 2);
    a->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    const HRESULT hr = MFCreateSourceReaderFromURL(full, a, &s->reader);
    a->Release();
    if (FAILED(hr)) {
        log("movie: cannot open %s (0x%08lX)", narrow(full).c_str(), static_cast<unsigned long>(hr));
        return nullptr;
    }
    s->reader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, FALSE);
    s->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
    s->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
    s->hasVideo = s->configureVideo();
    s->hasAudio = s->configureAudio();
    if (!s->hasVideo) s->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_VIDEO_STREAM, FALSE);
    if (!s->hasAudio) s->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, FALSE);
    s->videoEos = !s->hasVideo, s->audioEos = !s->hasAudio;
    PROPVARIANT v;
    PropVariantInit(&v);
    if (SUCCEEDED(s->reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &v))) s->duration = v.uhVal.QuadPart;
    PropVariantClear(&v);
    if (!s->hasVideo && !s->hasAudio) return nullptr;
    return s;
}

}  // namespace host::movie
#endif  // FABLE_POSIX

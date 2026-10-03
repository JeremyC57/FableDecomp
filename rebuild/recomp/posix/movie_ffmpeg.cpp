// Movie decoding with FFmpeg (Linux / Android; see win/movie_source.hpp). The game's
// movies are WMV (ASF with WMV1-3 video and WMA audio), which the platform decoders do
// not handle on Android.
#include "host.hpp"
#include "movie_source.hpp"
#include "w32/w32.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#include <deque>

namespace host::movie {
namespace {

struct Unit {
    Source::Kind kind;
    int64_t time;
    std::vector<uint8_t> data;
};

struct FfSource final : Source {
    AVFormatContext* fmt = nullptr;
    int vIdx = -1, aIdx = -1;
    AVCodecContext *vc = nullptr, *ac = nullptr;
    SwsContext* sws = nullptr;
    SwrContext* swr = nullptr;
    AVPacket* pkt = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    std::deque<Unit> pending;
    bool drained = false;

    ~FfSource() override {
        sws_freeContext(sws);
        swr_free(&swr);
        avcodec_free_context(&vc);
        avcodec_free_context(&ac);
        avformat_close_input(&fmt);
        av_packet_free(&pkt);
        av_frame_free(&frame);
    }

    static int64_t to100ns(int64_t ts, AVRational tb) { return ts == AV_NOPTS_VALUE ? 0 : av_rescale_q(ts, tb, AVRational{1, 10000000}); }

    AVCodecContext* openCodec(int idx) {
        const AVCodecParameters* par = fmt->streams[idx]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(par->codec_id);
        if (!codec) return nullptr;
        AVCodecContext* c = avcodec_alloc_context3(codec);
        avcodec_parameters_to_context(c, par);
        c->thread_count = 2;
        if (avcodec_open2(c, codec, nullptr) < 0) { avcodec_free_context(&c); return nullptr; }
        return c;
    }

    void videoFrame() {
        Unit u{Video, to100ns(frame->best_effort_timestamp, fmt->streams[vIdx]->time_base), {}};
        const int st = static_cast<int>(stride());
        u.data.assign(static_cast<size_t>(st) * height, 0);
        sws = sws_getCachedContext(sws, frame->width, frame->height, static_cast<AVPixelFormat>(frame->format), static_cast<int>(width),
                                   static_cast<int>(height), AV_PIX_FMT_BGR24, SWS_BILINEAR, nullptr, nullptr, nullptr);
        if (!sws) return;
        // Bottom-up: start at the last row with a negative stride.
        uint8_t* dst[4] = {u.data.data() + static_cast<size_t>(st) * (height - 1), nullptr, nullptr, nullptr};
        int dstStride[4] = {-st, 0, 0, 0};
        sws_scale(sws, frame->data, frame->linesize, 0, frame->height, dst, dstStride);
        pending.push_back(std::move(u));
    }

    void audioFrame() {
        Unit u{Audio, to100ns(frame->best_effort_timestamp, fmt->streams[aIdx]->time_base), {}};
        const int outCount = swr_get_out_samples(swr, frame->nb_samples);
        u.data.resize(static_cast<size_t>(outCount > 0 ? outCount : 0) * 2 * audioChannels);
        uint8_t* out[1] = {u.data.data()};
        const int got = swr_convert(swr, out, outCount, const_cast<const uint8_t**>(frame->extended_data), frame->nb_samples);
        if (got <= 0) return;
        u.data.resize(static_cast<size_t>(got) * 2 * audioChannels);
        pending.push_back(std::move(u));
    }

    void receive(AVCodecContext* c, bool video) {
        while (avcodec_receive_frame(c, frame) == 0) {
            if (video) videoFrame();
            else audioFrame();
            av_frame_unref(frame);
        }
    }

    Kind read(int64_t& time, std::vector<uint8_t>& data) override {
        while (pending.empty()) {
            if (drained) return End;
            const int r = av_read_frame(fmt, pkt);
            if (r < 0) {
                // end of file: flush both decoders
                if (vc) { avcodec_send_packet(vc, nullptr); receive(vc, true); }
                if (ac) { avcodec_send_packet(ac, nullptr); receive(ac, false); }
                drained = true;
                continue;
            }
            if (pkt->stream_index == vIdx && vc) {
                if (avcodec_send_packet(vc, pkt) == 0) receive(vc, true);
            } else if (pkt->stream_index == aIdx && ac) {
                if (avcodec_send_packet(ac, pkt) == 0) receive(ac, false);
            }
            av_packet_unref(pkt);
        }
        Unit& u = pending.front();
        time = u.time;
        data = std::move(u.data);
        const Kind k = u.kind;
        pending.pop_front();
        return k;
    }

    void seek(int64_t to) override {
        av_seek_frame(fmt, -1, av_rescale(to, AV_TIME_BASE, 10000000), AVSEEK_FLAG_BACKWARD);
        if (vc) avcodec_flush_buffers(vc);
        if (ac) avcodec_flush_buffers(ac);
        pending.clear();
        drained = false;
    }
};

}  // namespace

std::unique_ptr<Source> open(const wchar_t* file) {
    const std::string path = w32::toPosixPath(file);
    auto s = std::make_unique<FfSource>();
    if (avformat_open_input(&s->fmt, path.c_str(), nullptr, nullptr) < 0) {
        log("movie: cannot open %s", path.c_str());
        return nullptr;
    }
    if (avformat_find_stream_info(s->fmt, nullptr) < 0) return nullptr;
    s->vIdx = av_find_best_stream(s->fmt, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    s->aIdx = av_find_best_stream(s->fmt, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (s->vIdx >= 0 && (s->vc = s->openCodec(s->vIdx))) {
        s->hasVideo = true;
        s->width = static_cast<uint32_t>(s->vc->width);
        s->height = static_cast<uint32_t>(s->vc->height);
        s->videoFormat = avcodec_get_name(s->vc->codec_id);
    }
    if (s->aIdx >= 0 && (s->ac = s->openCodec(s->aIdx))) {
        s->audioRate = static_cast<uint32_t>(s->ac->sample_rate);
        s->audioChannels = s->ac->ch_layout.nb_channels >= 2 ? 2 : 1;
        AVChannelLayout outLayout;
        av_channel_layout_default(&outLayout, static_cast<int>(s->audioChannels));
        if (swr_alloc_set_opts2(&s->swr, &outLayout, AV_SAMPLE_FMT_S16, s->ac->sample_rate, &s->ac->ch_layout, s->ac->sample_fmt, s->ac->sample_rate, 0,
                                nullptr) == 0 &&
            swr_init(s->swr) == 0)
            s->hasAudio = true;
    }
    if (s->fmt->duration != AV_NOPTS_VALUE) s->duration = av_rescale(s->fmt->duration, 10000000, AV_TIME_BASE);
    if (!s->hasVideo && !s->hasAudio) return nullptr;
    return s;
}

}  // namespace host::movie

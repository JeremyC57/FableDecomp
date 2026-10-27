// libvorbis::_preextrapolate_helper(vorbis_dsp_state* v) -- libvorbis block.c: once enough PCM is buffered (pcm_current - centerW > 2*order), run a 32-order LPC extrapolation backwards over every channel to pre-roll the start of the stream: copy the channel reversed into an alloca work buffer, vorbis_lpc_from_data on it, vorbis_lpc_predict the centerW samples, and copy it back reversed; sets v->preextrapolate
// Grade: DIFFER(248v246) -- LTCG private register convention: retail takes v in ESI (never loaded); ours loads it from ecx (lane gap: LTCG)
// Leaf: @_preextrapolate_helper@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8F960, 246 bytes, ebp frame with two _alloca calls (_chkstk 0x00BFEA30). libpk's statically
// linked Xiph libvorbis 20030909 (lib/block.c). The LTCG link gave this static a private register convention: v arrives in ESI
// (never loaded in the body) -- a convention VC7.1 cannot be asked for.
// Identity: order 32 (alloca 0x80), the pcm_current - centerW > 64 guard, the reversed copy loops and the lpc_from_data /
// lpc_predict pair are block.c _preextrapolate_helper exactly. Catalogued as "Audio_ConvolutionStep" (descriptive).
// Callees: _chkstk 0x00BFEA30 (alloca), vorbis_lpc_from_data @ 0x00C9E5D0, vorbis_lpc_predict @ 0x00C9E8F0 (both __fastcall).
#include <malloc.h>
typedef __int64 ogg_int64_t;
typedef struct vorbis_info {
    int version;
    int channels;
    long rate;
    long bitrate_upper;
    long bitrate_nominal;
    long bitrate_lower;
    long bitrate_window;
    void* codec_setup;
} vorbis_info;
typedef struct vorbis_dsp_state {
    int analysisp;
    vorbis_info* vi;
    float** pcm;
    float** pcmret;
    int pcm_storage;
    int pcm_current;
    int pcm_returned;
    int preextrapolate;
    int eofflag;
    long lW;
    long W;
    long nW;
    long centerW;
    ogg_int64_t granulepos;
    ogg_int64_t sequence;
    ogg_int64_t glue_bits;
    ogg_int64_t time_bits;
    ogg_int64_t floor_bits;
    ogg_int64_t res_bits;
    void* backend_state;
} vorbis_dsp_state;

extern "C" float __fastcall vorbis_lpc_from_data(float* data, float* lpci, int n, int m);                 // 0x00C9E5D0
extern "C" void __fastcall vorbis_lpc_predict(float* coeff, float* prime, int m, float* data, long n);    // 0x00C9E8F0

extern "C" void __fastcall _preextrapolate_helper(vorbis_dsp_state* v)
{
    int i;
    int order = 32;
    float* lpc = (float*)_alloca(order * sizeof(*lpc));
    float* work = (float*)_alloca(v->pcm_current * sizeof(*work));
    long j;
    v->preextrapolate = 1;

    if (v->pcm_current - v->centerW > order * 2) { /* safety */
        for (i = 0; i < v->vi->channels; i++) {
            /* need to run the extrapolation in reverse! */
            for (j = 0; j < v->pcm_current; j++)
                work[j] = v->pcm[i][v->pcm_current - j - 1];

            /* prime as above */
            vorbis_lpc_from_data(work, lpc, v->pcm_current - v->centerW, order);

            /* run the predictor filter */
            vorbis_lpc_predict(lpc, work + v->pcm_current - v->centerW - order,
                               order,
                               work + v->pcm_current - v->centerW,
                               v->centerW);

            for (j = 0; j < v->pcm_current; j++)
                v->pcm[i][v->pcm_current - j - 1] = work[j];
        }
    }
}

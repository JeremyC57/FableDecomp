// libvorbis::floater_interpolate(bitrate_manager_state* bm, vorbis_info* vi, double desired_rate) -- pick the average-bitrate bin whose rate is closest to desired_rate, starting from rint(avgfloat)
// Grade: DIFFER(215v211) -- LTCG register argument: retail takes bm in ESI (vi and desired_rate on the stack, `ret 0Ch`); ours bm/vi in ecx/edx
// Retail: Fable.exe @ 0x00C9CB80, 211 bytes. libvorbis 1.0.1 lib/bitrate.c (static). Callees: floor import [0x01440100], __ftol2 0x00BFEA70. Data: 0.5 @ 0x012316F0, 2^32 @ 0x012387C8 (the unsigned avg_sampleacc). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: floater_interpolate
typedef unsigned int ogg_uint32_t;
extern "C" __declspec(dllimport) double __cdecl floor(double x);   // [0x01440100]
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
#define rint(x) (floor((x) + 0.5f))
typedef struct vorbis_info { int version; int channels; long rate; long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window; void* codec_setup; } vorbis_info;
typedef struct bitrate_manager_state {
    ogg_uint32_t* queue_binned; ogg_uint32_t* queue_actual; int queue_size; int queue_head; int queue_bins;
    long* avg_binacc; int avg_center; int avg_tail; ogg_uint32_t avg_centeracc; ogg_uint32_t avg_sampleacc;
    ogg_uint32_t avg_sampledesired; ogg_uint32_t avg_centerdesired;
    int* minmax_binstack; int* minmax_posstack; int* minmax_limitstack; long minmax_stackptr;
    long minmax_acctotal; int minmax_tail; ogg_uint32_t minmax_sampleacc; ogg_uint32_t minmax_sampledesired;
    int next_to_flush; int last_to_flush; double avgfloat; void* packetbuffers; void* packets;
} bitrate_manager_state;
extern "C" long __fastcall floater_interpolate(bitrate_manager_state* bm, vorbis_info* vi, double desired_rate)
{
    int bin = rint(bm->avgfloat);
    double lobitrate, hibitrate;

    lobitrate = (double)(bm->avg_binacc[bin] * 8) / bm->avg_sampleacc * vi->rate;
    while (lobitrate > desired_rate && bin > 0) {
        bin--;
        lobitrate = (double)(bm->avg_binacc[bin] * 8) / bm->avg_sampleacc * vi->rate;
    }

    if (bin + 1 < bm->queue_bins) {
        hibitrate = (double)(bm->avg_binacc[bin + 1] * 8) / bm->avg_sampleacc * vi->rate;
        if (fabs(hibitrate - desired_rate) < fabs(lobitrate - desired_rate))
            bin++;
    }
    return bin;
}

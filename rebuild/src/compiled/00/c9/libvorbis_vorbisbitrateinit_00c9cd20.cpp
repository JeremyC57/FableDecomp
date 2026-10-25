// libvorbis::vorbis_bitrate_init(vorbis_info* vi, bitrate_manager_state* bm) -- set up bitrate management: zero the state, size the packet queue from the managed-bitrate times and the short block size, allocate the bin queues, average/min-max tracking stacks and the queued packet buffers (one buffer when management is off)
// Leaf: @vorbis_bitrate_init@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9CD20, 484 bytes (+ padding), vi in ecx, bm in edx, plain `ret`. libvorbis 1.0.1
//   lib/bitrate.c. Callees: calloc (MSVCR71 import [0x01440058], kept in a register) through _ogg_calloc, __ftol2
//   0x00BFEA70, oggpack_writeinit 0x00C90F10 (`__fastcall`; tail-jumped on the unmanaged path). Doubles 0.0 0x0122ED70,
//   7.0 0x012BF330 (avgfloat = PACKETBLOBS/2).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES): Xiph libvorbis 1.0.1 inside libpk ("Xiph.Org
//   libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`). The 0x68-byte state is bitrate.h's
//   bitrate_manager_state, the 7 doubles at codec_setup + 0xD20 are codec_internal.h's bitrate_manager_info (after
//   book_param[256], fullbooks, psy_param[4] and the 0x1EC-byte vorbis_info_psy_global), PACKETBLOBS = 15 gives the
//   calloc sizes 15*4, (31*30)*4, 31*4 and the 0x14-byte oggpack_buffer / 0x20-byte ogg_packet queues; agent447's landed
//   vorbis_bitrate_clear 0x00C9CF10 frees the same members.
// Label: catalog "InitStreamDecoder_00c9cd20" / manifest "CVsyncCallbackTimerInfo::Initialize" are descriptive/propagated.
// Shape: the shipped source (the three minmax stacks are declared binstack, posstack, limitstack: the 930-entry calloc
//   lands at +0x30).
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // [0x01440058]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_calloc calloc
#define max(x, y) ((x) > (y) ? (x) : (y))
#define PACKETBLOBS 15
#define VE_BANDS 7

typedef unsigned int ogg_uint32_t;
typedef __int64 ogg_int64_t;

typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;

typedef struct {                                   // ogg.h, 0x20 bytes
    unsigned char* packet;
    long           bytes;
    long           b_o_s;
    long           e_o_s;
    ogg_int64_t    granulepos;
    ogg_int64_t    packetno;
} ogg_packet;

typedef struct vorbis_info {                       // codec.h
    int   version;
    int   channels;
    long  rate;                                    // +0x08
    long  bitrate_upper;
    long  bitrate_nominal;
    long  bitrate_lower;
    long  bitrate_window;
    void* codec_setup;                             // +0x1c
} vorbis_info;

typedef struct {                                   // psy.h, 0x1EC bytes
    int   eighth_octave_lines;
    float preecho_thresh[VE_BANDS];
    float postecho_thresh[VE_BANDS];
    float stretch_penalty;
    float preecho_minenergy;
    float ampmax_att_per_sec;
    int   coupling_pkHz[PACKETBLOBS];
    int   coupling_pointlimit[2][PACKETBLOBS];
    int   coupling_prepointamp[PACKETBLOBS];
    int   coupling_postpointamp[PACKETBLOBS];
    int   sliding_lowpass[2][PACKETBLOBS];
} vorbis_info_psy_global;

typedef struct bitrate_manager_info {              // codec_internal.h
    double queue_avg_time;
    double queue_avg_center;
    double queue_minmax_time;
    double queue_hardmin;
    double queue_hardmax;
    double queue_avgmin;
    double queue_avgmax;
    double avgfloat_downslew_max;
    double avgfloat_upslew_max;
} bitrate_manager_info;

typedef struct codec_setup_info {                  // codec_internal.h (members up to bi; hi and halfrate_flag follow)
    long                     blocksizes[2];
    int                      modes;
    int                      maps;
    int                      floors;
    int                      residues;
    int                      books;
    int                      psys;
    struct vorbis_info_mode* mode_param[64];
    int                      map_type[64];
    void*                    map_param[64];
    int                      floor_type[64];
    void*                    floor_param[64];
    int                      residue_type[64];
    void*                    residue_param[64];
    struct static_codebook*  book_param[256];
    struct codebook*         fullbooks;
    struct vorbis_info_psy*  psy_param[4];
    vorbis_info_psy_global   psy_g_param;          // +0xb34
    bitrate_manager_info     bi;                   // +0xd20
} codec_setup_info;

typedef struct bitrate_manager_state {             // bitrate.h, 0x68 bytes
    ogg_uint32_t*   queue_binned;                  // +0x00
    ogg_uint32_t*   queue_actual;                  // +0x04
    int             queue_size;                    // +0x08
    int             queue_head;
    int             queue_bins;                    // +0x10
    long*           avg_binacc;                    // +0x14
    int             avg_center;
    int             avg_tail;                      // +0x1c
    ogg_uint32_t    avg_centeracc;
    ogg_uint32_t    avg_sampleacc;
    ogg_uint32_t    avg_sampledesired;             // +0x28
    ogg_uint32_t    avg_centerdesired;             // +0x2c
    long*           minmax_binstack;               // +0x30
    long*           minmax_posstack;               // +0x34
    long*           minmax_limitstack;             // +0x38
    long            minmax_stackptr;
    long            minmax_acctotal;
    int             minmax_tail;                   // +0x44
    ogg_uint32_t    minmax_sampleacc;
    ogg_uint32_t    minmax_sampledesired;          // +0x4c
    int             next_to_flush;
    int             last_to_flush;
    double          avgfloat;                      // +0x58
    oggpack_buffer* packetbuffers;                 // +0x60
    ogg_packet*     packets;                       // +0x64
} bitrate_manager_state;

extern "C" void __fastcall oggpack_writeinit(oggpack_buffer* b);   // 0x00C90F10

/* compute bitrate tracking setup, allocate circular packet size queue */
extern "C" void __fastcall vorbis_bitrate_init(vorbis_info* vi, bitrate_manager_state* bm)
{
    int i;
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    bitrate_manager_info* bi = &ci->bi;
    long maxlatency;

    memset(bm, 0, sizeof(*bm));

    if (bi) {

        bm->avg_sampledesired = (ogg_uint32_t)(bi->queue_avg_time * vi->rate);
        bm->avg_centerdesired = (ogg_uint32_t)(bi->queue_avg_time * vi->rate * bi->queue_avg_center);
        bm->minmax_sampledesired = (ogg_uint32_t)(bi->queue_minmax_time * vi->rate);

        /* first find the max possible needed queue size */
        maxlatency = max(bm->avg_sampledesired - bm->avg_centerdesired,
                         bm->minmax_sampledesired) + bm->avg_centerdesired;

        if (maxlatency > 0 &&
            (bi->queue_avgmin > 0 || bi->queue_avgmax > 0 || bi->queue_hardmax > 0 ||
             bi->queue_hardmin > 0)) {
            long maxpackets = maxlatency / (ci->blocksizes[0] >> 1) + 3;
            long bins = PACKETBLOBS;

            bm->queue_size = maxpackets;
            bm->queue_bins = bins;
            bm->queue_binned = (ogg_uint32_t*)_ogg_calloc(maxpackets, bins * sizeof(*bm->queue_binned));
            bm->queue_actual = (ogg_uint32_t*)_ogg_calloc(maxpackets, sizeof(*bm->queue_actual));

            if ((bi->queue_avgmin > 0 || bi->queue_avgmax > 0) &&
                bi->queue_avg_time > 0) {

                bm->avg_binacc = (long*)_ogg_calloc(bins, sizeof(*bm->avg_binacc));
                bm->avgfloat = PACKETBLOBS / 2;

            } else {
                bm->avg_tail = -1;
            }

            if ((bi->queue_hardmin > 0 || bi->queue_hardmax > 0) &&
                bi->queue_minmax_time > 0) {

                bm->minmax_binstack = (long*)_ogg_calloc((bins * 2 + 1) * bins * 2,
                                                         sizeof(bm->minmax_binstack));
                bm->minmax_posstack = (long*)_ogg_calloc((bins * 2 + 1),
                                                         sizeof(bm->minmax_posstack));
                bm->minmax_limitstack = (long*)_ogg_calloc((bins * 2 + 1),
                                                           sizeof(bm->minmax_limitstack));
            } else {
                bm->minmax_tail = -1;
            }

            /* space for the packet queueing */
            bm->packetbuffers = (oggpack_buffer*)_ogg_calloc(maxpackets, sizeof(*bm->packetbuffers));
            bm->packets = (ogg_packet*)_ogg_calloc(maxpackets, sizeof(*bm->packets));
            for (i = 0; i < maxpackets; i++)
                oggpack_writeinit(bm->packetbuffers + i);

        } else {
            bm->packetbuffers = (oggpack_buffer*)_ogg_calloc(1, sizeof(*bm->packetbuffers));
            bm->packets = (ogg_packet*)_ogg_calloc(1, sizeof(*bm->packets));
            oggpack_writeinit(bm->packetbuffers);

        }
    }
}

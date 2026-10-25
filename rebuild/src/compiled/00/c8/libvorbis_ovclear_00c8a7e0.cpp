// libvorbis::ov_clear(OggVorbis_File* vf) -- libvorbis 1.0.1 vorbisfile teardown: clear the block, DSP and stream state, every link's info and comment, free the link tables, clear the sync state and zero the file; returns 0
// Leaf: @ov_clear@
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8A7E0, length 207 bytes (+ int3 padding), __fastcall (vf in ecx).
// Callees (libvorbis 1.0.1 / libogg, all __fastcall): vorbis_block_clear 0x00C8F1B0, vorbis_dsp_clear 0x00C8F650,
//   ogg_stream_clear 0x00C8CE80, vorbis_info_clear 0x00C8DFB0, vorbis_comment_clear 0x00C8DEF0, ogg_sync_clear
//   0x00C8D420; _ogg_free is the MSVCR71 free import [0x01440154] (held in esi). Caller: COggVorbisStream's dtor
//   0x00C885C0 (landed as "CAInputProcess::~CAInputProcess", whose "Release" is this function).
// Source: libvorbis 1.0.1 (20030909) lib/vorbisfile.c ov_clear, minus the `callbacks.close_func` call (libpk's copy
//   has no ov_callbacks: the 0x2C0-byte memset is sizeof(OggVorbis_File) without them).
// ---- libogg 1.1 / libvorbis 1.0.1 (20030909) public structures, as statically linked into libpk ----
typedef __int64 ogg_int64_t;

typedef struct {
    long endbyte;
    int endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long storage;
} oggpack_buffer;

typedef struct {
    unsigned char* data;
    int storage;
    int fill;
    int returned;
    int unsynced;
    int headerbytes;
    int bodybytes;
} ogg_sync_state;

typedef struct {
    unsigned char* body_data;
    long body_storage;
    long body_fill;
    long body_returned;
    int* lacing_vals;
    ogg_int64_t* granule_vals;
    long lacing_storage;
    long lacing_fill;
    long lacing_packet;
    long lacing_returned;
    unsigned char header[282];
    int header_fill;
    int e_o_s;
    int b_o_s;
    long serialno;
    long pageno;
    ogg_int64_t packetno;
    ogg_int64_t granulepos;
} ogg_stream_state;

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

typedef struct vorbis_comment {
    char** user_comments;
    int* comment_lengths;
    int comments;
    char* vendor;
} vorbis_comment;

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

typedef struct vorbis_block {
    float** pcm;
    oggpack_buffer opb;
    long lW;
    long W;
    long nW;
    int pcmend;
    int mode;
    int eofflag;
    ogg_int64_t granulepos;
    ogg_int64_t sequence;
    vorbis_dsp_state* vd;
    void* localstore;
    long localtop;
    long localalloc;
    long totaluse;
    struct alloc_chain* reap;
    long glue_bits;
    long time_bits;
    long floor_bits;
    long res_bits;
    void* internal;
} vorbis_block;

// libpk's private vorbisfile: no ov_callbacks member (reads go through the owner's stream reader), 0x2C0 bytes.
typedef struct OggVorbis_File {
    void* datasource;                      // +0x000
    int seekable;                          // +0x004
    ogg_int64_t offset;                    // +0x008
    ogg_int64_t end;                       // +0x010
    ogg_sync_state oy;                     // +0x018
    int links;                             // +0x034
    ogg_int64_t* offsets;                  // +0x038
    ogg_int64_t* dataoffsets;              // +0x03c
    long* serialnos;                       // +0x040
    ogg_int64_t* pcmlengths;               // +0x044
    vorbis_info* vi;                       // +0x048
    vorbis_comment* vc;                    // +0x04c
    ogg_int64_t pcm_offset;                // +0x050
    int ready_state;                       // +0x058
    long current_serialno;                 // +0x05c
    int current_link;                      // +0x060
    double bittrack;                       // +0x068
    double samptrack;                      // +0x070
    ogg_stream_state os;                   // +0x078
    vorbis_dsp_state vd;                   // +0x1e0
    vorbis_block vb;                       // +0x250
} OggVorbis_File;
// vorbisfile.c 1.0.1 (libpk's modified copy) is built /Gr: every entry point is extern "C" __fastcall.
#define OV_FALSE  -1
#define OV_EOF    -2
#define OV_EREAD  -128
#define OV_EINVAL -131
#define OPENED 2
#include <string.h>
extern "C" __declspec(dllimport) void __cdecl free(void* block);

extern "C" {
int __fastcall vorbis_block_clear(vorbis_block* vb);       // 0x00C8F1B0
void __fastcall vorbis_dsp_clear(vorbis_dsp_state* v);     // 0x00C8F650
int __fastcall ogg_stream_clear(ogg_stream_state* os);     // 0x00C8CE80
void __fastcall vorbis_info_clear(vorbis_info* vi);        // 0x00C8DFB0
void __fastcall vorbis_comment_clear(vorbis_comment* vc);  // 0x00C8DEF0
int __fastcall ogg_sync_clear(ogg_sync_state* oy);         // 0x00C8D420

int __fastcall ov_clear(OggVorbis_File* vf)
{
    if (vf) {
        vorbis_block_clear(&vf->vb);
        vorbis_dsp_clear(&vf->vd);
        ogg_stream_clear(&vf->os);

        if (vf->vi && vf->links) {
            int i;
            for (i = 0; i < vf->links; i++) {
                vorbis_info_clear(vf->vi + i);
                vorbis_comment_clear(vf->vc + i);
            }
            free(vf->vi);
            free(vf->vc);
        }
        if (vf->dataoffsets) free(vf->dataoffsets);
        if (vf->pcmlengths) free(vf->pcmlengths);
        if (vf->serialnos) free(vf->serialnos);
        if (vf->offsets) free(vf->offsets);
        ogg_sync_clear(&vf->oy);
        memset(vf, 0, sizeof(*vf));
    }
    return 0;
}
}
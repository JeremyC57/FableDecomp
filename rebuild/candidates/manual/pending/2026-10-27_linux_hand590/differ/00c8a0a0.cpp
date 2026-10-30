// _fetch_headers(OggVorbis_File* vf, vorbis_info* vi, vorbis_comment* vc, long* serialno, ogg_page* og_ptr) -- vorbisfile.c: read the first page (unless one is handed in), reset the logical stream to its serial number, then pull the three Vorbis header packets (identification, comment, setup) into vi/vc; OV_EREAD / OV_ENOTVORBIS when no first page, OV_EBADHEADER (clearing vi/vc and dropping back to OPENED) on a bad or missing header
// Grade: DIFFER(295v295) -- VC7.1 gives the same-TU static its own private convention (vf in ecx, serialno in eax, og_ptr / vi / vc on the stack) where retail's LTCG convention passes og_ptr in eax and serialno in ecx with vf / vi / vc on the stack; the _get_next_page calls also get vf in esi (no `mov eax,ebp`); every other byte matches the libvorbis 1.0.1 shape. Lane gap (as chunk 587's _get_prev_page 0x00C89D70); 2 grades
// Leaf: __fetch_headers@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8A0A0, 295 bytes. A C `static` with libpk's LTCG private convention: og_ptr in eax,
//   serialno in ecx, vf / vi / vc on the stack, callee-pops (`ret 0Ch`).
// Library: libvorbis 1.0.1 lib/vorbisfile.c inside libpk's statically linked Xiph decoder ("Xiph.Org libVorbis I
//   20030909"); no PDB/RTTI, so the name is the libvorbis one. Identity (two sources): the body is vorbisfile.c
//   _fetch_headers verbatim -- CHUNKSIZE 0x2134, OV_EREAD -128 (`cmp eax,-80h ; cmp edx,-1`), OV_ENOTVORBIS -132
//   (0xFFFFFF7C), OV_EBADHEADER -133 (0xFFFFFF7B), ready_state STREAMSET 3 / OPENED 2 at vf+0x58, os.serialno at
//   vf+0x1C8, the i<3 header loop -- and its callers are the vorbisfile statics in 1.0.1 order: _prefetch_all_headers
//   0x00C8A27C (`_fetch_headers(vf, vf->vi+i, vf->vc+i, NULL, NULL)`), _fetch_and_process_packet 0x00C8A6AD and _ov_open1
//   0x00C8CA5E (`_fetch_headers(vf, vf->vi, vf->vc, &vf->current_serialno, NULL)`). Callees in vorbisfile.c's order:
//   _get_next_page 0x00C89CB0 (static: vf in eax, `ret 0Ch`), ogg_page_serialno 0x00C8CDC0, ogg_stream_reset_serialno
//   0x00C8D9C0, vorbis_info_init 0x00C8DF70, vorbis_comment_init 0x00C8DBE0, ogg_stream_pagein 0x00C8D690,
//   ogg_stream_packetout 0x00C8DB10 (the `_packetout(os, op, 1)` wrapper; landed under a wrong STL name, NAME_FIXES.tsv),
//   vorbis_synthesis_headerin 0x00C8E580, vorbis_info_clear 0x00C8DFB0, vorbis_comment_clear 0x00C8DEF0 (libogg /
//   libvorbis API: LTCG extern "C" __fastcall, as the landed libpk rows bind them).
// Label: the manifest's "CreatureHitNotification_Initialize" is a guess (wrong).
// Convention: as for the parked _get_prev_page 0x00C89D70 (chunk 587), the static and its same-TU static callees carry
//   noinline bodies (`__stdcall` for the callee pop), and a scaffold caller (the _ov_open1 call) makes VC7.1 emit them
//   with a TU-local convention; only _fetch_headers is graded. C linkage gives `__fetch_headers@20`.
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
typedef struct ogg_page {
    unsigned char* header;
    long header_len;
    unsigned char* body;
    long body_len;
} ogg_page;
// libpk's stream reader (the OggVorbis_File datasource; COggVorbisStream's +4 member, vtable 0x012B8458).
class CStreamReader {
public:
    unsigned long Tell() const;                        // 0x00C4AA20
    unsigned long GetSize() const;                     // 0x00C4AA30
    bool Seek(long offset, int origin);                // 0x00C4AFD0: origin 2 = start
    bool ReadBlock(void* buffer, unsigned long size);  // 0x00C4B890
};
extern "C" {
char* __fastcall ogg_sync_buffer(ogg_sync_state* oy, long size);       // 0x00C8D470
int __fastcall ogg_sync_wrote(ogg_sync_state* oy, long bytes);         // 0x00C8D4F0
int __fastcall ogg_sync_reset(ogg_sync_state* oy);                     // 0x00C8D950
long __fastcall ogg_sync_pageseek(ogg_sync_state* oy, ogg_page* og);   // 0x00C8D510
}
#include <errno.h>
#define CHUNKSIZE 8500
#define OV_FALSE  -1
#define OV_EOF    -2
#define OV_EREAD  -128
#define OV_EFAULT -129

extern "C" {

#pragma auto_inline(off)
static __declspec(noinline) long __stdcall _get_data(OggVorbis_File* vf)                   // 0x00C89C10 (static)
{
    errno = 0;
    if (vf->datasource) {
        CStreamReader* reader = (CStreamReader*)vf->datasource;
        char* buffer = ogg_sync_buffer(&vf->oy, CHUNKSIZE);
        unsigned long position = reader->Tell();
        unsigned long bytes = reader->GetSize() - position;
        if (bytes < CHUNKSIZE) {
            if (bytes == 0)
                return 0;
        } else {
            bytes = CHUNKSIZE;
        }
        if (!reader->ReadBlock(buffer, bytes))
            return -1;
        ogg_sync_wrote(&vf->oy, bytes);
        return bytes;
    }
    return 0;
}

static __declspec(noinline) void __stdcall _seek_helper(OggVorbis_File* vf, ogg_int64_t offset)   // 0x00C89C80 (static)
{
    if (vf->datasource) {
        ((CStreamReader*)vf->datasource)->Seek((long)offset, 2);
        vf->offset = offset;
        ogg_sync_reset(&vf->oy);
    }
}

static __declspec(noinline) ogg_int64_t __stdcall _get_next_page(OggVorbis_File* vf, ogg_page* og, ogg_int64_t boundary)
{                                                                                           // 0x00C89CB0 (static)
    if (boundary > 0) boundary += vf->offset;
    while (1) {
        long more;

        if (boundary > 0 && vf->offset >= boundary) return (OV_FALSE);
        more = ogg_sync_pageseek(&vf->oy, og);

        if (more < 0) {
            vf->offset -= more;
        } else {
            if (more == 0) {
                if (!boundary) return (OV_FALSE);
                {
                    long ret = _get_data(vf);
                    if (ret == 0) return (OV_EOF);
                    if (ret < 0) return (OV_EREAD);
                }
            } else {
                ogg_int64_t ret = vf->offset;
                vf->offset += more;
                return (ret);
            }
        }
    }
}

#define OV_ENOTVORBIS -132
#define OV_EBADHEADER -133
#define OPENED     2
#define STREAMSET  3

typedef struct {
    unsigned char* packet;
    long bytes;
    long b_o_s;
    long e_o_s;
    ogg_int64_t granulepos;
    ogg_int64_t packetno;
} ogg_packet;

int __fastcall ogg_page_serialno(ogg_page* og);                                       // 0x00C8CDC0
int __fastcall ogg_stream_reset_serialno(ogg_stream_state* os, int serialno);         // 0x00C8D9C0
void __fastcall vorbis_info_init(vorbis_info* vi);                                    // 0x00C8DF70
void __fastcall vorbis_comment_init(vorbis_comment* vc);                              // 0x00C8DBE0
int __fastcall ogg_stream_pagein(ogg_stream_state* os, ogg_page* og);                 // 0x00C8D690
int __fastcall ogg_stream_packetout(ogg_stream_state* os, ogg_packet* op);            // 0x00C8DB10
int __fastcall vorbis_synthesis_headerin(vorbis_info* vi, vorbis_comment* vc, ogg_packet* op);   // 0x00C8E580
void __fastcall vorbis_info_clear(vorbis_info* vi);                                   // 0x00C8DFB0
void __fastcall vorbis_comment_clear(vorbis_comment* vc);                             // 0x00C8DEF0

static __declspec(noinline) int __stdcall _fetch_headers(OggVorbis_File* vf, vorbis_info* vi, vorbis_comment* vc,
                                                         long* serialno, ogg_page* og_ptr)
{
    ogg_page og;
    ogg_packet op;
    int i, ret;

    if (!og_ptr) {
        ogg_int64_t llret = _get_next_page(vf, &og, CHUNKSIZE);
        if (llret == OV_EREAD) return (OV_EREAD);
        if (llret < 0) return OV_ENOTVORBIS;
        og_ptr = &og;
    }

    ogg_stream_reset_serialno(&vf->os, ogg_page_serialno(og_ptr));
    if (serialno) *serialno = vf->os.serialno;
    vf->ready_state = STREAMSET;

    /* extract the initial header from the first page and verify that the
       Ogg bitstream is in fact Vorbis data */

    vorbis_info_init(vi);
    vorbis_comment_init(vc);

    i = 0;
    while (i < 3) {
        ogg_stream_pagein(&vf->os, og_ptr);
        while (i < 3) {
            int result = ogg_stream_packetout(&vf->os, &op);
            if (result == 0) break;
            if (result == -1) {
                ret = OV_EBADHEADER;
                goto bail_header;
            }
            if ((ret = vorbis_synthesis_headerin(vi, vc, &op))) {
                goto bail_header;
            }
            i++;
        }
        if (i < 3)
            if (_get_next_page(vf, og_ptr, CHUNKSIZE) < 0) {
                ret = OV_EBADHEADER;
                goto bail_header;
            }
    }
    return 0;

bail_header:
    vorbis_info_clear(vi);
    vorbis_comment_clear(vc);
    vf->ready_state = OPENED;

    return ret;
}
#pragma auto_inline(on)

// TU scaffolds: the three vorbisfile.c call shapes (_prefetch_all_headers, _fetch_and_process_packet, _ov_open1); not graded
int _prefetch_all_headers_scaffold(OggVorbis_File* vf, int i)
{
    return _fetch_headers(vf, vf->vi + i, vf->vc + i, 0, 0);
}
int _fetch_and_process_packet_scaffold(OggVorbis_File* vf, ogg_page* og)
{
    return _fetch_headers(vf, vf->vi, vf->vc, &vf->current_serialno, og);
}
int _ov_open1_scaffold(OggVorbis_File* vf)
{
    return _fetch_headers(vf, vf->vi, vf->vc, &vf->current_serialno, 0);
}

}   // extern "C"

// libvorbis::ov_open_callbacks(void* f, OggVorbis_File* vf, char* initial, long ibytes) -- vorbisfile.c (libpk fork without ov_callbacks): _ov_open1, then _ov_open2 when it succeeded
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside one vorbisfile.c TU (below) VC7.1 /O2 gives the static retail's private convention itself and tracks every same-TU callee's register use (chunk 585/586/593 lever).
// Leaf: @ov_open_callbacks@16
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8CAE0; convention as VC7.1 assigns it: __fastcall (f ecx, vf edx, initial/ibytes on the stack), `ret 8`.
// Library: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") lib/vorbisfile.c as statically linked into libpk, with libpk's
//   PKCBufferedReader (Seek 0x00C4AFD0, Tell 0x00C4AA20, GetSize 0x00C4AA30, ReadBlock 0x00C4B890) in place of ov_callbacks.
//   No PDB/RTTI: the names are the Xiph static/API names; identity is the byte-exact vorbisfile.c body in its call graph.
// ---- one vorbisfile.c TU (retry1): every libpk vorbisfile static of 0x00C89C10..0x00C8CB20 that the rows below need, in
//   dependency order, so VC7.1 /O2 gives each static its private register convention and tracks the callees' register
//   use exactly as retail; only the row named by `// Leaf:` is graded. The two scaffolds at the end stand in for the seek
//   paths (ov_raw_seek / ov_pcm_seek*) outside this slice that call _ov_initset / _ov_initprime.
// ---- libogg 1.1 / libvorbis 1.0.1 (20030909) public structures, as statically linked into libpk ----
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);   // IAT 0x01440054
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // IAT 0x01440158
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // IAT 0x01440058
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
enum ESeekOrigin { SEEK_ORIGIN_CURRENT = 0, SEEK_ORIGIN_END = 1, SEEK_ORIGIN_START = 2 };
class PKCBufferedReader {
public:
    unsigned long Tell();                              // 0x00C4AA20 (landed)
    unsigned long GetSize();                           // 0x00C4AA30 (landed)
    bool Seek(long offset, ESeekOrigin origin);        // 0x00C4AFD0 (landed): the reader's fseek
    bool ReadBlock(void* buffer, unsigned long size);  // 0x00C4B890
};
extern "C" {
char* __fastcall ogg_sync_buffer(ogg_sync_state* oy, long size);       // 0x00C8D470
int __fastcall ogg_sync_wrote(ogg_sync_state* oy, long bytes);         // 0x00C8D4F0
int __fastcall ogg_sync_reset(ogg_sync_state* oy);                     // 0x00C8D950
long __fastcall ogg_sync_pageseek(ogg_sync_state* oy, ogg_page* og);   // 0x00C8D510
}
extern "C" __declspec(dllimport) int* __cdecl _errno(void);   // IAT 0x01440008 (msvcr71 errno)
#define errno (*_errno())
#define CHUNKSIZE 8500
#ifndef NULL
#define NULL 0
#endif
#define OV_FALSE  -1
#define OV_EOF    -2
#define OV_EREAD  -128
#define OV_EFAULT -129

extern "C" {

#pragma auto_inline(off)
static __declspec(noinline) long _get_data(OggVorbis_File* vf)                   // 0x00C89C10 (static)
{
    errno = 0;
    if (vf->datasource) {
        char* buffer = ogg_sync_buffer(&vf->oy, CHUNKSIZE);
        unsigned long bytes = ((PKCBufferedReader*)vf->datasource)->Tell();
        unsigned long left = ((PKCBufferedReader*)vf->datasource)->GetSize() - bytes;
        bytes = CHUNKSIZE;
        if (left < CHUNKSIZE) {
            bytes = left;
            if (bytes == 0)
                return 0;
        }
        if (!((PKCBufferedReader*)vf->datasource)->ReadBlock(buffer, bytes))
            return -1;
        ogg_sync_wrote(&vf->oy, bytes);
        return bytes;
    }
    return 0;
}

static __declspec(noinline) void __stdcall _seek_helper(OggVorbis_File* vf, ogg_int64_t offset)   // 0x00C89C80 (static)
{
    if (vf->datasource) {
        ((PKCBufferedReader*)vf->datasource)->Seek((long)offset, SEEK_ORIGIN_START);
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

static __declspec(noinline) ogg_int64_t __stdcall _get_prev_page(OggVorbis_File* vf, ogg_page* og)
{
    ogg_int64_t begin = vf->offset;
    ogg_int64_t end = begin;
    ogg_int64_t ret;
    ogg_int64_t offset = -1;

    while (offset == -1) {
        begin -= CHUNKSIZE;
        if (begin < 0)
            begin = 0;
        _seek_helper(vf, begin);
        while (vf->offset < end) {
            ret = _get_next_page(vf, og, end - vf->offset);
            if (ret == OV_EREAD) return (OV_EREAD);
            if (ret < 0) {
                break;
            } else {
                offset = ret;
            }
        }
    }

    /* we have the offset.  Actually snork and hold the page now */
    _seek_helper(vf, offset);
    ret = _get_next_page(vf, og, CHUNKSIZE);
    if (ret < 0)
        /* this shouldn't be possible */
        return (OV_EFAULT);

    return (offset);
}

int __fastcall ogg_page_serialno(ogg_page* og);                                       // 0x00C8CDC0

static __declspec(noinline) int __fastcall _bisect_forward_serialno(OggVorbis_File* vf, ogg_int64_t begin,
                                                                   ogg_int64_t searched, ogg_int64_t end,
                                                                   long currentno, long m)   // 0x00C89E80 (static)
{
    ogg_int64_t endsearched = end;
    ogg_int64_t next = end;
    ogg_page og;
    ogg_int64_t ret;

    /* the below guards against garbage seperating the last and
       first pages of two links. */
    while (searched < endsearched) {
        ogg_int64_t bisect;

        if (endsearched - searched < CHUNKSIZE) {
            bisect = searched;
        } else {
            bisect = (searched + endsearched) / 2;
        }

        _seek_helper(vf, bisect);
        ret = _get_next_page(vf, &og, -1);
        if (ret == OV_EREAD) return (OV_EREAD);
        if (ret < 0 || ogg_page_serialno(&og) != currentno) {
            endsearched = bisect;
            if (ret >= 0) next = ret;
        } else {
            searched = ret + og.header_len + og.body_len;
        }
    }

    _seek_helper(vf, next);
    ret = _get_next_page(vf, &og, -1);
    if (ret == OV_EREAD) return (OV_EREAD);

    if (searched >= end || ret < 0) {
        vf->links = m + 1;
        vf->offsets = (ogg_int64_t*)malloc((vf->links + 1) * sizeof(*vf->offsets));
        vf->serialnos = (long*)malloc(vf->links * sizeof(*vf->serialnos));
        vf->offsets[m + 1] = searched;
    } else {
        ret = _bisect_forward_serialno(vf, next, vf->offset,
                                       end, ogg_page_serialno(&og), m + 1);
        if (ret == OV_EREAD) return (OV_EREAD);
    }

    vf->offsets[m] = begin;
    vf->serialnos[m] = currentno;
    return (0);
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

ogg_int64_t __fastcall ogg_page_granulepos(ogg_page* og);                             // 0x00C8CD30
long __fastcall vorbis_packet_blocksize(vorbis_info* vi, ogg_packet* op);             // 0x00C8EFC0

static __declspec(noinline) void __stdcall _prefetch_all_headers(OggVorbis_File* vf, ogg_int64_t dataoffset)
{                                                                                           // 0x00C8A1D0 (static)
    ogg_page og;
    int i;
    ogg_int64_t ret;

    vf->vi = (vorbis_info*)realloc(vf->vi, vf->links * sizeof(*vf->vi));
    vf->vc = (vorbis_comment*)realloc(vf->vc, vf->links * sizeof(*vf->vc));
    vf->dataoffsets = (ogg_int64_t*)malloc(vf->links * sizeof(*vf->dataoffsets));
    vf->pcmlengths = (ogg_int64_t*)malloc(vf->links * 2 * sizeof(*vf->pcmlengths));

    for (i = 0; i < vf->links; i++) {
        if (i == 0) {
            /* we already grabbed the initial header earlier.  Just set the offset */
            vf->dataoffsets[i] = dataoffset;
            _seek_helper(vf, dataoffset);

        } else {

            /* seek to the location of the initial header */

            _seek_helper(vf, vf->offsets[i]);
            if (_fetch_headers(vf, vf->vi + i, vf->vc + i, NULL, NULL) < 0) {
                vf->dataoffsets[i] = -1;
            } else {
                vf->dataoffsets[i] = vf->offset;
            }
        }

        /* fetch beginning PCM offset */

        if (vf->dataoffsets[i] != -1) {
            ogg_int64_t accumulated = 0;
            long lastblock = -1;
            int result;

            ogg_stream_reset_serialno(&vf->os, vf->serialnos[i]);

            while (1) {
                ogg_packet op;

                ret = _get_next_page(vf, &og, -1);
                if (ret < 0)
                    /* this should not be possible unless the file is
                       truncated/mangled */
                    break;

                if (ogg_page_serialno(&og) != vf->serialnos[i])
                    break;

                /* count blocksizes of all frames in the page */
                ogg_stream_pagein(&vf->os, &og);
                while ((result = ogg_stream_packetout(&vf->os, &op))) {
                    if (result > 0) { /* ignore holes */
                        long thisblock = vorbis_packet_blocksize(vf->vi + i, &op);
                        if (lastblock != -1)
                            accumulated += (lastblock + thisblock) >> 2;
                        lastblock = thisblock;
                    }
                }

                if (ogg_page_granulepos(&og) != -1) {
                    /* pcm offset of last packet on the first audio page */
                    accumulated = ogg_page_granulepos(&og) - accumulated;
                    break;
                }
            }

            /* less than zero?  This is a stream with samples trimmed off
               the beginning, a normal occurrence; set the offset to zero */
            if (accumulated < 0) accumulated = 0;

            vf->pcmlengths[i * 2] = accumulated;
        }

        /* get the PCM length of this link. To do this,
           get the last page of the stream */
        {
            ogg_int64_t end = vf->offsets[i + 1];
            _seek_helper(vf, end);

            while (1) {
                ret = _get_prev_page(vf, &og);
                if (ret < 0) {
                    /* this should not be possible */
                    vorbis_info_clear(vf->vi + i);
                    vorbis_comment_clear(vf->vc + i);
                    break;
                }
                if (ogg_page_granulepos(&og) != -1) {
                    vf->pcmlengths[i * 2 + 1] = ogg_page_granulepos(&og) - vf->pcmlengths[i * 2];
                    break;
                }
                vf->offset = ret;
            }
        }
    }
}

#define OV_EINVAL -131
void __fastcall vorbis_dsp_clear(vorbis_dsp_state* v);                                // 0x00C8F650
int __fastcall vorbis_block_clear(vorbis_block* vb);                                  // 0x00C8F1B0
int __fastcall vorbis_synthesis_halfrate(vorbis_info* vi, int flag);                  // 0x00C8F030

#define INITSET    4
#define OV_HOLE    -3
#define OV_EBADLINK -137
int __fastcall vorbis_synthesis_init(vorbis_dsp_state* v, vorbis_info* vi);           // 0x00C8FF00
int __fastcall vorbis_block_init(vorbis_dsp_state* v, vorbis_block* vb);              // 0x00C8F080
int __fastcall vorbis_synthesis(vorbis_block* vb, ogg_packet* op);                    // 0x00C8EDA0
int __fastcall vorbis_synthesis_blockin(vorbis_dsp_state* v, vorbis_block* vb);       // 0x00C8FF30
int __fastcall vorbis_synthesis_pcmout(vorbis_dsp_state* v, float*** pcm);           // 0x00C908E0

static __declspec(noinline) int _make_decode_ready(OggVorbis_File* vf)                  // 0x00C8A4B0 (static)
{
    if (vf->ready_state > STREAMSET) return 0;
    if (vf->ready_state < STREAMSET) return OV_EFAULT;
    if (vf->seekable) {
        if (vorbis_synthesis_init(&vf->vd, vf->vi + vf->current_link))
            return OV_EBADLINK;
    } else {
        if (vorbis_synthesis_init(&vf->vd, vf->vi))
            return OV_EBADLINK;
    }
    vorbis_block_init(&vf->vd, &vf->vb);
    vf->ready_state = INITSET;
    vf->bittrack = 0.f;
    vf->samptrack = 0.f;
    return 0;
}

static __declspec(noinline) void _decode_clear(OggVorbis_File* vf)                      // 0x00C8A520 (static)
{
    vorbis_dsp_clear(&vf->vd);
    vorbis_block_clear(&vf->vb);
    vf->ready_state = OPENED;
}

static __declspec(noinline) int __stdcall _fetch_and_process_packet(OggVorbis_File* vf, ogg_packet* op_in,
                                                                    int readp, int spanp)   // 0x00C8A540 (static)
{
    ogg_page og;

    /* handle one packet.  Try to fetch it from current stream state */
    /* extract packets from page */
    while (1) {

        /* process a packet if we can.  If the machine isn't loaded,
           neither is a page */
        if (vf->ready_state == INITSET) {
            while (1) {
                ogg_packet op;
                ogg_packet* op_ptr = (op_in ? op_in : &op);
                int result = ogg_stream_packetout(&vf->os, op_ptr);
                ogg_int64_t granulepos;

                op_in = NULL;
                if (result == -1) return (OV_HOLE); /* hole in the data. */
                if (result > 0) {
                    /* got a packet.  process it */
                    granulepos = op_ptr->granulepos;
                    if (!vorbis_synthesis(&vf->vb, op_ptr)) { /* lazy check for lazy
                                                                 header handling.  The
                                                                 header packets aren't
                                                                 audio, so if/when we
                                                                 submit them,
                                                                 vorbis_synthesis will
                                                                 reject them */

                        /* suck in the synthesis data and track bitrate */
                        {
                            int oldsamples = vorbis_synthesis_pcmout(&vf->vd, NULL);
                            /* for proper use of libvorbis within libvorbisfile,
                               oldsamples will always be zero. */
                            if (oldsamples) return (OV_EFAULT);

                            vorbis_synthesis_blockin(&vf->vd, &vf->vb);
                            vf->samptrack += vorbis_synthesis_pcmout(&vf->vd, NULL);
                            vf->bittrack += op_ptr->bytes * 8;
                        }

                        /* update the pcm offset. */
                        if (granulepos != -1 && !op_ptr->e_o_s) {
                            int link = (vf->seekable ? vf->current_link : 0);
                            int i, samples;

                            /* this packet has a pcm_offset on it (the last packet
                               completed on a page carries the offset) After processing
                               (above), we know the pcm position of the *last* sample
                               ready to be returned. Find the offset of the *first*

                               As an aside, this trick is inaccurate if we begin
                               reading anew right at the last page; the end-of-stream
                               granulepos declares the last frame in the stream, and the
                               last packet of the last page may be a partial frame.
                               So, we need a previous granulepos from an in-sequence page
                               to have a reference point.  Thus the !op_ptr->e_o_s clause
                               above */

                            if (vf->seekable && link > 0)
                                granulepos -= vf->pcmlengths[link * 2];
                            if (granulepos < 0) granulepos = 0; /* actually, this
                                                                   shouldn't be possible
                                                                   here unless the stream
                                                                   is very broken */

                            samples = vorbis_synthesis_pcmout(&vf->vd, NULL);

                            granulepos -= samples;
                            for (i = 0; i < link; i++)
                                granulepos += vf->pcmlengths[i * 2 + 1];
                            vf->pcm_offset = granulepos;
                        }
                        return (1);
                    }
                }
                else
                    break;
            }
        }

        if (vf->ready_state >= OPENED) {
            int ret;
            if (!readp) return (0);
            if ((ret = (int)_get_next_page(vf, &og, -1)) < 0) {
                return (OV_EOF); /* eof.
                                    leave unitialized */
            }

            /* bitrate tracking; add the header's bytes here, the body bytes
               are done by packet above */
            vf->bittrack += og.header_len * 8;

            /* has our decoding just traversed a bitstream boundary? */
            if (vf->ready_state == INITSET) {
                if (vf->current_serialno != ogg_page_serialno(&og)) {
                    if (!spanp)
                        return (OV_EOF);

                    _decode_clear(vf);

                    if (!vf->seekable) {
                        vorbis_info_clear(vf->vi);
                        vorbis_comment_clear(vf->vc);
                    }
                }
            }
        }

        /* Do we need to load a new machine before submitting the page? */
        /* This is different in the seekable and non-seekable cases.

           In the seekable case, we already have all the header
           information loaded and cached; we just initialize the machine
           with it and continue on our merry way.

           In the non-seekable (streaming) case, we'll only be at a
           boundary if we just left the previous logical bitstream and
           we're now nominally at the header of the next bitstream
        */

        if (vf->ready_state != INITSET) {
            int link;

            if (vf->ready_state < STREAMSET) {
                if (vf->seekable) {
                    vf->current_serialno = ogg_page_serialno(&og);

                    /* match the serialno to bitstream section.  We use this rather than
                       offset positions to avoid problems near logical bitstream
                       boundaries */
                    for (link = 0; link < vf->links; link++)
                        if (vf->serialnos[link] == vf->current_serialno) break;
                    if (link == vf->links) return (OV_EBADLINK); /* sign of a bogus
                                                                    stream.  error out,
                                                                    leave machine
                                                                    uninitialized */

                    vf->current_link = link;

                    ogg_stream_reset_serialno(&vf->os, vf->current_serialno);
                    vf->ready_state = STREAMSET;

                } else {
                    /* we're streaming */
                    /* fetch the three header packets, build the info struct */

                    int ret = _fetch_headers(vf, vf->vi, vf->vc, &vf->current_serialno, &og);
                    if (ret) return (ret);
                    vf->current_link++;
                    link = 0;
                }
            }

            {
                int ret = _make_decode_ready(vf);
                if (ret < 0) return ret;
            }
        }
        ogg_stream_pagein(&vf->os, &og);
    }
}
#pragma auto_inline(on)

int __fastcall ov_halfrate(OggVorbis_File* vf, int flag)                                // 0x00C8A8B0
{
    int i;
    if (vf->vi == 0) return OV_EINVAL;
    if (!vf->seekable) return OV_EINVAL;
    if (vf->ready_state >= STREAMSET)
        _decode_clear(vf);

    for (i = 0; i < vf->links; i++) {
        if (vorbis_synthesis_halfrate(vf->vi + i, flag)) {
            ov_halfrate(vf, 0);
            return OV_EINVAL;
        }
    }
    return 0;
}

#define PARTOPEN 1
int __fastcall ogg_sync_init(ogg_sync_state* oy);                                     // 0x00C8D400
int __fastcall ogg_stream_init(ogg_stream_state* os, int serialno);                   // 0x00C8CE20
int __fastcall ov_clear(OggVorbis_File* vf);                                          // 0x00C8A7E0

int __fastcall ov_raw_seek(OggVorbis_File* vf, ogg_int64_t pos);                       // 0x00C8AB70

static __declspec(noinline) int _ov_initset(OggVorbis_File* vf)                         // 0x00C8C1D0 (static)
{
    while (1) {
        if (vf->ready_state == INITSET) break;
        /* suck in another packet */
        {
            int ret = _fetch_and_process_packet(vf, NULL, 1, 0);
            if (ret < 0 && ret != OV_HOLE) return (ret);
        }
    }
    return 0;
}

static __declspec(noinline) int _ov_initprime(OggVorbis_File* vf)                       // 0x00C8C200 (static)
{
    vorbis_dsp_state* vd = &vf->vd;
    while (1) {
        if (vf->ready_state == INITSET)
            if (vorbis_synthesis_pcmout(vd, 0)) break;

        /* suck in another packet */
        {
            int ret = _fetch_and_process_packet(vf, NULL, 1, 0);
            if (ret < 0 && ret != OV_HOLE) return (ret);
        }
    }
    return 0;
}

static __declspec(noinline) int _open_seekable2(OggVorbis_File* vf)                     // 0x00C8C8C0 (static)
{
    long serialno = vf->current_serialno;
    ogg_int64_t dataoffset = vf->offset, end;
    ogg_page og;

    /* we're partially open and have a first link header state in
       storage in vf */
    /* we can seek, so set out learning all about this file */
    ((PKCBufferedReader*)vf->datasource)->Seek(0, SEEK_ORIGIN_END);
    vf->offset = vf->end = ((PKCBufferedReader*)vf->datasource)->Tell();

    /* We get the offset for the last page of the physical bitstream.
       Most OggVorbis files will contain a single logical bitstream */
    end = _get_prev_page(vf, &og);
    if (end < 0) return ((int)end);

    /* more than one logical bitstream? */
    if (ogg_page_serialno(&og) != serialno) {

        /* Chained bitstream. Bisect-search each logical bitstream
           section.  Do so based on serial number only */
        if (_bisect_forward_serialno(vf, 0, 0, end + 1, serialno, 0) < 0) return (OV_EREAD);

    } else {

        /* Only one logical bitstream */
        if (_bisect_forward_serialno(vf, 0, end, end + 1, serialno, 0)) return (OV_EREAD);

    }

    /* the initial header memory is referenced by vf after; don't free it */
    _prefetch_all_headers(vf, dataoffset);
    return (ov_raw_seek(vf, 0));
}

static __declspec(noinline) int __stdcall _ov_open1(void* f, OggVorbis_File* vf, char* initial, long ibytes)   // 0x00C8C9A0 (static)
{
    int offsettest = -1;
    int ret;
    if (f) {
        if (((PKCBufferedReader*)f)->Seek(0, SEEK_ORIGIN_CURRENT))
            offsettest = (int)((PKCBufferedReader*)f)->Tell();
        else
            offsettest = -1;
    }

    memset(vf, 0, sizeof(*vf));
    vf->datasource = f;

    ogg_sync_init(&vf->oy);

    if (initial) {
        char* buffer = ogg_sync_buffer(&vf->oy, ibytes);
        memcpy(buffer, initial, ibytes);
        ogg_sync_wrote(&vf->oy, ibytes);
    }

    if (offsettest != -1) vf->seekable = 1;

    vf->links = 1;
    vf->vi = (vorbis_info*)calloc(vf->links, sizeof(*vf->vi));
    vf->vc = (vorbis_comment*)calloc(vf->links, sizeof(*vf->vc));
    ogg_stream_init(&vf->os, -1);

    if ((ret = _fetch_headers(vf, vf->vi, vf->vc, &vf->current_serialno, 0)) < 0) {
        vf->datasource = 0;
        ov_clear(vf);
    } else
        vf->ready_state = PARTOPEN;
    return (ret);
}

static __declspec(noinline) int _ov_open2(OggVorbis_File* vf)                           // 0x00C8CA90 (static)
{
    if (vf->ready_state != PARTOPEN) return OV_EINVAL;
    vf->ready_state = OPENED;
    if (vf->seekable) {
        int ret = _open_seekable2(vf);
        if (ret) {
            vf->datasource = NULL;
            ov_clear(vf);
        }
        return (ret);
    } else
        vf->ready_state = STREAMSET;

    return 0;
}

int __fastcall ov_open_callbacks(void* f, OggVorbis_File* vf, char* initial, long ibytes)   // 0x00C8CAE0
{
    int ret = _ov_open1(f, vf, initial, ibytes);
    if (ret) return ret;
    return _ov_open2(vf);
}

int __fastcall ov_test_callbacks(void* f, OggVorbis_File* vf, char* initial, long ibytes)   // 0x00C8CB20
{
    return _ov_open1(f, vf, initial, ibytes);
}


// TU scaffolds (not graded): the seek paths (ov_raw_seek / ov_pcm_seek_page / ov_pcm_seek) that call _ov_initset and
// _ov_initprime live outside this TU slice; one visible caller each makes VC7.1 emit the statics.
int _ov_initset_scaffold(OggVorbis_File* vf) { return _ov_initset(vf); }
int _ov_initprime_scaffold(OggVorbis_File* vf) { return _ov_initprime(vf); }

}   // extern "C"
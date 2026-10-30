// libvorbis::_get_prev_page(OggVorbis_File* vf, ogg_page* og) -- vorbisfile.c: step back CHUNKSIZE (8500) bytes at a time from the current offset until a page starts before it, then re-read that last page; returns its file offset, OV_EREAD on a read error, OV_EFAULT if the re-read fails
// Grade: DIFFER(265v269) -- the two `mov eax,esi` before each _get_next_page call are missing: retail _get_next_page takes vf in eax (`push esi ; mov esi,eax`), the same-TU static gets vf in esi (callee-chosen private convention); every other byte matches
// Leaf: _get_prev_page@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C89D70, length 269 bytes; callers _open_seekable2 / _prefetch_all_headers /
//   the raw and pcm seek paths (0x00C8A410, 0x00C8A440, 0x00C8B326, 0x00C8B374, 0x00C8C905).
// Library: libvorbis 1.0.1 lib/vorbisfile.c inside libpk's statically linked Xiph decoder ("Xiph.Org libVorbis I
//   20030909"); no PDB/RTTI, so the name is the libvorbis one. Identity: CHUNKSIZE 0x2134 = 8500, OV_EREAD -128 (the
//   `ret == OV_EREAD` test is `cmp eax,-80h ; cmp edx,-1`), OV_EFAULT -129 (0xFFFFFF7F), the `offset == -1` loop test
//   (`and eax,edx ; cmp eax,-1`), `begin -= CHUNKSIZE; if (begin < 0) begin = 0;`, _seek_helper 0x00C89C80 and
//   _get_next_page 0x00C89CB0 in 1.0.1 order: _get_prev_page verbatim.
// Convention: a C `static` helper with a private register convention: vf in eax, og on the stack, callee-pops (`ret 4`).
//   As for the landed mdct_butterfly_first and this chunk's mdct_butterfly_generic, the static gets retail's registers
//   at plain /O2 when its callees are same-TU statics with bodies and a visible caller sits in the TU: _seek_helper
//   (vf in esi, `ret 8`), _get_next_page (vf in eax, `ret 0Ch`) and its _get_data (vf in esi) are the libpk-adapted
//   vorbisfile bodies of the parked 0x00C89C80 / 0x00C89CB0 / 0x00C89C10 (chunk 445). `__stdcall` on each static gives
//   the callee pop. The caller below is a scaffold (one call, as _open_seekable2 makes it) so the static is emitted with
//   a TU-local convention; only _get_prev_page is graded. C linkage gives `_get_prev_page@8` (retail's static has no
//   public symbol).
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
#pragma auto_inline(on)

ogg_int64_t _open_seekable2_scaffold(OggVorbis_File* vf)    // TU scaffold: one visible caller (not graded)
{
    ogg_page og;
    vf->offset = vf->end;
    return _get_prev_page(vf, &og);
}

}   // extern "C"

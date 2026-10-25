// _get_next_page(OggVorbis_File* vf, ogg_page* og, ogg_int64_t boundary) -- libvorbis 1.0.1 vorbisfile: scan forward for the next page (pulling data as needed, stopping at `boundary` bytes past the current offset when positive); returns the page's file offset, OV_FALSE at the boundary, OV_EOF at the end, OV_EREAD on a read error
// Grade: DIFFER(187v187) -- lane gap: LTCG custom convention (vf arrives in eax, og and boundary on the stack, `ret 0Ch`; its callee _get_data takes vf in esi): retail is an LTCG-private static of vorbisfile.c; a C++ body takes vf in ecx (__fastcall). Body logic otherwise as retail. 1 grade.
// Leaf: @_get_next_page@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C89CB0, length 187 bytes, `ret 0Ch`. Callees: ogg_sync_pageseek
//   0x00C8D510, _get_data 0x00C89C10.
// Source: libvorbis 1.0.1 (20030909) lib/vorbisfile.c _get_next_page, verbatim.
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
extern "C" {
char* __fastcall ogg_sync_buffer(ogg_sync_state* oy, long size);       // 0x00C8D470
int __fastcall ogg_sync_wrote(ogg_sync_state* oy, long bytes);         // 0x00C8D4F0
int __fastcall ogg_sync_reset(ogg_sync_state* oy);                     // 0x00C8D950
long __fastcall ogg_sync_pageseek(ogg_sync_state* oy, struct ogg_page* og);   // 0x00C8D510
}
#define OV_FALSE  -1
#define OV_EOF    -2
#define OV_EREAD  -128

extern "C" long __fastcall _get_data(OggVorbis_File* vf);    // 0x00C89C10

extern "C" ogg_int64_t __fastcall _get_next_page(OggVorbis_File* vf, struct ogg_page* og, ogg_int64_t boundary)
{
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

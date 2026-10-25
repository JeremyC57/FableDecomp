// _get_data(OggVorbis_File* vf) -- libpk vorbisfile: pull the next chunk (at most CHUNKSIZE = 8500 bytes, clamped to what is left of the reader) into the sync buffer; returns the byte count, 0 at the end or without a source, -1 when the read fails
// Grade: DIFFER(133v112) -- lane gap: LTCG custom convention (vf arrives in esi, no stack or ecx argument): retail is an LTCG-private static of vorbisfile.c; a C++ body can only take vf in ecx (__fastcall) or on the stack. Body logic otherwise as retail. 1 grade.
// Leaf: @_get_data@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C89C10, length 112 bytes. Callers: _get_next_page 0x00C89CB0 (and
//   the other vorbisfile statics). Callees: the errno import [0x01440008] (`errno = 0`), ogg_sync_buffer 0x00C8D470,
//   CStreamReader::Tell 0x00C4AA20 / GetSize 0x00C4AA30 / ReadBlock 0x00C4B890, ogg_sync_wrote 0x00C8D4F0.
// Source: libvorbis 1.0.1 (20030909) lib/vorbisfile.c _get_data, with libpk's reader in place of callbacks.read_func.
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
// libpk's stream reader (the OggVorbis_File datasource; COggVorbisStream's +4 member, vtable 0x012B8458).
class CStreamReader {
public:
    unsigned long Tell() const;                        // 0x00C4AA20: block index * block size + offset in block
    unsigned long GetSize() const;                     // 0x00C4AA30: +0x20
    bool Seek(long offset, int origin);                // 0x00C4AFD0: origin 0 = current, 1 = end, 2 = start
    bool ReadBlock(void* buffer, unsigned long size);  // 0x00C4B890
};
extern "C" {
char* __fastcall ogg_sync_buffer(ogg_sync_state* oy, long size);       // 0x00C8D470
int __fastcall ogg_sync_wrote(ogg_sync_state* oy, long bytes);         // 0x00C8D4F0
int __fastcall ogg_sync_reset(ogg_sync_state* oy);                     // 0x00C8D950
long __fastcall ogg_sync_pageseek(ogg_sync_state* oy, struct ogg_page* og);   // 0x00C8D510
}
#include <errno.h>
#define CHUNKSIZE 8500

extern "C" long __fastcall _get_data(OggVorbis_File* vf)
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

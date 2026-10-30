// tagcompare(const char* s1, const char* s2, int n) -- libvorbis info.c: case-insensitive compare of the first n characters (0 when equal, 1 on the first toupper mismatch)
// Leaf: _tagcompare@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8DD30, 88 bytes.
// Retail TU: libpk's statically linked Xiph libvorbis I 20030909 lib/info.c. tagcompare is the file's static; VC7.1 gives it a
//   private convention (s1 in ecx, s2 in eax, n on the stack, `ret 4`) when it is written inside
//   `extern "C" { static __declspec(noinline) __stdcall }` next to its real callers vorbis_comment_query (0x00C8DD90) and
//   vorbis_comment_query_count (0x00C8DE50), both LTCG-API `extern "C" __fastcall`. Parked earlier as "LTCG custom register
//   arguments"; the same-TU static lever (retry1, chunks 585/587/593) matches all three rows from this one TU.
// Callees: MSVCR71 toupper (IAT 0x014400DC), strlen/strcpy/strcat intrinsics, __chkstk 0x00BFEA30 (_alloca), "=" @ 0x0129C1F8.
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);                       // IAT 0x01440158
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // IAT 0x01440058
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);         // IAT 0x01440054
extern "C" __declspec(dllimport) void __cdecl free(void* block);                                // IAT 0x01440154
typedef __int64 ogg_int64_t;

struct oggpack_buffer {                 // 0x14 bytes (libogg bitwise.c)
    long           endbyte;             // +0x00
    int            endbit;              // +0x04
    unsigned char* buffer;              // +0x08
    unsigned char* ptr;                 // +0x0c
    long           storage;             // +0x10
};
struct ogg_packet {                     // 0x20 bytes
    unsigned char* packet;              // +0x00
    long           bytes;               // +0x04
    long           b_o_s;               // +0x08
    long           e_o_s;               // +0x0c
    ogg_int64_t    granulepos;          // +0x10
    ogg_int64_t    packetno;            // +0x18
};
struct vorbis_info {                    // 0x20 bytes
    int   version;                      // +0x00
    int   channels;                     // +0x04
    long  rate;                         // +0x08
    long  bitrate_upper;                // +0x0c
    long  bitrate_nominal;              // +0x10
    long  bitrate_lower;                // +0x14
    long  bitrate_window;               // +0x18
    void* codec_setup;                  // +0x1c codec_setup_info*
};
struct vorbis_info_mode {
    int blockflag;                      // +0x00
    int windowtype;                     // +0x04
    int transformtype;                  // +0x08
    int mapping;                        // +0x0c
};
struct codec_setup_info {               // 0xE80 bytes (vorbis_info_init callocs 0xE80)
    long              blocksizes[2];    // +0x000
    int               modes;            // +0x008
    int               maps;             // +0x00c
    int               floors;           // +0x010
    int               residues;         // +0x014
    int               books;            // +0x018
    int               psys;             // +0x01c
    vorbis_info_mode* mode_param[64];   // +0x020
    unsigned char     _pad120[0xe78 - 0x120];   // map/floor/residue/book tables, psy + bitrate + highlevel setup
    int               halfrate_flag;    // +0xe78 (post-1.0.1 CVS field: painless downsample for decode)
};
struct vorbis_dsp_state {               // 0x70 bytes
    int          analysisp;             // +0x00
    vorbis_info* vi;                    // +0x04
    float**      pcm;                   // +0x08
    float**      pcmret;                // +0x0c
    int          pcm_storage;           // +0x10
    int          pcm_current;           // +0x14
    int          pcm_returned;          // +0x18
    int          preextrapolate;        // +0x1c
    int          eofflag;               // +0x20
    long         lW;                    // +0x24
    long         W;                     // +0x28
    long         nW;                    // +0x2c
    long         centerW;               // +0x30
    ogg_int64_t  granulepos;            // +0x38
    ogg_int64_t  sequence;              // +0x40
    ogg_int64_t  glue_bits;             // +0x48
    ogg_int64_t  time_bits;             // +0x50
    ogg_int64_t  floor_bits;            // +0x58
    ogg_int64_t  res_bits;              // +0x60
    void*        backend_state;         // +0x68 private_state*
};
struct drft_lookup { int n; float* trigcache; int* splitcache; };
struct private_state {                  // backend_lookup_state
    void*          ve;                  // +0x00 envelope_lookup*
    int            window[2];           // +0x04
    void**         transform[2];        // +0x0c
    drft_lookup    fft_look[2];         // +0x14
    int            modebits;            // +0x2c
    void**         flr;                 // +0x30
    void**         residue;             // +0x34
    void*          psy;                 // +0x38
    void*          psy_g_look;          // +0x3c
    unsigned char* header;              // +0x40
    unsigned char* header1;             // +0x44
    unsigned char* header2;             // +0x48
    ogg_int64_t    bms[13];             // +0x50 bitrate_manager_state (not modelled)
    ogg_int64_t    sample_count;        // +0xb8
};
struct alloc_chain {
    void*        ptr;                   // +0x00
    alloc_chain* next;                  // +0x04
};
struct vorbis_block {                   // 0x70 bytes
    float**           pcm;              // +0x00
    oggpack_buffer    opb;              // +0x04
    long              lW;               // +0x18
    long              W;                // +0x1c
    long              nW;               // +0x20
    int               pcmend;           // +0x24
    int               mode;             // +0x28
    int               eofflag;          // +0x2c
    ogg_int64_t       granulepos;       // +0x30
    ogg_int64_t       sequence;         // +0x38
    vorbis_dsp_state* vd;               // +0x40
    void*             localstore;       // +0x44
    long              localtop;         // +0x48
    long              localalloc;       // +0x4c
    long              totaluse;         // +0x50
    alloc_chain*      reap;             // +0x54
    long              glue_bits;        // +0x58
    long              time_bits;        // +0x5c
    long              floor_bits;       // +0x60
    long              res_bits;         // +0x64
    void*             internal;         // +0x68
};
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00 (comments + 1 entries, NULL-terminated)
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;                // +0x00
    int            storage;             // +0x04
    int            fill;                // +0x08
    int            returned;            // +0x0c
    int            unsynced;            // +0x10
    int            headerbytes;         // +0x14
    int            bodybytes;           // +0x18
};

struct ogg_stream_state {               // 0x168 bytes
    unsigned char* body_data;           // +0x000 bytes from packet bodies
    long           body_storage;        // +0x004 storage elements allocated
    long           body_fill;           // +0x008 elements stored; fill mark
    long           body_returned;       // +0x00c elements of fill returned
    int*           lacing_vals;         // +0x010 the values that will go to the segment table
    ogg_int64_t*   granule_vals;        // +0x014 granulepos values for headers
    long           lacing_storage;      // +0x018
    long           lacing_fill;         // +0x01c
    long           lacing_packet;       // +0x020
    long           lacing_returned;     // +0x024
    unsigned char  header[282];         // +0x028 working space for header encode
    int            header_fill;         // +0x144
    int            e_o_s;               // +0x148
    int            b_o_s;               // +0x14c
    long           serialno;            // +0x150
    long           pageno;              // +0x154
    ogg_int64_t    packetno;            // +0x158
    ogg_int64_t    granulepos;          // +0x160
};
struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};
enum ESeekOrigin { SEEK_ORIGIN_CURRENT = 0, SEEK_ORIGIN_END = 1, SEEK_ORIGIN_START = 2 };
struct PKCBufferedReader {              // libpk reader that replaces the stdio ov_callbacks
    bool ConvertOffsetToAbsolute(long offset, ESeekOrigin origin);   // 0x00C4AFD0 (the seek callback)
    unsigned long GetPosition();                                     // 0x00C4AA20 (the tell callback)
};
struct OggVorbis_File {                 // 0x2C0 bytes (libpk fork: no ov_callbacks member)
    PKCBufferedReader* datasource;      // +0x000
    int              seekable;          // +0x004
    ogg_int64_t      offset;            // +0x008
    ogg_int64_t      end;               // +0x010
    ogg_sync_state   oy;                // +0x018
    int              links;             // +0x034
    ogg_int64_t*     offsets;           // +0x038
    ogg_int64_t*     dataoffsets;       // +0x03c
    long*            serialnos;         // +0x040
    ogg_int64_t*     pcmlengths;        // +0x044
    vorbis_info*     vi;                // +0x048
    vorbis_comment*  vc;                // +0x04c
    ogg_int64_t      pcm_offset;        // +0x050
    int              ready_state;       // +0x058
    long             current_serialno;  // +0x05c
    int              current_link;      // +0x060
    double           bittrack;          // +0x068
    double           samptrack;         // +0x070
    ogg_stream_state os;                // +0x078
    vorbis_dsp_state vd;                // +0x1e0
    vorbis_block     vb;                // +0x250
};

#define NOTOPEN   0
#define PARTOPEN  1
#define OPENED    2
#define STREAMSET 3
#define INITSET   4
#define OV_HOLE   -3
#define OV_EREAD  -128
#define OV_EIMPL  -130
#define OV_EINVAL -131
#define OV_ENOTVORBIS -132
#define OV_EBADHEADER -133
#define OV_EVERSION   -134
extern "C" __declspec(dllimport) int __cdecl toupper(int c);   // IAT 0x014400DC

#include <malloc.h>
extern "C" {
static __declspec(noinline) int __stdcall tagcompare(const char* s1, const char* s2, int n)
{
    int c = 0;
    while (c < n) {
        if (toupper(s1[c]) != toupper(s2[c]))
            return !0;
        c++;
    }
    return 0;
}

char* __fastcall vorbis_comment_query(vorbis_comment* vc, char* tag, int count)
{
    long i;
    int found = 0;
    int taglen = strlen(tag) + 1; /* +1 for the = we append */
    char* fulltag = (char*)_alloca(taglen + 1);

    strcpy(fulltag, tag);
    strcat(fulltag, "=");

    for (i = 0; i < vc->comments; i++) {
        if (!tagcompare(vc->user_comments[i], fulltag, taglen)) {
            if (count == found)
                /* We return a pointer to the data, not a copy */
                return vc->user_comments[i] + taglen;
            else
                found++;
        }
    }
    return 0; /* didn't find anything */
}

int __fastcall vorbis_comment_query_count(vorbis_comment* vc, char* tag)
{
    int i, count = 0;
    int taglen = strlen(tag) + 1; /* +1 for the = we append */
    char* fulltag = (char*)_alloca(taglen + 1);
    strcpy(fulltag, tag);
    strcat(fulltag, "=");

    for (i = 0; i < vc->comments; i++) {
        if (!tagcompare(vc->user_comments[i], fulltag, taglen))
            count++;
    }

    return count;
}
}
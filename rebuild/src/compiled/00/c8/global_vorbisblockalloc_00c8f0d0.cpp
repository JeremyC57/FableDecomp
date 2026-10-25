// _vorbis_block_alloc(vorbis_block* vb, long bytes) -- libvorbis block.c: carve an 8-aligned chunk from the block's local arena; when it does not fit, retire the old store onto the reap chain and malloc a fresh one of exactly that size
// Leaf: _vorbis_block_alloc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8F0D0, length 104 bytes (112 with padding).
// Library: libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I 20030909"; a post-1.0.1 CVS tree: it has
// codec_setup_info::halfrate_flag). No PDB/RTTI: the name is the libvorbis API name.
// Convention: LTCG __fastcall (vb in ecx, bytes in edx). Callee: MSVCR71 malloc (IAT 0x01440158, cached in ebx).
// Shape: the libvorbis body verbatim (WORD_ALIGN 8: `lea edi,[edx+7] ; and edi,-8`).
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // IAT 0x01440158
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

#define WORD_ALIGN 8

extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block* vb, long bytes)
{
    bytes = (bytes + (WORD_ALIGN - 1)) & ~(WORD_ALIGN - 1);
    if (bytes + vb->localtop > vb->localalloc) {
        /* can't just _ogg_realloc... there are outstanding pointers */
        if (vb->localstore) {
            struct alloc_chain* link = (alloc_chain*)malloc(sizeof(*link));
            vb->totaluse += vb->localtop;
            link->next = vb->reap;
            link->ptr = vb->localstore;
            vb->reap = link;
        }
        /* highly conservative */
        vb->localalloc = bytes;
        vb->localstore = malloc(vb->localalloc);
        vb->localtop = 0;
    }
    {
        void* ret = (void*)(((char*)vb->localstore) + vb->localtop);
        vb->localtop += bytes;
        return ret;
    }
}
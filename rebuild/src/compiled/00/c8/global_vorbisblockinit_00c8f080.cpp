// vorbis_block_init(vorbis_dsp_state* v, vorbis_block* vb) -- Xiph libvorbis block.c: clear the 0x70-byte vorbis_block and bind it to its DSP state; for an analysis state calloc the 0x48-byte vorbis_block_internal, init the packer and seed ampmax = -9999
// Leaf: @vorbis_block_init@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8F080, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); v arrives in ecx and vb in edx (`__fastcall`); `rep stosd` is the 28-word memset of vorbis_block; vd (+0x40) = v, localalloc (+0x4c) = 0, localstore (+0x44) = 0 in block.c order; when v->analysisp (+0) the internal block (+0x68) is calloc(1, 0x48) (IAT 0x01440058), oggpack_writeinit 0x00C90F10 runs on vb->opb (+4) and ampmax (+4 of the internal block) = -9999 -- the block.c body of this CVS tree (agent446 names oggpack_writeinit, bound @oggpack_writeinit@4); it precedes _vorbis_block_alloc 0x00C8F0D0.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was global InitStreamEntry_00c8f080, a propagated game-side name over an invented model.
#include <string.h>
typedef __int64 ogg_int64_t;
struct oggpack_buffer {                 // 0x14 bytes
    long           endbyte;             // +0x00
    int            endbit;              // +0x04
    unsigned char* buffer;              // +0x08
    unsigned char* ptr;                 // +0x0c
    long           storage;             // +0x10
};
struct vorbis_dsp_state {
    int analysisp;                      // +0x00
};
struct vorbis_block_internal {          // 0x48 bytes
    float**       pcmdelay;             // +0x00
    float         ampmax;               // +0x04
    int           blocktype;            // +0x08
    unsigned long packetblob_markers[15];
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
    void*             reap;             // +0x54
    long              glue_bits;        // +0x58
    long              time_bits;        // +0x5c
    long              floor_bits;       // +0x60
    long              res_bits;         // +0x64
    void*             internal;         // +0x68
};
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // MSVCR71 IAT 0x01440058
extern "C" void __fastcall oggpack_writeinit(oggpack_buffer* b);   // 0x00C90F10

extern "C" int __fastcall vorbis_block_init(vorbis_dsp_state* v, vorbis_block* vb)
{
    memset(vb, 0, sizeof(*vb));
    vb->vd = v;
    vb->localalloc = 0;
    vb->localstore = 0;
    if (v->analysisp) {
        vorbis_block_internal* vbi =
            (vorbis_block_internal*)(vb->internal = calloc(1, sizeof(vorbis_block_internal)));
        oggpack_writeinit(&vb->opb);
        vbi->ampmax = -9999;
    }
    return (0);
}

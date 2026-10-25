// libogg::ogg_stream_clear(ogg_stream_state* os) -- libogg framing: free the body buffer and the lacing / granule fifos and zero the stream state (null-safe)
// Leaf: @ogg_stream_clear@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CE80, length 66 bytes (80 with padding).
// Library: libpk's statically linked Xiph libogg (framing.c, libVorbis I 20030909 era). No PDB/RTTI: libogg API name.
// Convention: LTCG __fastcall (os in ecx). Callee: MSVCR71 free (IAT 0x01440154, cached in esi).
// Shape: the libogg body verbatim; three null-tested frees, then the 0x168-byte memset (`rep stosd` x 0x5A).
#include <string.h>
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // IAT 0x01440154
typedef __int64 ogg_int64_t;

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

extern "C" int __fastcall ogg_stream_clear(ogg_stream_state* os)
{
    if (os) {
        if (os->body_data) free(os->body_data);
        if (os->lacing_vals) free(os->lacing_vals);
        if (os->granule_vals) free(os->granule_vals);

        memset(os, 0, sizeof(*os));
    }
    return (0);
}
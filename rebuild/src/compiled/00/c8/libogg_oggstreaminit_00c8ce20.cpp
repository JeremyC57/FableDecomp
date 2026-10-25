// libogg::ogg_stream_init(ogg_stream_state* os, int serialno) -- libogg framing: zero the stream state, allocate the 16 KB body buffer and the 1024-entry lacing and granule fifos, and record the serial number; -1 on a null state
// Leaf: @ogg_stream_init@8
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CE20, length 94 bytes (96 with padding).
// Library: libpk's statically linked Xiph libogg (framing.c, libVorbis I 20030909 era). No PDB/RTTI: libogg API name.
// Convention: LTCG __fastcall (os in ecx, serialno in edx). Callee: MSVCR71 malloc (IAT 0x01440158, cached in edi).
// Shape: the libogg body verbatim. sizeof(ogg_stream_state) = 0x168 (`rep stosd` x 0x5A). The first two sizes fold
// to constants (the storage word was just stored), the third re-reads lacing_storage after the second malloc call.
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // IAT 0x01440158
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

extern "C" int __fastcall ogg_stream_init(ogg_stream_state* os, int serialno)
{
    if (os) {
        memset(os, 0, sizeof(*os));
        os->body_storage = 16 * 1024;
        os->body_data = (unsigned char*)malloc(os->body_storage * sizeof(*os->body_data));

        os->lacing_storage = 1024;
        os->lacing_vals = (int*)malloc(os->lacing_storage * sizeof(*os->lacing_vals));
        os->granule_vals = (ogg_int64_t*)malloc(os->lacing_storage * sizeof(*os->granule_vals));

        os->serialno = serialno;

        return (0);
    }
    return (-1);
}
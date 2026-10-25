// ogg_packet_clear(ogg_packet* op) -- Xiph libogg framing.c: free the packet's data and zero the 0x20-byte ogg_packet
// Leaf: @ogg_packet_clear@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DB30, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); free(op->packet) through the MSVCR71 IAT (0x01440154), then the 32-byte memset of the record, the framing.c body; ogg_packet is {packet, bytes, b_o_s, e_o_s, granulepos, packetno} = 0x20 bytes.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was SBufferRecord_00c8db30::Clear, a propagated game-side name over an invented model.
#include <string.h>
typedef __int64 ogg_int64_t;
struct ogg_packet {                     // 0x20 bytes
    unsigned char* packet;              // +0x00
    long           bytes;               // +0x04
    long           b_o_s;               // +0x08
    long           e_o_s;               // +0x0c
    ogg_int64_t    granulepos;          // +0x10
    ogg_int64_t    packetno;            // +0x18
};
extern "C" __declspec(dllimport) void __cdecl free(void* block);       // MSVCR71 IAT 0x01440154 (`call dword ptr [__imp__free]`)

extern "C" void __fastcall ogg_packet_clear(ogg_packet* op)
{
    free(op->packet);
    memset(op, 0, sizeof(*op));
}

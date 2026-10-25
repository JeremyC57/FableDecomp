// libogg::ogg_stream_destroy(ogg_stream_state* os) -- Xiph libogg framing.c: ogg_stream_clear the stream state, then free it; returns 0
// Leaf: @ogg_stream_destroy@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CED0, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); it calls ogg_stream_clear 0x00C8CE80 (agent446, bound @ogg_stream_clear@4) then MSVCR71 free (IAT 0x01440154) on the same pointer when non-null and returns 0, the framing.c body; it follows ogg_stream_clear.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was CTCMapwho_OnDie (a `__fastcall` free function), a propagated game-side name over an invented model.
typedef __int64 ogg_int64_t;
struct ogg_stream_state {               // 0x168 bytes
    unsigned char* body_data;           // +0x000
    long           body_storage;        // +0x004
    long           body_fill;           // +0x008
    long           body_returned;       // +0x00c
    int*           lacing_vals;         // +0x010
    ogg_int64_t*   granule_vals;        // +0x014
    long           lacing_storage;      // +0x018
    long           lacing_fill;         // +0x01c
    long           lacing_packet;       // +0x020
    long           lacing_returned;     // +0x024
    unsigned char  header[282];         // +0x028
    int            header_fill;         // +0x144
    int            e_o_s;               // +0x148
    int            b_o_s;               // +0x14c
    long           serialno;            // +0x150
    long           pageno;              // +0x154
    ogg_int64_t    packetno;            // +0x158
    ogg_int64_t    granulepos;          // +0x160
};
extern "C" __declspec(dllimport) void __cdecl free(void* block);       // MSVCR71 IAT 0x01440154 (`call dword ptr [__imp__free]`)
extern "C" int __fastcall ogg_stream_clear(ogg_stream_state* os);   // 0x00C8CE80

extern "C" int __fastcall ogg_stream_destroy(ogg_stream_state* os)
{
    if (os) {
        ogg_stream_clear(os);
        free(os);
    }
    return (0);
}

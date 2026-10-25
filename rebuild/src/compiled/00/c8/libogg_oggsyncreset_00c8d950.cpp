// libogg::ogg_sync_reset(ogg_sync_state* oy) -- Xiph libogg framing.c: clear the sync state's fill/returned/unsynced/headerbytes/bodybytes; returns 0
// Leaf: @ogg_sync_reset@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D950, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); it zeroes +0x08..+0x18 of the 0x1c-byte ogg_sync_state (fill, returned, unsynced, headerbytes, bodybytes) and returns 0 in the zero register, the framing.c body; it precedes ogg_stream_reset 0x00C8D970.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was global fn_00c8d950 (Close), a propagated game-side name over an invented model.
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;                // +0x00
    int            storage;             // +0x04
    int            fill;                // +0x08
    int            returned;            // +0x0c
    int            unsynced;            // +0x10
    int            headerbytes;         // +0x14
    int            bodybytes;           // +0x18
};

extern "C" int __fastcall ogg_sync_reset(ogg_sync_state* oy)
{
    oy->fill = 0;
    oy->returned = 0;
    oy->unsynced = 0;
    oy->headerbytes = 0;
    oy->bodybytes = 0;
    return (0);
}

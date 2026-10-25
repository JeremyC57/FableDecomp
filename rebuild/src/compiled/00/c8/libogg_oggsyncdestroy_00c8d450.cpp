// libogg::ogg_sync_destroy(ogg_sync_state* oy) -- Xiph libogg framing.c: ogg_sync_clear the sync state, then free it; returns 0
// Leaf: @ogg_sync_destroy@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D450, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); it calls ogg_sync_clear 0x00C8D420 (agent446, bound @ogg_sync_clear@4) then MSVCR71 free (IAT 0x01440154) when non-null and returns 0, the framing.c body; it follows ogg_sync_clear and precedes ogg_sync_buffer 0x00C8D470.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was CTCMapwho::OnDie, a propagated game-side name over an invented model.
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;                // +0x00
    int            storage;             // +0x04
    int            fill;                // +0x08
    int            returned;            // +0x0c
    int            unsynced;            // +0x10
    int            headerbytes;         // +0x14
    int            bodybytes;           // +0x18
};
extern "C" __declspec(dllimport) void __cdecl free(void* block);       // MSVCR71 IAT 0x01440154 (`call dword ptr [__imp__free]`)
extern "C" int __fastcall ogg_sync_clear(ogg_sync_state* oy);   // 0x00C8D420

extern "C" int __fastcall ogg_sync_destroy(ogg_sync_state* oy)
{
    if (oy) {
        ogg_sync_clear(oy);
        free(oy);
    }
    return (0);
}

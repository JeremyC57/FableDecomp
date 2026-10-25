// ogg_sync_wrote(ogg_sync_state* oy, long bytes) -- Xiph libogg framing.c: commit `bytes` written into the sync buffer: -1 when that overruns the storage, else advance fill and return 0
// Leaf: @ogg_sync_wrote@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D4F0, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); `fill(+8) + bytes > storage(+4)` returns -1, else fill += bytes, return 0 -- the framing.c body; it follows ogg_sync_buffer 0x00C8D470 (agent446).
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was CTCInventoryBase::GetSelectedCategory (FableGetSel_c8d4f0), a propagated game-side name over an invented model.
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;                // +0x00
    int            storage;             // +0x04
    int            fill;                // +0x08
    int            returned;            // +0x0c
    int            unsynced;            // +0x10
    int            headerbytes;         // +0x14
    int            bodybytes;           // +0x18
};

extern "C" int __fastcall ogg_sync_wrote(ogg_sync_state* oy, long bytes)
{
    if (oy->fill + bytes > oy->storage) return (-1);
    oy->fill += bytes;
    return (0);
}

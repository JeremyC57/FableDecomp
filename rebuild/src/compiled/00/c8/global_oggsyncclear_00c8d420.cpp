// ogg_sync_clear(ogg_sync_state* oy) -- libogg framing: free the sync buffer and re-initialise (zero) the sync state; always returns 0
// Leaf: ogg_sync_clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D420, length 34 bytes (48 with padding).
// Library: libpk's statically linked Xiph libogg (framing.c, libVorbis I 20030909 era). No PDB/RTTI: libogg API name.
// Convention: LTCG __fastcall (oy in ecx). Callees: MSVCR71 free (IAT 0x01440154); ogg_sync_init 0x00C8D400
// (LTCG __fastcall, oy in ecx: `if (oy) memset(oy, 0, sizeof(*oy)); return 0;`, not inlined by the retail link).
// Shape: the libogg body verbatim.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // IAT 0x01440154
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;                // +0x00
    int            storage;             // +0x04
    int            fill;                // +0x08
    int            returned;            // +0x0c
    int            unsynced;            // +0x10
    int            headerbytes;         // +0x14
    int            bodybytes;           // +0x18
};

extern "C" int __fastcall ogg_sync_init(ogg_sync_state* oy);       // 0x00C8D400

extern "C" int __fastcall ogg_sync_clear(ogg_sync_state* oy)
{
    if (oy) {
        if (oy->data) free(oy->data);
        ogg_sync_init(oy);
    }
    return (0);
}
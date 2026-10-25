// libogg::ogg_sync_init(ogg_sync_state* oy) -- libogg framing.c: zero a sync state (null-safe); always returns 0
// Leaf: @ogg_sync_init@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8D400, 27 bytes (+5 int3). libogg src/framing.c inside libpk's statically linked
//   Xiph Ogg Vorbis ("Xiph.Org libVorbis I 20030909"); the LTCG link made the API `__fastcall` (oy in ecx, plain `ret`).
// Identity: the next function, ogg_sync_clear 0x00C8D420 (agent446), calls this address with oy in ecx after freeing
//   oy->data; ogg_sync_state is 7 ints (0x1c bytes), exactly the seven stores. No callees.
// Shape: the shipped source; memset(oy,0,sizeof(*oy)) is the /O2 intrinsic, and the zero in eax doubles as the `return 0`
//   (the `xor eax,eax` comes before the null test).
// Label: manifest "allocator<std::vector<std::pair<long...>>>::construct" is propagated; the real name is the libogg API.
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)

typedef struct {
    unsigned char* data;        // +0x00
    int storage;                // +0x04
    int fill;                   // +0x08
    int returned;               // +0x0c
    int unsynced;               // +0x10
    int headerbytes;            // +0x14
    int bodybytes;              // +0x18
} ogg_sync_state;

extern "C" int __fastcall ogg_sync_init(ogg_sync_state* oy)
{
    if (oy) {
        memset(oy, 0, sizeof(*oy));
    }
    return (0);
}
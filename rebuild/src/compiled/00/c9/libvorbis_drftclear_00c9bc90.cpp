// libvorbis::drft_clear(drft_lookup* l) -- free a real-FFT lookup's trig and split caches and wipe it
// Leaf: drft_clear
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9BC90, 53 bytes. libvorbis 1.0.1 lib/smallft.c, compiled as C with /Gr:
//   `__fastcall`, l in ecx, plain `ret`.
// Callees: free (MSVCR71 import [0x01440154], hoisted into edi) through _ogg_free.
// Layout: libvorbis 1.0.1 smallft.h drft_lookup {n, trigcache, splitcache} (12 bytes).
// Shape: the shipped source; memset(l,0,12) expands to `xor eax,eax` + three stores.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef struct drft_lookup {
    int    n;
    float* trigcache;
    int*   splitcache;
} drft_lookup;

extern "C" void __fastcall drft_clear(drft_lookup* l)
{
    if (l) {
        if (l->trigcache) _ogg_free(l->trigcache);
        if (l->splitcache) _ogg_free(l->splitcache);
        memset(l, 0, sizeof(*l));
    }
}
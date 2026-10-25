// libvorbis::mdct_clear(mdct_lookup* l) -- free an MDCT lookup's twiddle and bit-reverse tables and wipe it
// Leaf: mdct_clear
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9C4C0, 59 bytes. libvorbis 1.0.1 lib/mdct.c, compiled as C with /Gr:
//   `__fastcall`, l in ecx, plain `ret`.
// Callees: free (MSVCR71 import [0x01440154], hoisted into edi) through _ogg_free.
// Layout: libvorbis 1.0.1 mdct.h mdct_lookup {n, log2n, trig, bitrev, scale} (20 bytes, DATA_TYPE float).
// Shape: the shipped source; memset(l,0,20) expands to `xor eax,eax` + five stores.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef float DATA_TYPE;
typedef struct mdct_lookup {
    int        n;
    int        log2n;
    DATA_TYPE* trig;
    int*       bitrev;
    DATA_TYPE  scale;
} mdct_lookup;

extern "C" void __fastcall mdct_clear(mdct_lookup* l)
{
    if (l) {
        if (l->trig) _ogg_free(l->trig);
        if (l->bitrev) _ogg_free(l->bitrev);
        memset(l, 0, sizeof(*l));
    }
}
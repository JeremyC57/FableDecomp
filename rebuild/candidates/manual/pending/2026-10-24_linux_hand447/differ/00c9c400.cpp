// libvorbis::mdct_butterflies(mdct_lookup* init, DATA_TYPE* x, int points) -- run the MDCT butterfly stages (first, generic per stage, then the 32-point tail)
// Grade: DIFFER(174v185) -- LTCG register arguments: retail takes init in EAX (x, points on the stack, `ret 8`) and calls three LTCG helpers (mdct_butterfly_first 0x00C9C1E0 with x in edi/ecx, mdct_butterfly_generic 0x00C9C2F0 with ecx/edx + one push, mdct_butterfly_32 0x00C9C000 with eax)
// Retail: Fable.exe @ 0x00C9C400, 185 bytes. libvorbis 1.0.1 lib/mdct.c (STIN, out of line under LTCG). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: mdct_butterflies
typedef float DATA_TYPE;
typedef struct mdct_lookup { int n; int log2n; DATA_TYPE* trig; int* bitrev; DATA_TYPE scale; } mdct_lookup;
extern "C" void __fastcall mdct_butterfly_first(DATA_TYPE* T, DATA_TYPE* x, int points);                 // 0x00C9C1E0
extern "C" void __fastcall mdct_butterfly_generic(DATA_TYPE* T, DATA_TYPE* x, int points, int trigint);  // 0x00C9C2F0
extern "C" void __fastcall mdct_butterfly_32(DATA_TYPE* x);                                               // 0x00C9C000
extern "C" void __fastcall mdct_butterflies(mdct_lookup* init, DATA_TYPE* x, int points)
{
    DATA_TYPE* T = init->trig;
    int stages = init->log2n - 5;
    int i, j;

    if (--stages > 0) {
        mdct_butterfly_first(T, x, points);
    }

    for (i = 1; --stages > 0; i++) {
        for (j = 0; j < (1 << i); j++)
            mdct_butterfly_generic(T, x + (points >> i) * j, points >> i, 4 << i);
    }

    for (j = 0; j < points; j += 32)
        mdct_butterfly_32(x + j);
}

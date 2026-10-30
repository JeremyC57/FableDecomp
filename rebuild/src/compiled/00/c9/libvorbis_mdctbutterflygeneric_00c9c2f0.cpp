// libvorbis::mdct_butterfly_generic(float* T, float* x, int points, int trigint) -- mdct.c: N/stage-point generic MDCT butterfly (in place, two registers): walks x from the top half down in blocks of 8 floats, adding the upper half into the lower and rotating the differences by the trig table T, stepping T by trigint per pair
// Leaf: _mdct_butterfly_generic@16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C9C2F0, length 269 bytes; sole caller mdct_butterflies 0x00C9C400
// (the inner `for (j...)` loop, after the mdct_butterfly_first 0x00C9C1E0 call).
// Library: libvorbis 1.0.1 lib/mdct.c inside libpk's statically linked Xiph decoder ("Xiph.Org libVorbis I 20030909");
// no PDB/RTTI, so the name is the libvorbis one. Identity: the block offsets x1[6..7], x1[4..5], x1[2..3], x1[0..1]
// against x2[..], each pair rotated by T[0]/T[1] and followed by `T += trigint` (the stack argument, scaled by 4 into
// esi once), then x1 -= 8, x2 -= 8, while (x2 >= x): mdct_butterfly_generic verbatim, the 4-argument sibling of the
// landed mdct_butterfly_first.
// Convention: a C `static` (STIN) helper, so VC7.1 gives it a private register convention: points in ecx, T in edx,
// x in ebx, trigint on the stack (`ret 4`). As for the landed mdct_butterfly_first (chunk 585), a same-TU
// `static __declspec(noinline)` body with its visible caller mdct_butterflies in the TU gets exactly these registers at
// the plain lane /O2 (no /GL). The caller is written extern "C" non-static so it is emitted (static in retail);
// mdct_butterfly_first / mdct_butterfly_32 are declared only. Retail's private convention is callee-pops (`ret 4`
// for trigint, as LTCG private conventions are; the caller pushes trigint at 0x00C9C464 and does not pop it): a plain
// static gave `ret` (267v269); `__stdcall` on the static keeps the private registers and adds the callee pop (MATCH).
// C linkage gives the C symbol `_mdct_butterfly_generic@16` (retail's static has no public symbol).
#define DATA_TYPE float
#define REG_TYPE float
#define MULT_NORM(x) (x)

typedef struct {
    int n;
    int log2n;
    DATA_TYPE* trig;
    int* bitrev;
    DATA_TYPE scale;
} mdct_lookup;

extern "C" {

void mdct_butterfly_first(DATA_TYPE* T, DATA_TYPE* x, int points);   // 0x00C9C1E0 (static in retail; declared only)
void mdct_butterfly_32(DATA_TYPE* x);                                 // 0x00C9C000 (static in retail; declared only)

#pragma auto_inline(off)
static __declspec(noinline) void __stdcall mdct_butterfly_generic(DATA_TYPE* T, DATA_TYPE* x, int points, int trigint)
{
    DATA_TYPE* x1 = x + points - 8;
    DATA_TYPE* x2 = x + (points >> 1) - 8;
    REG_TYPE r0;
    REG_TYPE r1;

    do {
        r0 = x1[6] - x2[6];
        r1 = x1[7] - x2[7];
        x1[6] += x2[6];
        x1[7] += x2[7];
        x2[6] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[7] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;

        r0 = x1[4] - x2[4];
        r1 = x1[5] - x2[5];
        x1[4] += x2[4];
        x1[5] += x2[5];
        x2[4] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[5] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;

        r0 = x1[2] - x2[2];
        r1 = x1[3] - x2[3];
        x1[2] += x2[2];
        x1[3] += x2[3];
        x2[2] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[3] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;

        r0 = x1[0] - x2[0];
        r1 = x1[1] - x2[1];
        x1[0] += x2[0];
        x1[1] += x2[1];
        x2[0] = MULT_NORM(r1 * T[1] + r0 * T[0]);
        x2[1] = MULT_NORM(r1 * T[0] - r0 * T[1]);

        T += trigint;
        x1 -= 8;
        x2 -= 8;

    } while (x2 >= x);
}
#pragma auto_inline(on)

void mdct_butterflies(mdct_lookup* init, DATA_TYPE* x, int points)   // 0x00C9C400 (static in retail, init in eax)
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

}   // extern "C"
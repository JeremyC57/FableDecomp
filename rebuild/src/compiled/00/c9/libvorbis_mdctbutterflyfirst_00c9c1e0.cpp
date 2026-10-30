// libvorbis::mdct_butterfly_first(float* T, float* x, int points) -- mdct.c: first-stage N-point MDCT butterfly (in place, two registers): walks x from the top half down in blocks of 8 floats, adding the upper half into the lower and rotating the differences by the trig table T (16 floats per block)
// Leaf: _mdct_butterfly_first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C9C1E0, length 259 bytes; sole caller mdct_butterflies 0x00C9C400
// (call at 0x00C9C421).
// Library: libvorbis 1.0.1 lib/mdct.c inside libpk's statically linked Xiph decoder (0xC908E0..0xC9FDF0, "Xiph.Org
// libVorbis I 20030909"); no PDB/RTTI, so the name is the libvorbis one. Identity: the block offsets (x1[6..7], x1[4..5],
// x1[2..3], x1[0..1] against x2[..]; T[0,1], T[4,5], T[8,9], T[12,13]; x1-=8, x2-=8, T+=16, while(x2>=x)) are
// mdct_butterfly_first verbatim, and its caller 0x00C9C400 is mdct_butterflies (`stages = log2n-5; if(--stages>0)`
// then this call, then the mdct_butterfly_generic 0x00C9C2F0 double loop and the mdct_butterfly_32 0x00C9C000 loop).
// Convention: a C `static` (STIN) helper, so VC7.1 gives it a private register convention: points in ecx, x in edi,
// T in edx, plain `ret`. This is NOT an LTCG-only shape: a same-TU `static __declspec(noinline)` body called from a
// visible caller in the TU gets exactly retail's registers under the plain lane /O2 (MATCH, no /GL). The caller here is
// mdct_butterflies verbatim, written extern "C" non-static so it is emitted (in retail it is static too, init in eax);
// mdct_butterfly_generic / mdct_butterfly_32 are declared only (statics with bodies in retail; not graded here).
// C linkage (the extern "C" block) gives the C symbol `_mdct_butterfly_first`.
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

void mdct_butterfly_generic(DATA_TYPE* T, DATA_TYPE* x, int points, int trigint);   // 0x00C9C2F0 (static in retail; declared only)
void mdct_butterfly_32(DATA_TYPE* x);                                                 // 0x00C9C000 (static in retail; declared only)

#pragma auto_inline(off)
static __declspec(noinline) void mdct_butterfly_first(DATA_TYPE* T, DATA_TYPE* x, int points)
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

        r0 = x1[4] - x2[4];
        r1 = x1[5] - x2[5];
        x1[4] += x2[4];
        x1[5] += x2[5];
        x2[4] = MULT_NORM(r1 * T[5] + r0 * T[4]);
        x2[5] = MULT_NORM(r1 * T[4] - r0 * T[5]);

        r0 = x1[2] - x2[2];
        r1 = x1[3] - x2[3];
        x1[2] += x2[2];
        x1[3] += x2[3];
        x2[2] = MULT_NORM(r1 * T[9] + r0 * T[8]);
        x2[3] = MULT_NORM(r1 * T[8] - r0 * T[9]);

        r0 = x1[0] - x2[0];
        r1 = x1[1] - x2[1];
        x1[0] += x2[0];
        x1[1] += x2[1];
        x2[0] = MULT_NORM(r1 * T[13] + r0 * T[12]);
        x2[1] = MULT_NORM(r1 * T[12] - r0 * T[13]);

        x1 -= 8;
        x2 -= 8;
        T += 16;

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
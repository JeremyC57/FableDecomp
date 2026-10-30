// libvorbis::mdct_bitreverse(mdct_lookup* init, float* x) -- libvorbis 1.0.1 mdct.c mdct_bitreverse: the bit-reversal pass of the MDCT, pairing bit-reversed elements of the upper half, rotating by the trig table and writing the outputs from both ends of x
// Leaf: _mdct_bitreverse
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9C500, 282 bytes. libvorbis 1.0.1 lib/mdct.c inside libpk's Xiph decoder
// ("Xiph.Org libVorbis I 20030909"); no PDB/RTTI, so the name is the libvorbis one. It follows the landed
// mdct_butterfly_generic 0x00C9C2F0 / mdct_butterflies 0x00C9C400 and mdct_clear 0x00C9C4C0 in source order.
// Callers: mdct_backward (call at 0x00C9C71E, right after its mdct_butterflies call) and mdct_forward (call at 0x00C9C9F9),
// both passing init in eax and x in edx: VC7.1's private register convention for a STIN (static) helper with a same-TU
// caller (chunk 585 rule). Identity: n>>1 offsets into x, the bitrev table (+0xC) read four at a time, T = trig + n, the
// HALVE (x * .5f, the float literal at 0x0122F59C) of the sums and differences, the w0[0..3] / w1[0..3] stores and
// `while (w0 < w1)`: mdct_bitreverse verbatim. The same-TU caller is the head of 1.0.1 mdct_backward's call sequence.
#define DATA_TYPE float
#define REG_TYPE float
#define MULT_NORM(x) (x)
#define HALVE(x) ((x)*.5f)

typedef struct {
    int n;
    int log2n;
    DATA_TYPE* trig;
    int* bitrev;
    DATA_TYPE scale;
} mdct_lookup;

extern "C" {

void mdct_butterflies(mdct_lookup* init, DATA_TYPE* x, int points);   // 0x00C9C400 (static in retail; declared only)

static __declspec(noinline) void mdct_bitreverse(mdct_lookup* init, DATA_TYPE* x)
{
    int        n   = init->n;
    int*       bit = init->bitrev;
    DATA_TYPE* w0  = x;
    DATA_TYPE* w1  = x = w0 + (n >> 1);
    DATA_TYPE* T   = init->trig + n;

    do {
        DATA_TYPE* x0 = x + bit[0];
        DATA_TYPE* x1 = x + bit[1];

        REG_TYPE r0 = x0[1] - x1[1];
        REG_TYPE r1 = x0[0] + x1[0];
        REG_TYPE r2 = MULT_NORM(r1 * T[0] + r0 * T[1]);
        REG_TYPE r3 = MULT_NORM(r1 * T[1] - r0 * T[0]);

        w1 -= 4;

        r0 = HALVE(x0[1] + x1[1]);
        r1 = HALVE(x0[0] - x1[0]);

        w0[0] = r0 + r2;
        w1[2] = r0 - r2;
        w0[1] = r1 + r3;
        w1[3] = r3 - r1;

        x0 = x + bit[2];
        x1 = x + bit[3];

        r0 = x0[1] - x1[1];
        r1 = x0[0] + x1[0];
        r2 = MULT_NORM(r1 * T[2] + r0 * T[3]);
        r3 = MULT_NORM(r1 * T[3] - r0 * T[2]);

        r0 = HALVE(x0[1] + x1[1]);
        r1 = HALVE(x0[0] - x1[0]);

        w0[2] = r0 + r2;
        w1[0] = r0 - r2;
        w0[3] = r1 + r3;
        w1[1] = r3 - r1;

        T   += 4;
        bit += 4;
        w0  += 4;

    } while (w0 < w1);
}

// Same-TU caller: the start of 1.0.1 mdct_backward (rotate, butterflies, bitreverse); only the static leaf is graded.
void mdct_backward(mdct_lookup* init, DATA_TYPE* in, DATA_TYPE* out)
{
    int n  = init->n;
    int n2 = n >> 1;

    mdct_butterflies(init, out + n2, n2);
    mdct_bitreverse(init, out);
    in[0] = out[0];
}

}   // extern "C"
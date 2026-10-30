// libvorbis::mdct_butterfly_8(DATA_TYPE* x) -- the 8-point MDCT butterfly stage
// Leaf: _mdct_butterfly_8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9BEB0, 125 bytes: libvorbis I 20030909 lib/mdct.c `STIN void mdct_butterfly_8(DATA_TYPE *x)`
//   kept out of line, x arriving in eax; called (and tail-jumped) by mdct_butterfly_16 (0x00C9BFEB / 0x00C9BFF2).
// Shape (retry1, chunk 585/586 lever): parked as an "LTCG register argument" gap; written as the TU-local static inside
//   `extern "C" { static __declspec(noinline) }` with its real same-TU caller mdct_butterfly_16, VC7.1 picks x in eax itself.
typedef float DATA_TYPE;
typedef float REG_TYPE;
#define cPI2_8 .7071067811865476f
#define MULT_NORM(x) (x)
extern "C" {
static __declspec(noinline) void mdct_butterfly_8(DATA_TYPE* x)
{
    REG_TYPE r0 = x[6] + x[2];
    REG_TYPE r1 = x[6] - x[2];
    REG_TYPE r2 = x[4] + x[0];
    REG_TYPE r3 = x[4] - x[0];

    x[6] = r0 + r2;
    x[4] = r0 - r2;

    r0 = x[5] - x[1];
    r2 = x[7] - x[3];
    x[0] = r1 + r0;
    x[2] = r1 - r0;

    r0 = x[5] + x[1];
    r1 = x[7] + x[3];
    x[3] = r2 + r3;
    x[1] = r2 - r3;
    x[7] = r1 + r0;
    x[5] = r1 - r0;
}

// Same-TU caller: mdct.c mdct_butterfly_16 (retail 0x00C9BF30, calls 0x00C9BFEB / tail jump 0x00C9BFF2), present so the
// static gets retail's private convention. Not graded here.
static __declspec(noinline) void mdct_butterfly_16(DATA_TYPE* x)
{
    REG_TYPE r0 = x[1] - x[9];
    REG_TYPE r1 = x[0] - x[8];

    x[8] += x[0];
    x[9] += x[1];
    x[0] = MULT_NORM((r0 + r1) * cPI2_8);
    x[1] = MULT_NORM((r0 - r1) * cPI2_8);

    r0 = x[3] - x[11];
    r1 = x[10] - x[2];
    x[10] += x[2];
    x[11] += x[3];
    x[2] = r0;
    x[3] = r1;

    r0 = x[12] - x[4];
    r1 = x[13] - x[5];
    x[12] += x[4];
    x[13] += x[5];
    x[4] = MULT_NORM((r0 - r1) * cPI2_8);
    x[5] = MULT_NORM((r0 + r1) * cPI2_8);

    r0 = x[14] - x[6];
    r1 = x[15] - x[7];
    x[14] += x[6];
    x[15] += x[7];
    x[6] = r0;
    x[7] = r1;

    mdct_butterfly_8(x);
    mdct_butterfly_8(x + 8);
}

void __cdecl vorbis_mdct_butterfly_16_caller(DATA_TYPE* x)
{
    mdct_butterfly_16(x);
}
}
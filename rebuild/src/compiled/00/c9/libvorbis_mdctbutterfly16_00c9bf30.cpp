// libvorbis::mdct_butterfly_16(DATA_TYPE* x) -- mdct.c: the 16-point MDCT butterfly stage -- four (x[k], x[k+1]) / (x[k+8], x[k+9]) pair rotations (two by cPI2_8 = sqrt(1/2), two plain), adding the upper half into the lower, then both 8-point halves through mdct_butterfly_8
// Vendor-proof: libvorbis-1.0.1(lib/mdct.c) @mdct_butterfly_16@4 -- 199 bytes verified against Fable.exe 0x00C9BF30
// Leaf: _mdct_butterfly_16
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9BF30, 199 bytes: libvorbis I 20030909 lib/mdct.c `STIN void mdct_butterfly_16(DATA_TYPE *x)` kept
//   out of line with x arriving in eax (a private LTCG convention); it calls mdct_butterfly_8 0x00C9BEB0 at 0x00C9BFEB and tail-jumps to it
//   for x+16 at 0x00C9BFF2. Callers: mdct_butterfly_32 0x00C9C000 (both tail sites, landed chunk 866).
// Identity (two sources): (1) the body is mdct.c's mdct_butterfly_16 verbatim -- pair 0 and pair 2 multiply by cPI2_8 (pool 0x012BF320 =
//   0.70710678f), pairs 1 and 3 are plain copies, then the two butterfly_8 calls; (2) the landed mdct_butterfly_8 TU (0x00C9BEB0) already
//   carries this function as its same-TU caller at this address, and landed mdct_butterfly_32 binds both of its tail calls here. The
//   manifest label `C3DBoundingBox::InitialiseFromSphere` at this address is a propagated error (libpk has no C3DBoundingBox).
// Shape: the landed mdct_butterfly_8 TU text unchanged (chunk 585/586 lever: `extern "C" { static __declspec(noinline) }` statics beside a
//   same-TU caller, so VC7.1 /O2 picks x in eax itself); only the graded leaf differs.
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
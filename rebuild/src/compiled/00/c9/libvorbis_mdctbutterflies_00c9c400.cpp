// libvorbis::mdct_butterflies(mdct_lookup* init, DATA_TYPE* x, int points) -- run the MDCT butterfly stages (first, generic per stage, then the 32-point tail)
// Shape (retry1): parked as an "LTCG register arguments" gap; the TU holds mdct.c's statics (butterfly_8/16/32, first, generic as __stdcall, butterflies as __stdcall) inside `extern "C" { static __declspec(noinline) }` with an mdct_backward-shape caller, and VC7.1 /O2 picks every retail private convention itself (init in EAX; butterfly_first x in edi/ecx; generic ecx/edx + one push, callee-pop; butterfly_32 x in EAX).
// Leaf: _mdct_butterflies@12
// Retail: Fable.exe @ 0x00C9C400, 185 bytes. libvorbis 1.0.1 lib/mdct.c (STIN, out of line under LTCG). Grades: 1.
// The bodies are the shipped libvorbis 1.0.1 mdct.c source (butterfly_32 / generic as in mdct.c; only mdct_butterflies is graded here).
typedef float DATA_TYPE;
typedef float REG_TYPE;
#define cPI3_8 .38268343236508977175f
#define cPI2_8 .70710678118654752441f
#define cPI1_8 .92387953251128675613f
typedef struct mdct_lookup { int n; int log2n; DATA_TYPE* trig; int* bitrev; DATA_TYPE scale; } mdct_lookup;
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

static __declspec(noinline) void mdct_butterfly_32(DATA_TYPE* x)
{
    REG_TYPE r0 = x[30] - x[14];
    REG_TYPE r1 = x[31] - x[15];

    x[30] += x[14];
    x[31] += x[15];
    x[14] = MULT_NORM(r0 * cPI2_8 + r1 * cPI2_8);
    x[15] = MULT_NORM(r1 * cPI2_8 - r0 * cPI2_8);

    r0 = x[28] - x[12];
    r1 = x[29] - x[13];
    x[28] += x[12];
    x[29] += x[13];
    x[12] = MULT_NORM(r0 * cPI1_8 - r1 * cPI3_8);
    x[13] = MULT_NORM(r0 * cPI3_8 + r1 * cPI1_8);

    r0 = x[26] - x[10];
    r1 = x[27] - x[11];
    x[26] += x[10];
    x[27] += x[11];
    x[10] = MULT_NORM((r0 - r1) * cPI2_8);
    x[11] = MULT_NORM((r0 + r1) * cPI2_8);

    r0 = x[24] - x[8];
    r1 = x[25] - x[9];
    x[24] += x[8];
    x[25] += x[9];
    x[8] = MULT_NORM(r0 * cPI3_8 - r1 * cPI1_8);
    x[9] = MULT_NORM(r1 * cPI3_8 + r0 * cPI1_8);

    r0 = x[22] - x[6];
    r1 = x[7] - x[23];
    x[22] += x[6];
    x[23] += x[7];
    x[6] = r1;
    x[7] = r0;

    r0 = x[4] - x[20];
    r1 = x[5] - x[21];
    x[20] += x[4];
    x[21] += x[5];
    x[4] = MULT_NORM(r1 * cPI1_8 + r0 * cPI3_8);
    x[5] = MULT_NORM(r1 * cPI3_8 - r0 * cPI1_8);

    r0 = x[2] - x[18];
    r1 = x[3] - x[19];
    x[18] += x[2];
    x[19] += x[3];
    x[2] = MULT_NORM((r1 + r0) * cPI2_8);
    x[3] = MULT_NORM((r1 - r0) * cPI2_8);

    r0 = x[0] - x[16];
    r1 = x[1] - x[17];
    x[16] += x[0];
    x[17] += x[1];
    x[0] = MULT_NORM(r1 * cPI3_8 + r0 * cPI1_8);
    x[1] = MULT_NORM(r1 * cPI1_8 - r0 * cPI3_8);

    mdct_butterfly_16(x);
    mdct_butterfly_16(x + 16);
}

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

static __declspec(noinline) void __stdcall mdct_butterflies(mdct_lookup* init, DATA_TYPE* x, int points)
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

// Same-TU caller (the mdct_butterflies calls of mdct.c mdct_backward / mdct_forward: mdct_butterflies(init, x + n2, n2)), present so
// the statics get retail's private conventions. Not graded.
void __cdecl vorbis_mdct_butterflies_caller(mdct_lookup* init, DATA_TYPE* x)
{
    int n = init->n;
    int n2 = n >> 1;
    mdct_butterflies(init, x + n2, n2);
}
}
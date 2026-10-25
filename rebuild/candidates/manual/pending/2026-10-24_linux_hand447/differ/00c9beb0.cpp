// libvorbis::mdct_butterfly_8(DATA_TYPE* x) -- the 8-point MDCT butterfly stage
// Grade: DIFFER(125v125) -- LTCG register argument: retail takes x in EAX, ours in ecx; the 47 instructions are identical modulo that base register
// Retail: Fable.exe @ 0x00C9BEB0, 125 bytes. libvorbis 1.0.1 lib/mdct.c (STIN, kept out of line by LTCG). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: mdct_butterfly_8
typedef float DATA_TYPE;
typedef float REG_TYPE;
extern "C" void __fastcall mdct_butterfly_8(DATA_TYPE* x)
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

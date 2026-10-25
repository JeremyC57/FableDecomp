// libvorbis::max_curve(float* c, float* c2) -- element-wise maximum of two 56-point tone-masking curves into c
// Grade: DIFFER(180v183) -- LTCG register argument: c in ecx, c2 in EAX (same as min_curve 0x00C918C0); `__fastcall` gives c2 in edx
// Retail: Fable.exe @ 0x00C91980, 183 bytes. libvorbis 1.0.1 lib/psy.c (static). Twin shape of min_curve with `test ah,41h ; jne`. Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: max_curve
#define EHMER_MAX 56
extern "C" void __fastcall max_curve(float* c, float* c2)
{
    int i;
    for (i = 0; i < EHMER_MAX; i++) if (c2[i] > c[i]) c[i] = c2[i];
}

// libvorbis::min_curve(float* c, float* c2) -- element-wise minimum of two 56-point tone-masking curves into c
// Grade: DIFFER(180v183) -- LTCG register argument: retail takes c in ecx and c2 in EAX (`mov edi,eax ; sub edi,esi` builds a c2-c delta, `lea edx,[eax+0Ch]`); a C++ `__fastcall` puts c2 in edx and allocates differently
// Retail: Fable.exe @ 0x00C918C0, 183 bytes. libvorbis 1.0.1 lib/psy.c (static, called from setup_tone_curves). The loop over EHMER_MAX (56) is unrolled by 8 (`mov esi,7` + one peeled element). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: min_curve
#define EHMER_MAX 56
extern "C" void __fastcall min_curve(float* c, float* c2)
{
    int i;
    for (i = 0; i < EHMER_MAX; i++) if (c2[i] < c[i]) c[i] = c2[i];
}

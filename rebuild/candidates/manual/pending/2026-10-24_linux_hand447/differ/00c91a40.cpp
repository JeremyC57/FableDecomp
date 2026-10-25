// libvorbis::attenuate_curve(float* c, float att) -- add att (dB) to every point of a 56-point tone-masking curve
// Grade: DIFFER(97v97) -- LTCG register argument: retail takes c in EAX (`add eax,8`), ours in ecx (`lea eax,[ecx+8]`); every other byte matches
// Retail: Fable.exe @ 0x00C91A40, 97 bytes (`ret 4`, att on the stack). libvorbis 1.0.1 lib/psy.c (static). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: attenuate_curve
#define EHMER_MAX 56
extern "C" void __fastcall attenuate_curve(float* c, float att)
{
    int i;
    for (i = 0; i < EHMER_MAX; i++)
        c[i] += att;
}

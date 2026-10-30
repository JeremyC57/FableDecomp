// libvorbis::attenuate_curve(float* c, float att) -- add att (dB) to every point of a 56-point tone-masking curve
// Leaf: attenuate_curve
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C91A40, 97 bytes: libvorbis 1.0.1 lib/psy.c `static void attenuate_curve(float *c,
//   float att)`, the loop unrolled by 8 (`add eax,20h ; dec ecx`), c arriving in eax and att on the stack (`ret 4`).
// Shape (retry1, chunk 585/587/593 lever): a TU-local static keeps its C name and gets VC7.1's private register
//   convention only inside `extern "C" { static __declspec(noinline) }`; `__stdcall` gives retail's callee-pop `ret 4`.
//   VC7.1 picks c in eax itself once a same-TU caller exists (parked earlier as an "LTCG register argument" gap).
// Callers: setup_tone_curves (psy.c) at 0x00C91FCD and 0x00C91FFC.
#define EHMER_MAX 56
extern "C" {
static __declspec(noinline) void __stdcall attenuate_curve(float* c, float att)
{
    int i;
    for (i = 0; i < EHMER_MAX; i++)
        c[i] += att;
}

// Same-TU caller (the attenuate_curve calls of psy.c setup_tone_curves, 0x00C91FCD / 0x00C91FFC), present so the static
// gets retail's private convention. Not graded.
void __cdecl vorbis_setup_tone_curves_attenuate_caller(float* workc, float att)
{
    attenuate_curve(workc, att);
}
}
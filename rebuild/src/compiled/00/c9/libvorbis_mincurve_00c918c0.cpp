// libvorbis::min_curve(float* c, float* c2) -- element-wise minimum of two 56-point tone-masking curves into c
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libvorbis static retail's private register convention itself (chunk 585/586/593 lever).
// Retail: Fable.exe @ 0x00C918C0, 183 bytes. libvorbis 1.0.1 lib/psy.c (static, called from setup_tone_curves). The loop over EHMER_MAX (56) is unrolled by 8 (`mov esi,7` + one peeled element). Grades: 1.
// The body is the shipped libvorbis 1.0.1 source; c in ECX and c2 in EAX is VC7.1's own private convention for the static.
// Leaf: _min_curve
#define EHMER_MAX 56
extern "C" {
static __declspec(noinline) void min_curve(float* c, float* c2)
{
    int i;
    for (i = 0; i < EHMER_MAX; i++) if (c2[i] < c[i]) c[i] = c2[i];
}

// Same-TU caller (the min_curve calls of psy.c setup_tone_curves), present so the static gets retail's private
// convention. Not graded.
void __cdecl min_curve_caller(float* c, float* c2) { min_curve(c, c2); }
}
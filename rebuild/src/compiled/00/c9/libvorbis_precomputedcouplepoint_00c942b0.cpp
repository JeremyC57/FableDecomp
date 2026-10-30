// libvorbis::precomputed_couple_point(float premag, int floorA, int floorB, float* mag, float* ang) -- point-stereo coupling of one line from the two floor values
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libvorbis static retail's private register convention itself (chunk 585/586/593 lever).
// Retail: Fable.exe @ 0x00C942B0, 91 bytes. libvorbis 1.0.1 lib/psy.c (static). Data: hypot_lookup @ 0x013A3D48, FLOOR1_fromdB_INV_LOOKUP @ 0x013A3948, 1.0f @ 0x0122DED8. Grades: 1.
// The body is the shipped libvorbis 1.0.1 source; the register convention is VC7.1's own private one for the same-TU static.
// Leaf: _precomputed_couple_point
extern const float hypot_lookup[32];                 // 0x013A3D48
extern const float FLOOR1_fromdB_INV_LOOKUP[256];    // 0x013A3948
extern "C" int __cdecl abs(int x);
#pragma intrinsic(abs)
extern "C" {
static __declspec(noinline) void __stdcall precomputed_couple_point(float premag, int floorA, int floorB, float* mag, float* ang)
{
    int test = (floorA > floorB) - 1;
    int offset = 31 - abs(floorA - floorB);
    float floormag = hypot_lookup[((offset < 0) - 1) & offset] + 1.f;

    floormag *= FLOOR1_fromdB_INV_LOOKUP[(floorB & test) | (floorA & (~test))];

    *mag = premag * floormag;
    *ang = 0.f;
}

// Same-TU caller (the precomputed_couple_point calls of psy.c _vp_couple), present so the static gets retail's private
// convention. Not graded.
void __cdecl precomputed_couple_point_caller(float premag, int floorA, int floorB, float* mag, float* ang) { precomputed_couple_point(premag, floorA, floorB, mag, ang); }
}
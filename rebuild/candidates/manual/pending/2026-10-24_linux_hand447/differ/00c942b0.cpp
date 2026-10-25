// libvorbis::precomputed_couple_point(float premag, int floorA, int floorB, float* mag, float* ang) -- point-stereo coupling of one line from the two floor values
// Grade: DIFFER(99v91) -- LTCG register argument: retail takes floorA in EDI and floorB in ESI (premag, mag, ang on the stack, `ret 0Ch`), ours floorA/floorB in ecx/edx
// Retail: Fable.exe @ 0x00C942B0, 91 bytes. libvorbis 1.0.1 lib/psy.c (static). Data: hypot_lookup @ 0x013A3D48, FLOOR1_fromdB_INV_LOOKUP @ 0x013A3948, 1.0f @ 0x0122DED8. Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: precomputed_couple_point
extern const float hypot_lookup[32];                 // 0x013A3D48
extern const float FLOOR1_fromdB_INV_LOOKUP[256];    // 0x013A3948
extern "C" int __cdecl abs(int x);
#pragma intrinsic(abs)
extern "C" void __fastcall precomputed_couple_point(float premag, int floorA, int floorB, float* mag, float* ang)
{
    int test = (floorA > floorB) - 1;
    int offset = 31 - abs(floorA - floorB);
    float floormag = hypot_lookup[((offset < 0) - 1) & offset] + 1.f;

    floormag *= FLOOR1_fromdB_INV_LOOKUP[(floorB & test) | (floorA & (~test))];

    *mag = premag * floormag;
    *ang = 0.f;
}

// libvorbis::post_Y(int* A, int* B, int pos) -- libvorbis floor1.c: merge two fitted post values (the other one if either is unset/negative, else their mean)
// Grade: DIFFER(34v23) -- lane gap: retail takes A in eax, B in edx and pos in ecx (LTCG custom register convention, no stack arguments)
// Leaf: @post_Y@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA26A0, 23 bytes (+9 int3). libvorbis lib/floor1.c static helper (floor1_fit).
// Label: manifest "InterpolateInt_Conditional" is a descriptive placeholder; the real name is the floor1.c static `post_Y`.
extern "C" int __fastcall post_Y(int* A, int* B, int pos)
{
    if (A[pos] < 0)
        return B[pos];
    if (B[pos] < 0)
        return A[pos];

    return (A[pos] + B[pos]) >> 1;
}

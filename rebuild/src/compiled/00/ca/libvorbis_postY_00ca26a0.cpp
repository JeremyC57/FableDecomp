// libvorbis::post_Y(int* A, int* B, int pos) -- libvorbis floor1.c: merge two fitted post values (the other one if either is unset/negative, else their mean)
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libvorbis static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: _post_Y
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA26A0, 23 bytes (+9 int3). libvorbis lib/floor1.c static helper (floor1_fit).
// Label: manifest "InterpolateInt_Conditional" is a descriptive placeholder; the real name is the floor1.c static `post_Y`.
extern "C" {
static __declspec(noinline) int post_Y(int* A, int* B, int pos)
{
    if (A[pos] < 0)
        return B[pos];
    if (B[pos] < 0)
        return A[pos];

    return (A[pos] + B[pos]) >> 1;
}

// Same-TU caller (the post_Y calls of floor1.c floor1_fit), present so the static gets retail's private
// convention. Not graded.
int __cdecl post_Y_caller(int* A, int* B, int pos) { return post_Y(A, B, pos); }
}
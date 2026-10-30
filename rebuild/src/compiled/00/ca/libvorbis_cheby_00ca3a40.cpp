// libvorbis::cheby(float* g, int ord) -- libvorbis lsp.c: convert a symmetric polynomial to Chebyshev form in place (g[0] halved, then the j-2/j butterfly per order)
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libvorbis static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: _cheby
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3A40, 196 bytes (+12 int3). libvorbis lib/lsp.c static helper (vorbis_lpc_to_lsp);
//   constant 0.5f @ 0x0122F59C.
// Label: manifest "FloatArray_TransformWithUnroll" is a descriptive placeholder; the real name is the lsp.c static `cheby`.
extern "C" {
static __declspec(noinline) void cheby(float* g, int ord)
{
    int i, j;

    g[0] *= .5f;
    for (i = 2; i <= ord; i++) {
        for (j = ord; j >= i; j--) {
            g[j - 2] -= g[j];
            g[j] += g[j];
        }
    }
}

// Same-TU caller (the cheby calls of lsp.c vorbis_lpc_to_lsp), present so the static gets retail's private
// convention. Not graded.
void __cdecl cheby_caller(float* g, int ord) { cheby(g, ord); }
}
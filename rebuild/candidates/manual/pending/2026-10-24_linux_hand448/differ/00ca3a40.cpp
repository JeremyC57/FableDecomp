// libvorbis::cheby(float* g, int ord) -- libvorbis lsp.c: convert a symmetric polynomial to Chebyshev form in place (g[0] halved, then the j-2/j butterfly per order)
// Grade: DIFFER(200v196) -- lane gap: retail takes g in esi and ord in edi (LTCG custom register convention, unsaved); VC7.1's 4x unroll of the inner loop matches the shape
// Leaf: @cheby@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3A40, 196 bytes (+12 int3). libvorbis lib/lsp.c static helper (vorbis_lpc_to_lsp);
//   constant 0.5f @ 0x0122F59C.
// Label: manifest "FloatArray_TransformWithUnroll" is a descriptive placeholder; the real name is the lsp.c static `cheby`.
extern "C" void __fastcall cheby(float* g, int ord)
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

// libvorbis::drft_forward(drft_lookup* l, float* data) -- forward real FFT through the cached twiddles/factors (no-op for n == 1)
// Grade: DIFFER(29v30) -- LTCG callee convention: the row itself is `__fastcall` (l in ecx, data in edx) but drftf1 0x00C98E80 takes c in EDI, ch in EAX, ifac in ECX and n/wa pushed with a callee pop; a C++ declaration cannot express that
// Retail: Fable.exe @ 0x00C9BC10, 30 bytes. libvorbis 1.0.1 lib/smallft.c drft_forward (drft_backward 0x00C9BC30 is the same shape around drftb1 0x00C9B9F0). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: drft_forward
typedef struct drft_lookup { int n; float* trigcache; int* splitcache; } drft_lookup;
extern "C" void __fastcall drftf1(int n, float* c, float* ch, float* wa, int* ifac);   // 0x00C98E80
extern "C" void __fastcall drft_forward(drft_lookup* l, float* data)
{
    if (l->n == 1) return;
    drftf1(l->n, data, l->trigcache, l->trigcache + l->n, l->splitcache);
}

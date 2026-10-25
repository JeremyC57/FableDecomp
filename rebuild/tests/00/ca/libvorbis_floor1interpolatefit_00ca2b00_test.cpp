// Behaviour test for 0x00CA2B00 floor1_interpolate_fit (libvorbis floor1.c): blend two fitted post lists by del/65536,
// keeping the 0x8000 "unused" flag only when both inputs carry it; NULL when either input is missing.
#include <cstdio>
#define VIF_POSIT 63
struct vorbis_block { int allocs; long last_bytes; int pool[128]; };
struct vorbis_look_floor1 {
    int sorted_index[VIF_POSIT + 2];
    int forward_index[VIF_POSIT + 2];
    int reverse_index[VIF_POSIT + 2];
    int hineighbor[VIF_POSIT];
    int loneighbor[VIF_POSIT];
    int posts;
    int n;
    int quant_q;
    void* vi;
    long phrasebits;
    long postbits;
    long frames;
};
void* _vorbis_block_alloc(vorbis_block* vb, long bytes) { vb->allocs++; vb->last_bytes = bytes; return vb->pool; }
int* floor1_interpolate_fit(vorbis_block* vb, vorbis_look_floor1* look, int* A, int* B, int del)
{
    long i;
    long posts = look->posts;
    int* output = 0;

    if (A && B) {
        output = (int*)_vorbis_block_alloc(vb, sizeof(*output) * posts);

        for (i = 0; i < posts; i++) {
            output[i] = ((65536 - del) * (A[i] & 0x7fff) + del * (B[i] & 0x7fff) + 32768) >> 16;
            if (A[i] & 0x8000 && B[i] & 0x8000) output[i] |= 0x8000;
        }
    }

    return (output);
}
int main()
{
    static vorbis_block vb;
    static vorbis_look_floor1 look;
    look.posts = 4;
    int A[4] = { 100, 0x8000 | 50, 0x8000 | 200, 1000 };
    int B[4] = { 300, 0x8000 | 150, 400, 0x8000 | 0 };
    int ok = 1;
    ok &= floor1_interpolate_fit(&vb, &look, 0, B, 0) == 0 && vb.allocs == 0;
    ok &= floor1_interpolate_fit(&vb, &look, A, 0, 0) == 0 && vb.allocs == 0;
    int* out = floor1_interpolate_fit(&vb, &look, A, B, 32768);   // halfway
    ok &= out == vb.pool && vb.allocs == 1 && vb.last_bytes == 16;
    ok &= out[0] == 200;
    ok &= out[1] == (0x8000 | 100);
    ok &= out[2] == 300;            // flag only on A: dropped
    ok &= out[3] == 500;            // flag only on B: dropped
    out = floor1_interpolate_fit(&vb, &look, A, B, 0);           // all A, flags only where both
    ok &= out[0] == 100 && out[1] == (0x8000 | 50) && out[2] == 200 && out[3] == 1000;
    out = floor1_interpolate_fit(&vb, &look, A, B, 65536);       // all B
    ok &= out[0] == 300 && out[1] == (0x8000 | 150) && out[2] == 400 && out[3] == 0;
    if (!ok) { std::printf("00ca2b00_TEST FAIL\n"); return 1; }
    std::printf("00ca2b00_TEST PASS\n");
    return 0;
}
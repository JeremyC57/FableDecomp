// behaviour test: libvorbis 1.0.1 res1_forward -- nonzero channels are added into out, packed to the front of in,
// encoded once, and the (coded) residue subtracted back out; all-zero input returns 0 without encoding.
#include <cstdio>
struct vorbis_block { long pcmend; };
static int g_calls = 0, g_used = 0;
static int _01forward(vorbis_block*, void*, float** in, int ch, long**)
{
    ++g_calls; g_used = ch;
    for (int i = 0; i < ch; i++) in[i][0] *= 0.5f;     // "coding" leaves half the residue
    return 7;
}
static int res1_forward(vorbis_block* vb, void* vl, float** in, float** out, int* nonzero, int ch, long** partword)
{
    int i, j, used = 0, n = vb->pcmend / 2;
    for (i = 0; i < ch; i++)
        if (nonzero[i]) {
            if (out)
                for (j = 0; j < n; j++)
                    out[i][j] += in[i][j];
            in[used++] = in[i];
        }
    if (used) {
        int ret = _01forward(vb, vl, in, used, partword);
        if (out) {
            used = 0;
            for (i = 0; i < ch; i++)
                if (nonzero[i]) {
                    for (j = 0; j < n; j++)
                        out[i][j] -= in[used][j];
                    used++;
                }
        }
        return ret;
    }
    return 0;
}
int main()
{
    vorbis_block vb; vb.pcmend = 4;                      // n = 2
    float a[2] = { 2.0f, 4.0f }, b[2] = { 6.0f, 8.0f }, c[2] = { 10.0f, 12.0f };
    float oa[2] = { 0, 0 }, ob[2] = { 0, 0 }, oc[2] = { 0, 0 };
    float* in[3] = { a, b, c };
    float* out[3] = { oa, ob, oc };
    int nonzero[3] = { 0, 1, 1 };
    int r = res1_forward(&vb, 0, in, out, nonzero, 3, 0);
    if (r != 7 || g_calls != 1 || g_used != 2 || in[0] != b || in[1] != c
        || ob[0] != 3.0f || ob[1] != 0.0f || oc[0] != 5.0f || oc[1] != 0.0f || oa[0] != 0.0f) {
        std::printf("00ca1060_TEST FAIL\n"); return 1;
    }
    int zero[3] = { 0, 0, 0 };
    if (res1_forward(&vb, 0, in, out, zero, 3, 0) != 0 || g_calls != 1) { std::printf("00ca1060_TEST FAIL\n"); return 1; }
    std::printf("00ca1060_TEST PASS\n");
    return 0;
}
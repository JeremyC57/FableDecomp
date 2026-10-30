// Behaviour test for libvorbis floor0_inverse2 (0x00CA39C0): zero the envelope without a memo, else render it.
#include <cstdio>
#include <cstring>
typedef struct { long W; } vorbis_block;
typedef struct { int ampdB; } vorbis_info_floor0;
typedef struct { int ln; int m; int** linearmap; int n[2]; vorbis_info_floor0* vi; } vorbis_look_floor0;
static int g_calls, g_n, g_m; static float g_amp, g_off;
static void vorbis_lsp_to_curve(float* curve, int* map, int n, int ln, float* lsp, int m, float amp, float ampoffset)
{ g_calls++; g_n = n; g_m = m; g_amp = amp; g_off = ampoffset; (void)curve; (void)map; (void)ln; (void)lsp; }
static int floor0_inverse2(vorbis_block* vb, void* i, void* memo, float* out)
{
    vorbis_look_floor0* look = (vorbis_look_floor0*)i;
    vorbis_info_floor0* info = look->vi;
    if (memo) {
        float* lsp = (float*)memo;
        float amp = lsp[look->m];
        vorbis_lsp_to_curve(out, look->linearmap[vb->W], look->n[vb->W], look->ln, lsp, look->m, amp, (float)info->ampdB);
        return (1);
    }
    memset(out, 0, sizeof(*out) * look->n[vb->W]);
    return (0);
}
int main()
{
    int map0[4] = { 0 }, map1[4] = { 0 }; int* maps[2] = { map0, map1 };
    vorbis_info_floor0 info = { 140 };
    vorbis_look_floor0 look = { 8, 2, maps, { 3, 4 }, &info };
    vorbis_block vb = { 1 };
    float out[5] = { 9, 9, 9, 9, 9 }; float lsp[3] = { 0.1f, 0.2f, 55.0f };
    if (floor0_inverse2(&vb, &look, 0, out) != 0 || out[0] != 0 || out[3] != 0 || out[4] != 9) { std::printf("00ca39c0_TEST FAIL\n"); return 1; }
    if (floor0_inverse2(&vb, &look, lsp, out) != 1 || g_calls != 1 || g_n != 4 || g_m != 2 || g_amp != 55.0f || g_off != 140.0f) { std::printf("00ca39c0_TEST FAIL\n"); return 1; }
    std::printf("00ca39c0_TEST PASS\n");
    return 0;
}
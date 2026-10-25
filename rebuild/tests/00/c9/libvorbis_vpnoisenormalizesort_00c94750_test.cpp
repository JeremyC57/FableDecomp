// Behaviour test for 0x00C94750 libvorbis _vp_noise_normalize_sort (copy of the model + logic; the alloca work array is a fixed buffer here).
#include <cstdio>
#include <cstdlib>
#include <cmath>
struct vorbis_info_psy_part { int normal_start; int normal_partition; };
struct vorbis_look_psy { int n; vorbis_info_psy_part* vi; };
extern "C" int apsort_t(const void* a, const void* b)
{
    float f1 = (float)std::fabs((double)**(float**)a);
    float f2 = (float)std::fabs((double)**(float**)b);
    return (f1 < f2) - (f1 > f2);
}
static void normalize_sort(vorbis_look_psy* p, float* magnitudes, int* sortedindex)
{
    int i, j, n = p->n;
    vorbis_info_psy_part* vi = p->vi;
    int partition = vi->normal_partition;
    float* work[64];
    int start = vi->normal_start;
    for (j = start; j < n; j += partition) {
        if (j + partition > n) partition = n - j;
        for (i = 0; i < partition; i++) work[i] = magnitudes + i + j;
        std::qsort(work, partition, sizeof(*work), apsort_t);
        for (i = 0; i < partition; i++) sortedindex[i + j - start] = (int)(work[i] - magnitudes);
    }
}
int main()
{
    float m[10] = { 9, 9, 0.5f, -4, 2, 1, -7, 3, -0.25f, 6 };
    int idx[10];
    vorbis_info_psy_part vi = { 2, 4 };
    vorbis_look_psy p = { 10, &vi };
    normalize_sort(&p, m, idx);
    int ok = idx[0] == 3 && idx[1] == 4 && idx[2] == 5 && idx[3] == 2 &&       /* |-4| |2| |1| |0.5| */
             idx[4] == 6 && idx[5] == 9 && idx[6] == 7 && idx[7] == 8;          /* |-7| |6| |3| |-0.25| */
    p.n = 9;                                                                     /* last partition truncated to 3 */
    normalize_sort(&p, m, idx);
    if (!(idx[4] == 6 && idx[5] == 7 && idx[6] == 8)) ok = 0;
    if (!ok) { std::printf("00c94750_TEST FAIL\n"); return 1; }
    std::printf("00c94750_TEST PASS\n");
    return 0;
}
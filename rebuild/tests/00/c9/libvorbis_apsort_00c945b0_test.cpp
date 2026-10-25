// Behaviour test for 0x00C945B0 libvorbis apsort (copy of the logic, driven through qsort).
#include <cstdio>
#include <cstdlib>
#include <cmath>
extern "C" int apsort_t(const void* a, const void* b)
{
    float f1 = (float)std::fabs((double)**(float**)a);
    float f2 = (float)std::fabs((double)**(float**)b);
    return (f1 < f2) - (f1 > f2);
}
int main()
{
    float v[5] = { 1.f, -3.f, 2.f, -0.5f, 0.f };
    float* w[5] = { v, v + 1, v + 2, v + 3, v + 4 };
    std::qsort(w, 5, sizeof(float*), apsort_t);
    int ok = *w[0] == -3.f && *w[1] == 2.f && *w[2] == 1.f && *w[3] == -0.5f && *w[4] == 0.f;
    if (apsort_t(&w[0], &w[0]) != 0) ok = 0;
    if (apsort_t(&w[0], &w[1]) != -1 || apsort_t(&w[1], &w[0]) != 1) ok = 0;
    if (!ok) { std::printf("00c945b0_TEST FAIL\n"); return 1; }
    std::printf("00c945b0_TEST PASS\n");
    return 0;
}
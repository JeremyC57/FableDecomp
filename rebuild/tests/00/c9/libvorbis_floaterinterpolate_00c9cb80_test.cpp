// Behaviour test for libvorbis bitrate.c floater_interpolate (0x00C9CB80): pick the bin whose bitrate is nearest the target.
#include <cstdio>
#include <cmath>
struct bm_t { long* avg_binacc; unsigned long avg_sampleacc; double avgfloat; int queue_bins; };
static long floater_interpolate(bm_t* bm, long rate, double desired_rate)
{
    int bin = (int)std::floor(bm->avgfloat + .5);
    double lobitrate = (double)(bm->avg_binacc[bin] * 8) / bm->avg_sampleacc * rate, hibitrate;
    while (lobitrate > desired_rate && bin > 0) { bin--; lobitrate = (double)(bm->avg_binacc[bin] * 8) / bm->avg_sampleacc * rate; }
    if (bin + 1 < bm->queue_bins) {
        hibitrate = (double)(bm->avg_binacc[bin + 1] * 8) / bm->avg_sampleacc * rate;
        if (std::fabs(hibitrate - desired_rate) < std::fabs(lobitrate - desired_rate)) bin++;
    }
    return bin;
}
int main()
{
    long acc[4] = { 100, 200, 300, 400 };
    bm_t bm = { acc, 8, 3.0, 4 };          // bitrate = acc * rate
    if (floater_interpolate(&bm, 1, 250.0) != 1 || floater_interpolate(&bm, 1, 290.0) != 2 || floater_interpolate(&bm, 1, 1000.0) != 3)
    { std::printf("00c9cb80_TEST FAIL\n"); return 1; }
    std::printf("00c9cb80_TEST PASS\n"); return 0;
}
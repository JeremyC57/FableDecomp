#include <cstdio>
// ov_raw_total: per-link byte span offsets[i+1]-offsets[i]; i<0 sums every link; OV_EINVAL guards.
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct VF { int seekable; int links; i64* offsets; int ready_state; };
static i64 ov_raw_total(VF* vf, int i)
{
    if (vf->ready_state < 2) return -131;
    if (!vf->seekable || i >= vf->links) return -131;
    if (i < 0) {
        i64 acc = 0;
        for (int j = 0; j < vf->links; j++) acc += ov_raw_total(vf, j);
        return acc;
    }
    return vf->offsets[i + 1] - vf->offsets[i];
}
int main()
{
    i64 big = (i64)5000000 * 1000;
    i64 off[4] = { 0, 1000, big, big + 123 };
    VF vf; vf.seekable = 1; vf.links = 3; vf.offsets = off; vf.ready_state = 2;
    if (ov_raw_total(&vf, 1) != big - 1000 || ov_raw_total(&vf, -1) != big + 123) { std::printf("00c8aa00_TEST FAIL\n"); return 1; }
    if (ov_raw_total(&vf, 3) != -131) { std::printf("00c8aa00_TEST FAIL\n"); return 1; }
    vf.ready_state = 1;
    if (ov_raw_total(&vf, 0) != -131) { std::printf("00c8aa00_TEST FAIL\n"); return 1; }
    vf.ready_state = 4; vf.seekable = 0;
    if (ov_raw_total(&vf, 0) != -131) { std::printf("00c8aa00_TEST FAIL\n"); return 1; }
    std::printf("00c8aa00_TEST PASS\n");
    return 0;
}
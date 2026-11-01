// Behaviour test for 00ca2480: libvorbis floor1.c inspect_error -- a candidate line segment is rejected (1) when any
// masked sample is too far above/below the line, or the total error is too large after the two-fit attenuation
#include <cstdio>
#include <cmath>
struct vorbis_info_floor1 { int maxover, maxunder, maxerr; float twofitatten; };
static int vorbis_dBquant(const float* x)
{
    int i = (int)((*x) * 7.3142857f + 1023.5f);
    if (i > 1023) return 1023;
    if (i < 0) return 0;
    return i;
}
static int inspect_error(int x0, int x1, int y0, int y1, const float* mask, const float* mdct, vorbis_info_floor1* info)
{
    int dy = y1 - y0, adx = x1 - x0, ady = dy < 0 ? -dy : dy, base = dy / adx, sy = dy < 0 ? base - 1 : base + 1;
    int x = x0, y = y0, err = 0, val = vorbis_dBquant(mask + x), mse = 0, n = 0;
    ady -= (base < 0 ? -base : base) * adx;
    mse = (y - val); mse *= mse; n++;
    if (mdct[x] + info->twofitatten >= mask[x]) {
        if (y + info->maxover < val) return 1;
        if (y - info->maxunder > val) return 1;
    }
    while (++x < x1) {
        err = err + ady;
        if (err >= adx) { err -= adx; y += sy; } else { y += base; }
        val = vorbis_dBquant(mask + x);
        mse += ((y - val) * (y - val)); n++;
        if (mdct[x] + info->twofitatten >= mask[x]) {
            if (val) {
                if (y + info->maxover < val) return 1;
                if (y - info->maxunder > val) return 1;
            }
        }
    }
    if (info->maxover * info->maxover / n > info->maxerr) return 0;
    if (info->maxunder * info->maxunder / n > info->maxerr) return 0;
    if (mse / n > info->maxerr) return 1;
    return 0;
}
int main()
{
    float mask[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }, mdct[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
    int flat = vorbis_dBquant(mask);                    // 1023.5 -> 1023
    vorbis_info_floor1 info = { 8, 8, 2, 0.f };
    bool ok = inspect_error(0, 5, flat, flat, mask, mdct, &info) == 0;   // the line sits on the mask: no error
    ok = ok && inspect_error(0, 5, flat - 20, flat - 20, mask, mdct, &info) == 1;   // 20 under the mask: past maxover
    vorbis_info_floor1 tight = { 5, 5, 10, 0.f };                         // 5^2/5 = 5 <= maxerr: the mean square error decides
    ok = ok && inspect_error(0, 5, flat - 4, flat - 4, mask, mdct, &tight) == 1;   // mse 16 > 10
    tight.maxerr = 20;
    ok = ok && inspect_error(0, 5, flat - 4, flat - 4, mask, mdct, &tight) == 0;   // mse 16 <= 20
    std::printf(ok ? "00ca2480_TEST PASS\n" : "00ca2480_TEST FAIL\n");
    return ok ? 0 : 1;
}
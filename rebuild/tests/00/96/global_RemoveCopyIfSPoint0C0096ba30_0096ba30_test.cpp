#include <cstdio>
#include <math.h>

struct SPoint0C { float x, y, z; };
struct SOutsideHeightBand {
    float m_height;
    float m_tolerance;
    bool operator()(const SPoint0C& p) const { return !(fabs(p.z - m_height) < m_tolerance); }
};

static SPoint0C* RemoveCopyIf(const SPoint0C* first, const SPoint0C* last, SPoint0C* result, SOutsideHeightBand pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    return result;
}

int main()
{
    SPoint0C pts[5] = { { 1, 2, 10.0f }, { 3, 4, 12.5f }, { 5, 6, 9.6f }, { 7, 8, 11.0f }, { 9, 10, 10.5f } };
    SPoint0C out[5];
    SOutsideHeightBand band; band.m_height = 10.0f; band.m_tolerance = 1.0f;
    SPoint0C* r = RemoveCopyIf(pts, pts + 5, out, band);  // keeps z = 10, 9.6, 10.5 (11.0 is not strictly inside)
    bool ok = r == out + 3;
    if (ok && (out[0].x != 1 || out[1].x != 5 || out[1].y != 6 || out[1].z != 9.6f || out[2].x != 9)) ok = false;
    if (RemoveCopyIf(pts, pts, out, band) != out) ok = false;
    if (!ok) { std::printf("0096ba30_TEST FAIL\n"); return 1; }
    std::printf("0096ba30_TEST PASS\n");
    return 0;
}
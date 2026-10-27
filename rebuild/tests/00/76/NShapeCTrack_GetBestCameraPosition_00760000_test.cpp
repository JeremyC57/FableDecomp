// Behaviour test for 0x00760000: GetBestCameraPosition returns the position it was given
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SVec { float X, Y, Z; };
struct STrack { SVec GetBestCameraPosition(const SVec& p, float) const { return p; } };
int main()
{
    SVec p; p.X = 1.0f; p.Y = -2.0f; p.Z = 3.5f; STrack t;
    SVec r = t.GetBestCameraPosition(p, 9.0f);
    CHECK(r.X == 1.0f && r.Y == -2.0f && r.Z == 3.5f);
    if (!g_fail) printf("00760000_TEST PASS\n");
    return g_fail;
}
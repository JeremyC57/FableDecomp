// Behaviour test for 0x00789A80: GetShotAccuracyFraction = Accuracy * 0.01
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SRanged { float Accuracy; float GetShotAccuracyFraction() const { return Accuracy * 0.01f; } };
int main()
{
    SRanged r; r.Accuracy = 50.0f;
    CHECK(r.GetShotAccuracyFraction() > 0.4999f && r.GetShotAccuracyFraction() < 0.5001f);
    r.Accuracy = 0.0f; CHECK(r.GetShotAccuracyFraction() == 0.0f);
    if (!g_fail) printf("00789a80_TEST PASS\n");
    return g_fail;
}
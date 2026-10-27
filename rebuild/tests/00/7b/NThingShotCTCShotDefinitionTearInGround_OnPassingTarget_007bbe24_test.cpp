// Behaviour test for 0x007BBE24: OnPassingTarget schedules death two seconds later
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SShot { float DieIn; void SetDieInSeconds(float s) { DieIn = s; } void OnPassingTarget() { SetDieInSeconds(2.0f); } };
int main()
{
    SShot s; s.DieIn = -1.0f; s.OnPassingTarget(); CHECK(s.DieIn == 2.0f);
    if (!g_fail) printf("007bbe24_TEST PASS\n");
    return g_fail;
}
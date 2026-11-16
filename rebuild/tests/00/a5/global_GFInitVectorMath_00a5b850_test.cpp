// Behaviour test for 0x00a5b850 NVectorMath::GFInitVectorMath: links against the candidate's object (the source's cpuid
// block stays in the source; this test only declares the function) and checks the SSE flag it sets.
#include <stdio.h>
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; printf("check failed line %d\n", __LINE__); } } while (0)
namespace NVectorMath { void GFInitVectorMath(); }
bool GEnableSIMD = false;                        // 0x013D2880 in retail; defined here for the linked candidate object
int main()
{
    NVectorMath::GFInitVectorMath();
    CHECK(GEnableSIMD == true);                  // every CPU this test runs on reports SSE (cpuid 1, edx bit 25)
    NVectorMath::GFInitVectorMath();                // the flag is only ever set, never cleared
    CHECK(GEnableSIMD == true);
    if (g_ok) printf("00a5b850_TEST PASS\n");
    return g_ok ? 0 : 1;
}

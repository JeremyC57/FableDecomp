#include <cstdio>
// Behaviour of CTCTargetingSpirit::vfunc_22 (0x004DEC09): returns a default-constructed result object.
static int g_ctors = 0;
struct CTargetingResult_Retail { long m_value; CTargetingResult_Retail() : m_value(1234) { ++g_ctors; } };
struct CTCTargetingSpirit_Calls { CTargetingResult_Retail GetClassTypeName(); };
CTargetingResult_Retail CTCTargetingSpirit_Calls::GetClassTypeName() { return CTargetingResult_Retail(); }
int main()
{
    CTCTargetingSpirit_Calls t; CTargetingResult_Retail r = t.GetClassTypeName();
    if (r.m_value != 1234 || g_ctors < 1) { std::printf("004dec09_TEST FAIL\n"); return 1; }
    std::printf("004dec09_TEST PASS\n"); return 0;
}
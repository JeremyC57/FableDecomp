// Behaviour test for 0x0122A5E0, the dynamic atexit destructor for CCharString::NullString (on fable_string.h, as the row):
// the inline ~CCharString on the static empty string calls CCharString::UnassignString (stubbed here, counting calls) and
// drops the live-string count by one. The test runs the destructor on NullString explicitly, as the atexit thunk does.
#include <cstdio>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_NULLSTRING
#include "fable_string.h"

long g_CCharStringInstanceCount = 0;                     // 0x013BD800
static int g_unassignCalls = 0;

inline CCharString::CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
inline CCharString::~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
void CCharString::UnassignString() { ++g_unassignCalls; PStringData = 0; }

CCharString CCharString::NullString;                     // 0x013BD804

int main()
{
    // The dynamic initializer ran before main: one live string, null data.
    if (g_CCharStringInstanceCount != 1 || g_unassignCalls != 0) { std::printf("AUTO_TINY_0122a5e0_TEST FAIL\n"); return 1; }
    CCharString::NullString.~CCharString();              // what the atexit thunk runs
    if (g_CCharStringInstanceCount != 0 || g_unassignCalls != 1) { std::printf("AUTO_TINY_0122a5e0_TEST FAIL\n"); return 1; }
    std::printf("AUTO_TINY_0122a5e0_TEST PASS\n");
    return 0;
}

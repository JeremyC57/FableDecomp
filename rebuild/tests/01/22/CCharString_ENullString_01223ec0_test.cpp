#include <cstdio>
#include <cstring>
#include <cmath>
static int g_count = 0; static int g_unassigned = 0;
class CCharString {
public:
    void* PStringData;
    CCharString() : PStringData(0) { ++g_count; }
    ~CCharString() { UnassignString(); --g_count; }
    void UnassignString() { ++g_unassigned; PStringData = 0; }
};

int main()
{
    {
        CCharString nullString;                           // what $E_NullString runs
        if (g_count != 1 || nullString.PStringData != 0) do { std::printf("01223ec0_TEST FAIL\n"); return 1; } while (0);
    }
    if (g_count != 0 || g_unassigned != 1) do { std::printf("01223ec0_TEST FAIL\n"); return 1; } while (0);        // the atexit half
    std::printf("01223ec0_TEST PASS\n");
    return 0;
}
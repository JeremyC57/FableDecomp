// Behaviour test for 0x00CB0BC0 strmbase::AmGetLastErrorToHResult: E_FAIL for no error, a negative value unchanged,
// else HRESULT_FROM_WIN32 (0x8007xxxx).
#include <cstdio>

static unsigned int g_err = 0;
static unsigned int GetLastError_() { return g_err; }

static int AmGetLastErrorToHResult()
{
    unsigned int dwLastError = GetLastError_();
    if (dwLastError != 0)
        return (int)dwLastError <= 0 ? (int)dwLastError
                                     : (int)((dwLastError & 0x0000FFFF) | (7 << 16) | 0x80000000);
    else
        return (int)0x80004005;
}

int main()
{
    g_err = 0;
    if (AmGetLastErrorToHResult() != (int)0x80004005) { std::printf("00cb0bc0_TEST FAIL\n"); return 1; }
    g_err = 5;
    if (AmGetLastErrorToHResult() != (int)0x80070005) { std::printf("00cb0bc0_TEST FAIL\n"); return 1; }
    g_err = 0x12345;
    if (AmGetLastErrorToHResult() != (int)0x80072345) { std::printf("00cb0bc0_TEST FAIL\n"); return 1; }
    g_err = 0x8004DEAD;
    if (AmGetLastErrorToHResult() != (int)0x8004DEAD) { std::printf("00cb0bc0_TEST FAIL\n"); return 1; }
    std::printf("00cb0bc0_TEST PASS\n");
    return 0;
}
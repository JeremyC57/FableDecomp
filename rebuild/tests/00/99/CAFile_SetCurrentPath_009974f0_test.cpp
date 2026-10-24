// Behaviour test for 0x009974F0 CAFile::SetCurrentPath: the CRT chdir sees the new path first, then the cached current
// path holds the same text.
#include <cstdio>
#include <cwchar>
static wchar_t g_chdirSeen[64];
static wchar_t g_cached[64];
static int g_order = 0, g_chdirOrder = 0, g_assignOrder = 0;
static int FakeChdir(const wchar_t* p) { std::wcscpy(g_chdirSeen, p); g_chdirOrder = ++g_order; return 0; }
static void SetCurrentPath(const wchar_t* path)
{
    FakeChdir(path);
    std::wcscpy(g_cached, path); g_assignOrder = ++g_order;
}
int main()
{
    int fails = 0;
    SetCurrentPath(L"D:\\Fable\\Data");
    if (std::wcscmp(g_chdirSeen, L"D:\\Fable\\Data") != 0) ++fails;
    if (std::wcscmp(g_cached, L"D:\\Fable\\Data") != 0) ++fails;
    if (!(g_chdirOrder == 1 && g_assignOrder == 2)) ++fails;
    std::printf(fails ? "009974f0_TEST FAIL\n" : "009974f0_TEST PASS\n");
    return fails ? 1 : 0;
}
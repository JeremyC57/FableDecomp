// behaviour test for 0x0041A5B0 NResourceDirectoryNames::GetScoreboardImageCacheDir() (model: My Documents + "My Games\\Fable\\" + "Scoreboard\\")
#include <cstdio>
#include <string>
static std::wstring GetMyDocumentsDir() { return L"C:\\Docs\\"; }
static std::wstring GetUserDataDir() { return GetMyDocumentsDir() + L"My Games\\Fable\\"; }
static std::wstring Get() { return GetUserDataDir() + L"Scoreboard\\"; }
int main()
{
    int ok = Get() == std::wstring(L"C:\\Docs\\My Games\\Fable\\Scoreboard\\");
    if (!ok) { std::printf("0041a5b0_TEST FAIL\n"); return 1; }
    std::printf("0041a5b0_TEST PASS\n");
    return 0;
}
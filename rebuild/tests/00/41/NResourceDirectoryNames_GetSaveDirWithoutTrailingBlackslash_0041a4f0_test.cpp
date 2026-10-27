// behaviour test for 0x0041A4F0 NResourceDirectoryNames::GetSaveDirWithoutTrailingBlackslash() (model: My Documents + "My Games\\Fable\\" + "Saves")
#include <cstdio>
#include <string>
static std::wstring GetMyDocumentsDir() { return L"C:\\Docs\\"; }
static std::wstring GetUserDataDir() { return GetMyDocumentsDir() + L"My Games\\Fable\\"; }
static std::wstring Get() { return GetUserDataDir() + L"Saves"; }
int main()
{
    int ok = Get() == std::wstring(L"C:\\Docs\\My Games\\Fable\\Saves");
    if (!ok) { std::printf("0041a4f0_TEST FAIL\n"); return 1; }
    std::printf("0041a4f0_TEST PASS\n");
    return 0;
}
// behaviour test for 0x0041A650 NResourceDirectoryNames::GetTattooUserDir() (model: My Documents + "My Games\\Fable\\" + "Tattoos\\")
#include <cstdio>
#include <string>
static std::wstring GetMyDocumentsDir() { return L"C:\\Docs\\"; }
static std::wstring GetUserDataDir() { return GetMyDocumentsDir() + L"My Games\\Fable\\"; }
static std::wstring Get() { return GetUserDataDir() + L"Tattoos\\"; }
int main()
{
    int ok = Get() == std::wstring(L"C:\\Docs\\My Games\\Fable\\Tattoos\\");
    if (!ok) { std::printf("0041a650_TEST FAIL\n"); return 1; }
    std::printf("0041a650_TEST PASS\n");
    return 0;
}
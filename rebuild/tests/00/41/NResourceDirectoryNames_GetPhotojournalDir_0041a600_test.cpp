// behaviour test for 0x0041A600 NResourceDirectoryNames::GetPhotojournalDir() (model: My Documents + "My Games\\Fable\\" + "Photojournal\\")
#include <cstdio>
#include <string>
static std::wstring GetMyDocumentsDir() { return L"C:\\Docs\\"; }
static std::wstring GetUserDataDir() { return GetMyDocumentsDir() + L"My Games\\Fable\\"; }
static std::wstring Get() { return GetUserDataDir() + L"Photojournal\\"; }
int main()
{
    int ok = Get() == std::wstring(L"C:\\Docs\\My Games\\Fable\\Photojournal\\");
    if (!ok) { std::printf("0041a600_TEST FAIL\n"); return 1; }
    std::printf("0041a600_TEST PASS\n");
    return 0;
}
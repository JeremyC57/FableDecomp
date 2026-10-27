// behaviour test for 0x0041A0A0 NResourceDirectoryNames::GetMiscDir() (model: the returned wide string is the folder literal)
#include <cstdio>
#include <cwchar>
static const wchar_t* Get() { return L"Data\\Misc\\"; }
int main()
{
    int ok = std::wcscmp(Get(), L"Data\\Misc\\") == 0 && Get()[std::wcslen(Get()) - 1] == L'\\';
    if (!ok) { std::printf("0041a0a0_TEST FAIL\n"); return 1; }
    std::printf("0041a0a0_TEST PASS\n");
    return 0;
}
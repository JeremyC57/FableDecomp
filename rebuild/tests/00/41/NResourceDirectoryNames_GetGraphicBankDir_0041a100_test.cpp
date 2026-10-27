// behaviour test for 0x0041A100 NResourceDirectoryNames::GetGraphicBankDir() (model: the returned wide string is the folder literal)
#include <cstdio>
#include <cwchar>
static const wchar_t* Get() { return L"Data\\Graphics\\"; }
int main()
{
    int ok = std::wcscmp(Get(), L"Data\\Graphics\\") == 0 && Get()[std::wcslen(Get()) - 1] == L'\\';
    if (!ok) { std::printf("0041a100_TEST FAIL\n"); return 1; }
    std::printf("0041a100_TEST PASS\n");
    return 0;
}
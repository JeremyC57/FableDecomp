// behaviour test for 0x0041A1E0 NResourceDirectoryNames::GetShaderBankDir() (model: the returned wide string is the folder literal)
#include <cstdio>
#include <cwchar>
static const wchar_t* Get() { return L"Data\\Shaders\\"; }
int main()
{
    int ok = std::wcscmp(Get(), L"Data\\Shaders\\") == 0 && Get()[std::wcslen(Get()) - 1] == L'\\';
    if (!ok) { std::printf("0041a1e0_TEST FAIL\n"); return 1; }
    std::printf("0041a1e0_TEST PASS\n");
    return 0;
}
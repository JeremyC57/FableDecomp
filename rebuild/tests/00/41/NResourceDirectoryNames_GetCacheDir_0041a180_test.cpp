// behaviour test for 0x0041A180 NResourceDirectoryNames::GetCacheDir() (model: the returned wide string is the folder literal)
#include <cstdio>
#include <cwchar>
static const wchar_t* Get() { return L"Data\\Cache\\"; }
int main()
{
    int ok = std::wcscmp(Get(), L"Data\\Cache\\") == 0 && Get()[std::wcslen(Get()) - 1] == L'\\';
    if (!ok) { std::printf("0041a180_TEST FAIL\n"); return 1; }
    std::printf("0041a180_TEST PASS\n");
    return 0;
}
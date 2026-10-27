// behaviour test for 0x0041A2D0: GetDVDDialogueDir builds L"Data\\lang\\" + language + L"\\"
#include <cstdio>
#include <cwchar>
static const wchar_t* Language() { return L"English"; }
static void Build(wchar_t* out) { std::wcscpy(out, L"Data\\lang\\"); std::wcscat(out, Language()); std::wcscat(out, L"\\"); }
int main()
{
    wchar_t buf[64]; Build(buf);
    if (std::wcscmp(buf, L"Data\\lang\\English\\") != 0) { std::printf("0041a2d0_TEST FAIL\n"); return 1; }
    std::printf("0041a2d0_TEST PASS\n");
    return 0;
}
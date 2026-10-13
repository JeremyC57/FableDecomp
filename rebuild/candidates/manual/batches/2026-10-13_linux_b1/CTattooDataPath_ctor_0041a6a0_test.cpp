#include <cstdio>

static const wchar_t* g_text = 0;
struct CWideString { void* p; CWideString(const wchar_t* t) { g_text = t; p = 0; } };
struct CTattooDataPath { CWideString Path; CTattooDataPath() : Path(L"Data\\Tattoos\\") {} };
int main()
{
    CTattooDataPath tag;
    if (!g_text || g_text[0] != L'D' || g_text[4] != L'\\') { std::printf("0041a6a0_TEST FAIL\n"); return 1; }
    std::printf("0041a6a0_TEST PASS\n");
    return 0;
}

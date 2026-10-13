#include <cstdio>

struct CWStringParser { void* vt; wchar_t* String; unsigned long StringPos; unsigned long Line;
    void Init(wchar_t* t) { String = t; StringPos = 0; Line = 0; } };
int main()
{
    wchar_t text[4] = { 65, 66, 0, 0 }; CWStringParser p; p.StringPos = 9; p.Line = 9; p.Init(text);
    if (p.String != text || p.StringPos != 0 || p.Line != 0) { std::printf("00a0c8e0_TEST FAIL\n"); return 1; }
    std::printf("00a0c8e0_TEST PASS\n");
    return 0;
}

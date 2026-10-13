#include <cstdio>

struct CStateBlock { long Value; unsigned char Flag; };
struct CBankStateBlock { void* vt; CStateBlock State; };
struct CShaderBankStateBlock { void* vt; CStateBlock State;
    void Set(const CBankStateBlock* s) { State.Flag = s->State.Flag; State.Value = s->State.Value; } };
int main()
{
    CBankStateBlock src; src.State.Value = 77; src.State.Flag = 3;
    CShaderBankStateBlock dst; dst.State.Value = 0; dst.State.Flag = 0; dst.Set(&src);
    if (dst.State.Value != 77 || dst.State.Flag != 3) { std::printf("00a61f40_TEST FAIL\n"); return 1; }
    std::printf("00a61f40_TEST PASS\n");
    return 0;
}
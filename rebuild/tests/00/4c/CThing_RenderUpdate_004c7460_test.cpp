// agentFAM04 (2026-11-11): portable prelude so the unchanged test also builds with g++ -std=c++11 on a 64-bit host (no MSVC keywords there).
#ifndef _MSC_VER
#define __fastcall
#define __thiscall
#define __cdecl
#define __stdcall
#define __declspec(x)
#endif
#include <cstdio>
bool __fastcall fn_operator(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!fn_operator(&object, 1, 2)) { std::printf("AUTO_TINY_004c7460_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004c7460_TEST FAIL\n");
    return 1;
}
#include <cstdio>
extern "C" __declspec(dllimport) void* __stdcall GetModuleHandleA(const char*);
static void* __fastcall F(const char* n) { return GetModuleHandleA(n); }
int main()
{
    if (!F(0)) { std::printf("00ca4870_TEST FAIL\n"); return 1; }

    std::printf("00ca4870_TEST PASS\n");
    return 0;
}
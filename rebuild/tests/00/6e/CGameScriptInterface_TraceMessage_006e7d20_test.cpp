#include <cstdio>
void __stdcall CGameScriptInterface_TraceMessage(int name) {}
int main()
{
    CGameScriptInterface_TraceMessage(1);
    std::printf("AUTO_TINY_006e7d20_TEST PASS\n");
    return 0;
}
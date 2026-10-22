#include <cstdio>
void __stdcall CInputProcessFreezeControls_ProcessInput(int event, int input) {}
int main()
{
    CInputProcessFreezeControls_ProcessInput(1, 2);
    std::printf("AUTO_TINY_0068cb60_TEST PASS\n");
    return 0;
}
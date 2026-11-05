#include <cstdio>

class CEngineBillboardGenerator;

unsigned long __fastcall CEngineBillboardGenerator_PeekSceneFilterFlags(CEngineBillboardGenerator* generator);

int main()
{
    unsigned long r = CEngineBillboardGenerator_PeekSceneFilterFlags((CEngineBillboardGenerator*)0);
    if (r == 1)
        std::printf("00b8c780_TEST PASS\n");
    else
        std::printf("FAIL %lu\n", r);
    return 0;
}
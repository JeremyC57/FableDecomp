#include <cstdio>

class CEngineSimulationManager;

unsigned long __fastcall CEngineSimulationManager_PeekSceneFilterFlags(CEngineSimulationManager* manager);

class CEngineSimulationManager { public: int dummy; };

int main()
{
    CEngineSimulationManager obj;
    unsigned long r = CEngineSimulationManager_PeekSceneFilterFlags(&obj);
    if (r == 3UL)
        std::printf("00b83d10_TEST PASS\n");
    else
        std::printf("FAIL %lu\n", r);
    return 0;
}
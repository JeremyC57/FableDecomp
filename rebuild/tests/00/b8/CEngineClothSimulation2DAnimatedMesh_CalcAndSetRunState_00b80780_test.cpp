// behaviour test for 00b80780 (standalone copy of the model)
#include <cstdio>

class CEngineSimulationBase { public: virtual ~CEngineSimulationBase() {} long RunState; void SetRunState(long s) { RunState = s; } };
class CEngineClothSimulation2DAnimatedMesh : public CEngineSimulationBase { public: bool CalcAndSetRunState(); };
bool CEngineClothSimulation2DAnimatedMesh::CalcAndSetRunState() { SetRunState(0); return RunState != 1; }

int main()
{
    CEngineClothSimulation2DAnimatedMesh m; m.RunState = 1;
    bool r = m.CalcAndSetRunState();
    if (!(r == true && m.RunState == 0)) { std::printf("00b80780_TEST FAIL\n"); return 1; }
    std::printf("00b80780_TEST PASS\n");
    return 0;
}
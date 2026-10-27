// Behaviour test for 0x00B806D0 CEngineSimulationBase::GetMaximumDistanceError() const -- the simulation manager's MaxDistanceError
#include <cstdio>
class CEngineSimulationManager { public: float MaxDistanceError; float GetMaximumDistanceError() const { return MaxDistanceError; } };
static CEngineSimulationManager* g_pEngineSimulationManager = 0;
class CEngineSimulationBase {
public:
    virtual ~CEngineSimulationBase() { }
    virtual float GetMaximumDistanceError() const { return g_pEngineSimulationManager->GetMaximumDistanceError(); }
};
int main()
{
    CEngineSimulationManager m; m.MaxDistanceError = 0.25f;
    g_pEngineSimulationManager = &m;
    CEngineSimulationBase b;
    if (b.GetMaximumDistanceError() != 0.25f) { std::printf("00b806d0_TEST FAIL\n"); return 1; }
    m.MaxDistanceError = 3.0f;
    if (b.GetMaximumDistanceError() != 3.0f) { std::printf("00b806d0_TEST FAIL\n"); return 1; }
    std::printf("00b806d0_TEST PASS\n");
    return 0;
}
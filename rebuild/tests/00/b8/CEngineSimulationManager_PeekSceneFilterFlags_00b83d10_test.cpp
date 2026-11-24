#include <cstdio>

class CEngineSimulationManagerLayout;

// NF335 (NF324 Q2): the row now defines the PDB member CEngineSimulationManager::PeekSceneFilterFlags
// (?PeekSceneFilterFlags@CEngineSimulationManager@@UBEKXZ).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineSimulationManager { public:
    virtual unsigned long PeekSceneFilterFlags() const;
};

class CEngineSimulationManagerLayout { public: int dummy; };

int main()
{
    CEngineSimulationManagerLayout obj;
    unsigned long r = ((CEngineSimulationManager*)(&obj))->CEngineSimulationManager::PeekSceneFilterFlags();
    if (r == 3UL)
        std::printf("00b83d10_TEST PASS\n");
    else
        std::printf("FAIL %lu\n", r);
    return 0;
}
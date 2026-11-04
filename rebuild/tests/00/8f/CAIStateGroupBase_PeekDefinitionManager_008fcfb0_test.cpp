// Behaviour test for CAIStateGroupBase::PeekDefinitionManager 0x008FCFB0: the state group reaches the definition manager through its
// brain's main game component (brain at +4, component at +0x14), so swapping the component swaps the manager returned.
#include <cstdio>
struct CGameDefinitionManager { int id; };
struct CMainGameComponent { CGameDefinitionManager* Defs; const CGameDefinitionManager& PeekDefinitionManager() const { return *Defs; } };
struct CAIBrain { unsigned char Before[0x14]; CMainGameComponent* Component; };
struct CAIStateGroupBase {
    void* VPtr; CAIBrain* PBrain;
    const CGameDefinitionManager& PeekDefinitionManager() const { return PBrain->Component->PeekDefinitionManager(); }
};
int main()
{
    CGameDefinitionManager d1 = { 1 }, d2 = { 2 };
    CMainGameComponent c1 = { &d1 }, c2 = { &d2 };
    CAIBrain brain; brain.Component = &c1;
    CAIStateGroupBase g; g.PBrain = &brain;
    if (&g.PeekDefinitionManager() != &d1) { std::printf("008fcfb0_TEST FAIL\n"); return 1; }
    brain.Component = &c2;
    if (&g.PeekDefinitionManager() != &d2) { std::printf("008fcfb0_TEST FAIL\n"); return 1; }
    std::printf("008fcfb0_TEST PASS\n");
    return 0;
}
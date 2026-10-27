// Behaviour test for 004c7ae0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CThingManager { public: void* DefinitionManager; void* World; void* WorldMap; unsigned int* WorldSeed; void* PlayerManager; };
CThingManager* g_pThingManager;
class CThing { public: unsigned int* GetPWorldSeed() const { return g_pThingManager->WorldSeed; } };
int main() {
    unsigned int seed = 1234; CThingManager tm; tm.WorldSeed = &seed; g_pThingManager = &tm;
    CThing t;
    if (t.GetPWorldSeed() != &seed || *t.GetPWorldSeed() != 1234) { std::printf("004c7ae0_TEST FAIL\n"); return 1; }
    { std::printf("004c7ae0_TEST PASS\n"); return 0; }
}
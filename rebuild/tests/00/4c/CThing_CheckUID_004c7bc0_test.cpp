// Behaviour test for CThing::CheckUID 0x004C7BC0: the release body only queries the current thing manager
// (IsSerialisingGameState) and discards the answer, so it must call the manager once, change no state and return nothing.
#include <cstdio>
struct CThingManager { mutable int Queries; bool SerialisingGameState; bool IsSerialisingGameState() const { ++Queries; return SerialisingGameState; } };
static CThingManager* PCurrentThingManager = 0;
struct CThing { long UID; void CheckUID() const { PCurrentThingManager->IsSerialisingGameState(); } };
int main()
{
    CThingManager manager = { 0, true };
    PCurrentThingManager = &manager;
    CThing thing; thing.UID = 77;
    thing.CheckUID();
    if (manager.Queries != 1 || thing.UID != 77 || !manager.SerialisingGameState) { std::printf("004c7bc0_TEST FAIL\n"); return 1; }
    std::printf("004c7bc0_TEST PASS\n");
    return 0;
}
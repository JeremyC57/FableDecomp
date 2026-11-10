// Behaviour test for CThingAICreature::ForceAIReset (0x006D5AB0): the reset is forwarded to the brain the counted pointer holds.
#include <stdio.h>
static const void* g_reset;
struct Brain { int Id; void ForceReset() { g_reset = this; } };
template <class T> struct CountedPtr { T* Data; void* Info; T* operator->() const { return Data; } };
struct Creature {
    CountedPtr<Brain> PBrain;
    void ForceAIReset() { PBrain->ForceReset(); }
};
int main()
{
    Brain b; b.Id = 7;
    Creature c; c.PBrain.Data = &b; c.PBrain.Info = 0;
    c.ForceAIReset();
    if (g_reset != &b) { printf("006d5ab0_TEST FAIL\n"); return 1; }
    printf("006d5ab0_TEST PASS\n");
    return 0;
}

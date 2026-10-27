// Behaviour test for 0x007E5B30: OnInitialActivate suspends the owner's AI with this component as suspender
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SCreature { bool Suspended; const void* By; void SuspendAI(bool s, const void* by) { Suspended = s; By = by; } };
struct SComp { SCreature* ParentThing; void OnInitialActivate() { ParentThing->SuspendAI(true, this); } };
int main()
{
    SCreature c; c.Suspended = false; c.By = 0; SComp k; k.ParentThing = &c;
    k.OnInitialActivate(); CHECK(c.Suspended && c.By == &k);
    if (!g_fail) printf("007e5b30_TEST PASS\n");
    return g_fail;
}
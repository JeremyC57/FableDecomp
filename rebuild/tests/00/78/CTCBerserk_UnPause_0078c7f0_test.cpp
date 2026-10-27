// Behaviour test for 0x0078C7F0: UnPause re-adds berserk mode 0x24 unless active
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SModes { bool Active; int Added; bool IsModeActive(long) const { return Active; } void AddMode(long) { ++Added; Active = true; } };
struct SBerserk { bool Pausable, Paused; SModes* Modes;
    void UnPause() { if (Pausable && Paused) { Paused = false; if (!Modes->IsModeActive(0x24)) Modes->AddMode(0x24); } } };
int main()
{
    SModes m; m.Active = false; m.Added = 0; SBerserk b; b.Modes = &m;
    b.Pausable = true; b.Paused = true; b.UnPause(); CHECK(!b.Paused && m.Added == 1);
    b.Paused = true; b.UnPause(); CHECK(m.Added == 1);
    b.Pausable = false; b.Paused = true; m.Active = false; b.UnPause(); CHECK(b.Paused && m.Added == 1);
    if (!g_fail) printf("0078c7f0_TEST PASS\n");
    return g_fail;
}
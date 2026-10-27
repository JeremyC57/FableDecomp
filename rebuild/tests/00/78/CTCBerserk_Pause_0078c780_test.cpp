// Behaviour test for 0x0078C780: Pause removes berserk mode 0x24 once, only when pausable and running
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SModes { int Removed; long LastMode; void RemoveMode(long m) { ++Removed; LastMode = m; } };
struct SBerserk { bool Pausable, Paused; SModes* Modes;
    void Pause() { if (Pausable && !Paused) { Paused = true; Modes->RemoveMode(0x24); } } };
int main()
{
    SModes m; m.Removed = 0; m.LastMode = 0; SBerserk b; b.Modes = &m;
    b.Pausable = false; b.Paused = false; b.Pause(); CHECK(m.Removed == 0 && !b.Paused);
    b.Pausable = true; b.Pause(); CHECK(m.Removed == 1 && b.Paused && m.LastMode == 0x24);
    b.Pause(); CHECK(m.Removed == 1);
    if (!g_fail) printf("0078c780_TEST PASS\n");
    return g_fail;
}
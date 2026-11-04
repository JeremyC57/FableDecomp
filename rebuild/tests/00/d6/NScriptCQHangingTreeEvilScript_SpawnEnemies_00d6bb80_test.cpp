// Behaviour test for CQ_HangingTreeEvilScript::SpawnEnemies (0x00D6BB80): spawn count enemies then the carried-over guards
// (zeroing the carry-over), collect the assault guards and start MonitorEnemies; a terminating thread stops at once.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Script { long CarryOverGuards; int spawned, collected, threads, termAfter, checks;
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void SpawnEnemies(long count) {
        for (long i = 0; i < count; ++i) { if (IsActiveThreadTerminating()) return; ++spawned; }
        if (IsActiveThreadTerminating()) return;
        for (long j = 0; j < CarryOverGuards; ++j) { if (IsActiveThreadTerminating()) return; ++spawned; }
        if (IsActiveThreadTerminating()) return;
        CarryOverGuards = 0; ++collected; ++threads; } };
int main()
{
    Script a = { 2, 0, 0, 0, 100, 0 };
    a.SpawnEnemies(3);
    Script b = { 2, 0, 0, 0, 1, 0 };
    b.SpawnEnemies(3);
    bool ok = a.spawned == 5 && a.CarryOverGuards == 0 && a.collected == 1 && a.threads == 1 && b.spawned == 1 && b.threads == 0 && b.CarryOverGuards == 2;
    printf(ok ? "00d6bb80_TEST PASS\n" : "00d6bb80_TEST FAIL\n");
    return ok ? 0 : 1;
}
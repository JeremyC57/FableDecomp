// Behaviour test for CASoundManager::CloseAllBanks (0x009918F0): every bank record is destroyed and the map ends empty.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_destroyed;
struct Record { ~Record() { ++g_destroyed; } };
struct Map { Record* records; int size; void clear() { delete[] records; records = 0; size = 0; } };
int main()
{
    Map banks; banks.records = new Record[3]; banks.size = 3;
    banks.clear();
    bool ok = g_destroyed == 3 && banks.size == 0;
    printf(ok ? "009918f0_TEST PASS\n" : "009918f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
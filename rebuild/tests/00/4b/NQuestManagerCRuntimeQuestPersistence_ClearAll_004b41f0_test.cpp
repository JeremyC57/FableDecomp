// Behaviour test for 0x004B41F0: ClearAll empties both persist-script maps (erase all, then Clear).
#include <cstdio>
struct Map { int n; int destroyed; int clears; void erase_all() { destroyed += n; n = 0; } void Clear() { ++clears; } void EraseAndClear() { erase_all(); Clear(); } };
struct P { Map PersistScripts, MasterDataPersistScripts; void ClearAll() { PersistScripts.EraseAndClear(); MasterDataPersistScripts.EraseAndClear(); } };
int main()
{
    P p = {{3, 0, 0}, {2, 0, 0}}; p.ClearAll();
    bool ok = p.PersistScripts.n == 0 && p.PersistScripts.destroyed == 3 && p.PersistScripts.clears == 1 && p.MasterDataPersistScripts.n == 0 && p.MasterDataPersistScripts.destroyed == 2 && p.MasterDataPersistScripts.clears == 1;
    if (!ok) { std::printf("004b41f0_TEST FAIL\n"); return 1; }
    std::printf("004b41f0_TEST PASS\n");
    return 0;
}
// Test: CDefClassBase::UnloadResource -- clears the manager's instantiated def pointer unless more than two references hold it
#include <cstdio>
struct CDef { long IVRefCount; };
struct CInstantiatedDefInfo { CDef* Object; };
static bool UnloadResource(CInstantiatedDefInfo* info)
{
    if (info->Object && info->Object->IVRefCount > 2) return false;
    if (info->Object) --info->Object->IVRefCount;       // releasing the counted pointer
    info->Object = 0;
    return true;
}
int main()
{
    CDef held = { 3 }, two = { 2 };
    CInstantiatedDefInfo a = { &held }, b = { &two }, c = { 0 };
    if (UnloadResource(&a) || a.Object != &held || !UnloadResource(&b) || b.Object || two.IVRefCount != 1 || !UnloadResource(&c)) { std::printf("009ace90_TEST FAIL\n"); return 1; }
    std::printf("009ace90_TEST PASS\n");
    return 0;
}
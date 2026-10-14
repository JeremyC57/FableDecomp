#include <cstdio>
int g_instances = 0;
struct CInitBaseClass { bool flag04; CInitBaseClass() : flag04(false) { ++g_instances; } };
struct CIEngine : CInitBaseClass { bool initialised; CIEngine() : CInitBaseClass() { initialised = false; } };
int main()
{
    CIEngine e; bool ok = g_instances == 1 && !e.flag04 && !e.initialised;
    if (!ok) { std::printf("00b23750_TEST FAIL\n"); return 1; }
    std::printf("00b23750_TEST PASS\n"); return 0;
}
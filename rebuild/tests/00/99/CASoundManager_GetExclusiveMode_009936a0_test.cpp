// Behaviour test for CASoundManager::GetExclusiveMode (0x009936A0): return the exclusive-mode flag. Host model.
#include <cstdio>
struct Manager { bool ExclusiveMode; bool GetExclusiveMode() const { return ExclusiveMode; } };
int main()
{
    Manager m; m.ExclusiveMode = true;  bool ok = m.GetExclusiveMode();
    m.ExclusiveMode = false; ok = ok && !m.GetExclusiveMode();
    if (!ok) { std::printf("009936a0_TEST FAIL\n"); return 1; }
    std::printf("009936a0_TEST PASS\n");
    return 0;
}

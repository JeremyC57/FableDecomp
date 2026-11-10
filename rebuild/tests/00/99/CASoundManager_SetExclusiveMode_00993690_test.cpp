// Behaviour test for CASoundManager::SetExclusiveMode (0x00993690): store the exclusive-mode flag. Host model.
#include <cstdio>
struct Manager { bool ExclusiveMode; void SetExclusiveMode(bool value) { ExclusiveMode = value; } };
int main()
{
    Manager m; m.ExclusiveMode = false;
    m.SetExclusiveMode(true);  bool ok = m.ExclusiveMode;
    m.SetExclusiveMode(false); ok = ok && !m.ExclusiveMode;
    if (!ok) { std::printf("00993690_TEST FAIL\n"); return 1; }
    std::printf("00993690_TEST PASS\n");
    return 0;
}

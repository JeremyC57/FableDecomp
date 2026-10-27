#include <cstdio>
// Behaviour model: the flag is set only for a valid thing that has the component.
struct C { bool flag; };
static void set(bool valid, C* comp, bool v) { if (valid && comp) comp->flag = v; }
int main()
{
    C c = { false };
    set(false, &c, true); if (c.flag) { std::printf("0089e880_TEST FAIL\n"); return 1; }
    set(true, &c, true); if (!c.flag) { std::printf("0089e880_TEST FAIL\n"); return 1; }
    set(true, 0, false);
    std::printf("0089e880_TEST PASS\n");
    return 0;
}
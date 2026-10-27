#include <cstdio>
#include <cstring>
// Behaviour model: the base is built on the caller's scene list, then the enable variable is registered by name.
static const char* g_registered = 0;
struct Base { void* scenes; Base(void* s) : scenes(s) { } void AddEnable(const char* n) { g_registered = n; } };
struct Mgr : Base { Mgr(void* s) : Base(s) { AddEnable("EnableChangingPrimitives"); } };
int main()
{
    int list = 0; Mgr m(&list);
    if (m.scenes != &list || !g_registered || std::strcmp(g_registered, "EnableChangingPrimitives")) { std::printf("00bc5570_TEST FAIL\n"); return 1; }
    std::printf("00bc5570_TEST PASS\n");
    return 0;
}
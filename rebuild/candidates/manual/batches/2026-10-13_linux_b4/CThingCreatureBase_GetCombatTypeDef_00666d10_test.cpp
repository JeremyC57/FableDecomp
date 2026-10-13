#include <cstdio>

static int g_rel = 0;
struct Base { virtual void v00() {} virtual void Release() { ++g_rel; } long RefCount; };
struct P { Base* Object; ~P() { Base* o = Object; if (o) { if (--o->RefCount == 0) o->Release(); } } };
static Base g_def;
struct C { P GetP() { P p; p.Object = &g_def; ++g_def.RefCount; return p; } Base* Get() { return GetP().Object; } };
int main()
{
    g_def.RefCount = 0; C c; Base* b = c.Get();
    if (b != &g_def || g_def.RefCount != 0 || g_rel != 1) { std::printf("00666d10_TEST FAIL\n"); return 1; }
    std::printf("00666d10_TEST PASS\n");
    return 0;
}

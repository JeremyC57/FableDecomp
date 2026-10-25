#include <cstdio>
class CScriptHost;
static CScriptHost* g_owner = 0; static unsigned long g_group = 0;
struct CDriverGroup { unsigned char _body[0x20]; CDriverGroup(CScriptHost* owner, unsigned long group) { g_owner = owner; g_group = group; } };
class CScriptHost { public: CDriverGroup* CreateGroup(unsigned long group) { return new CDriverGroup(this, group); } };
int main()
{
    CScriptHost host;
    CDriverGroup* g = host.CreateGroup(7);
    bool ok = g != 0 && g_owner == &host && g_group == 7;
    delete g;
    if (!ok) { std::printf("00c0e8e0_TEST FAIL\n"); return 1; }
    std::printf("00c0e8e0_TEST PASS\n");
    return 0;
}

#include <cstdio>
struct CDriverGroup { unsigned long number; };
static CDriverGroup g_groups[2] = { { 3 }, { 5 } };
struct CGroupRegistry { virtual CDriverGroup* Find(const unsigned long& key) { for (int i = 0; i < 2; ++i) if (g_groups[i].number == key) return &g_groups[i]; return 0; } };
class CScriptHost { public: unsigned char _pad_0x00[0x74]; CGroupRegistry m_groups; CDriverGroup* FindGroup(unsigned long group) { return m_groups.Find(group); } };
int main()
{
    CScriptHost host;
    if (host.FindGroup(5) != &g_groups[1] || host.FindGroup(4) != 0 || (char*)&host.m_groups - (char*)&host != 0x74) { std::printf("00c0e950_TEST FAIL\n"); return 1; }
    std::printf("00c0e950_TEST PASS\n");
    return 0;
}

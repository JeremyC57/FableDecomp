#include <cstdio>
struct CCharString { const char* text; };
static const CCharString* g_added = 0;
struct CDefStringTable { unsigned char _pad[0x34]; bool Locked; unsigned char ReadOnly; long AddString(const CCharString& t) { g_added = &t; return 123; } };
CDefStringTable g_DefStringTable;
struct CDefString { long TablePos; void SetDefaultString(const CCharString& t) { if (!g_DefStringTable.Locked) TablePos = g_DefStringTable.AddString(t); } };
int main()
{
    CCharString s = { "x" }; CDefString d; d.TablePos = -1;
    g_DefStringTable.Locked = true; d.SetDefaultString(s);
    if (d.TablePos != -1 || g_added != 0) { std::printf("009d5080_TEST FAIL\n"); return 1; }
    g_DefStringTable.Locked = false; d.SetDefaultString(s);
    if (d.TablePos != 123 || g_added != &s || (char*)&g_DefStringTable.Locked - (char*)&g_DefStringTable != 0x34) { std::printf("009d5080_TEST FAIL\n"); return 1; }
    std::printf("009d5080_TEST PASS\n"); return 0;
}
#include <cstdio>
static int g_order[8]; static int g_n = 0;
class CCharString { public:
    int m_tag;
    ~CCharString() { if (g_n < 8) g_order[g_n++] = m_tag; }
};
class CWideString { public:
    int m_tag;
    ~CWideString() { if (g_n < 8) g_order[g_n++] = m_tag; }
};
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
struct SSymbolMapEntry { CCharString first; CSymbolInfo second; };
void DestroySymbolMapEntry(SSymbolMapEntry* entry) { delete entry; }
int main()
{
    int bad = 0;
    SSymbolMapEntry* e = new SSymbolMapEntry;
    e->first.m_tag = 1; e->second.GroupName.m_tag = 2; e->second.EntryName.m_tag = 3;
    DestroySymbolMapEntry(e);
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) bad = 1;
    if (bad) { std::printf("006fb730_TEST FAIL\n"); return 1; }
    std::printf("006fb730_TEST PASS\n");
    return 0;
}
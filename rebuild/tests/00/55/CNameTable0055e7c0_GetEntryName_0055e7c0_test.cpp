#include <cstdio>
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { } CCharString(const CCharString& o) : PStringData(o.PStringData) { } };
struct SNamedEntry { CCharString m_name; long m_value[3]; };
struct CNameTable { unsigned char _pad[0x28]; SNamedEntry* m_pEntries; CCharString GetEntryName(const unsigned long& i) { return m_pEntries[i].m_name; } };
int main()
{
    SNamedEntry e[3]; e[0].m_name.PStringData = (void*)0x10; e[1].m_name.PStringData = (void*)0x20; e[2].m_name.PStringData = (void*)0x30;
    CNameTable t; t.m_pEntries = e; unsigned long i = 2;
    if (sizeof(SNamedEntry) != 16 || t.GetEntryName(i).PStringData != (void*)0x30) { std::printf("0055e7c0_TEST FAIL\n"); return 1; }
    i = 0; if (t.GetEntryName(i).PStringData != (void*)0x10) { std::printf("0055e7c0_TEST FAIL\n"); return 1; }
    std::printf("0055e7c0_TEST PASS\n"); return 0;
}
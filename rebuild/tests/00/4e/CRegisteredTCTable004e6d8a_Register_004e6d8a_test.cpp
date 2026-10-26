#include <stdio.h>
static int g_strDtors;
class CCharString { public: void* m_pStringData; ~CCharString() { ++g_strDtors; } };
struct CRegisteredTCInfo { long m_type; long m_index; CCharString m_name; };
struct SPair {
    CCharString first; CRegisteredTCInfo second;
    SPair(const CCharString& name, const CRegisteredTCInfo& info) { first.m_pStringData = name.m_pStringData; second.m_type = info.m_type; second.m_index = info.m_index; second.m_name.m_pStringData = info.m_name.m_pStringData; }
};
static SPair* g_pushed[4]; static int g_pushCount; static long g_pushedType;
struct CRetailVector_SPair {
    SPair* first; SPair* last; SPair* end;
    void push_back(const SPair& value) { g_pushed[g_pushCount++] = (SPair*)&value; g_pushedType = value.second.m_type; }
};
struct CRegisteredTCTable_004e6d8a {
    CRetailVector_SPair m_entries; unsigned char m_locked; unsigned char m_changed;
    void Register(const CCharString& name, const CRegisteredTCInfo& info);
};
void CRegisteredTCTable_004e6d8a::Register(const CCharString& name, const CRegisteredTCInfo& info)
{
    m_entries.push_back(SPair(name, info));
    m_changed = 1;
}
int main()
{
    CRegisteredTCTable_004e6d8a t; t.m_changed = 0; t.m_locked = 0;
    CCharString name; name.m_pStringData = (void*)0x1234; CRegisteredTCInfo info; info.m_type = 77; info.m_index = 3; info.m_name.m_pStringData = 0;
    int before = g_strDtors;
    t.Register(name, info);
    bool ok = g_pushCount == 1 && g_pushedType == 77 && t.m_changed == 1 && g_strDtors - before == 2 && sizeof(SPair) == 16;
    if (!ok) { printf("004e6d8a_TEST FAIL\n"); return 1; }
    printf("004e6d8a_TEST PASS\n"); return 0;
}
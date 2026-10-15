// behaviour test for 0x00AB0BF0 MorphEntry_FromValue: the referenced entry's name, or "" for a null name
#include <cstdio>
#include <cstring>
struct CStr { char buf[32]; CStr(const char* s, int) { std::strcpy(buf, s); } };
struct SMorphEntry { const char* m_pName; CStr GetName() const { return m_pName ? CStr(m_pName, -1) : CStr("", -1); } };
struct CParticleMorphs { const SMorphEntry* m_pEntry; CStr MorphEntry_FromValue() const { return m_pEntry->GetName(); } };
int main()
{
    SMorphEntry e1 = { "Spark" }, e2 = { 0 };
    CParticleMorphs a = { &e1 }, b = { &e2 };
    if (std::strcmp(a.MorphEntry_FromValue().buf, "Spark") != 0 || b.MorphEntry_FromValue().buf[0] != 0) { std::printf("00ab0bf0_TEST FAIL\n"); return 1; }
    std::printf("00ab0bf0_TEST PASS\n");
    return 0;
}
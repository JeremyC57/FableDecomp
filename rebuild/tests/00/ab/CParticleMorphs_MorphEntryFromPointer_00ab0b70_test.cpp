// behaviour test for 0x00AB0B70 MorphEntry_FromPointer: the held C string, or "" for null
#include <cstdio>
#include <cstring>
struct CStr { char buf[32]; CStr(const char* s, int) { std::strcpy(buf, s); } };
class CParticleMorphs { public: const char* m_pName; CStr MorphEntry_FromPointer() const { return m_pName ? CStr(m_pName, -1) : CStr("", -1); } };
int main()
{
    CParticleMorphs a = { "Glow" }, b = { 0 };
    if (std::strcmp(a.MorphEntry_FromPointer().buf, "Glow") != 0 || b.MorphEntry_FromPointer().buf[0] != 0) { std::printf("00ab0b70_TEST FAIL\n"); return 1; }
    std::printf("00ab0b70_TEST PASS\n");
    return 0;
}
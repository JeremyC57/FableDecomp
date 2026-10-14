#include <cstdio>
#include <cstring>
static int g_set = 0; static const char* g_name = 0;
struct CCharString { void* m_pData; operator const char*() const { return (const char*)m_pData; } };
struct CNamedObject_00a0b6e0 { virtual void SetName(const char* n) { ++g_set; g_name = n; } };
struct CNamedOwner_00a0b6e0 { unsigned char _pad[0x10]; CNamedObject_00a0b6e0* m_pObject; void SetName(const CCharString& name) { if (m_pObject) m_pObject->SetName(name); } };
int main() {
    CNamedOwner_00a0b6e0 o; CNamedObject_00a0b6e0 obj; CCharString s; s.m_pData = (void*)"abc"; o.m_pObject = 0; o.SetName(s); if (g_set) { std::printf("00a0b6e0_TEST FAIL\n"); return 1; }
    o.m_pObject = &obj; o.SetName(s); if (g_set != 1 || std::strcmp(g_name, "abc")) { std::printf("00a0b6e0_TEST FAIL\n"); return 1; }
    std::printf("00a0b6e0_TEST PASS\n"); return 0;
}

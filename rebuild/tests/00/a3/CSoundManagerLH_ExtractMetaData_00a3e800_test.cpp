#include <cstdio>
#include <cstring>
static int g_set = 0; static const char* g_name = 0; static long g_value = 0;
namespace N_0099af10 { struct CCharString { void* m_pData; operator const char*() const { return (const char*)m_pData; } }; } using N_0099af10::CCharString;
struct CNamedObject_00a3e800 { virtual void SetNamedValue(const char* n, long v) { ++g_set; g_name = n; g_value = v; } };
struct CNamedOwner_00a3e800 { unsigned char _pad[0x40]; CNamedObject_00a3e800* m_pObject; void SetNamedValue(const CCharString& name, long value) { if (m_pObject) m_pObject->SetNamedValue(name, value); } };
int main() {
    CNamedOwner_00a3e800 o; CNamedObject_00a3e800 obj; CCharString s; s.m_pData = (void*)"xyz"; o.m_pObject = 0; o.SetNamedValue(s, 5); if (g_set) { std::printf("00a3e800_TEST FAIL\n"); return 1; }
    o.m_pObject = &obj; o.SetNamedValue(s, 7); if (g_set != 1 || std::strcmp(g_name, "xyz") || g_value != 7) { std::printf("00a3e800_TEST FAIL\n"); return 1; }
    std::printf("00a3e800_TEST PASS\n"); return 0;
}
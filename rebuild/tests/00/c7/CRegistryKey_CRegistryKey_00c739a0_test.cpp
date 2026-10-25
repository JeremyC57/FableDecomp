// Behaviour test for 00c739a0: CRegistryKey::~CRegistryKey -- an open key is closed once; a closed one is left alone
#include <cstdio>
static int g_closed = 0; static void* g_last = 0;
static long RegCloseKeyStub(void* key) { ++g_closed; g_last = key; return 0; }
class CRegistryKey { public: CRegistryKey() : m_hKey(0) { } virtual ~CRegistryKey() { if (m_hKey) { RegCloseKeyStub(m_hKey); m_hKey = 0; } } void* m_hKey; };
int main()
{
    int key;
    { CRegistryKey k; k.m_hKey = &key; }
    int ok = g_closed == 1 && g_last == &key;
    { CRegistryKey k; }
    ok = ok && g_closed == 1;
    std::printf(ok ? "00c739a0_TEST PASS\n" : "00c739a0_TEST FAIL\n");
    return ok ? 0 : 1;
}

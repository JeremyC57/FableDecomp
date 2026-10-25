// Behaviour test for 00c73800: CRegistryKey::Close -- closes and forgets an open key; idempotent
#include <cstdio>
static int g_closed = 0; static void* g_last = 0;
static long RegCloseKeyStub(void* key) { ++g_closed; g_last = key; return 0; }
class CRegistryKey { public: CRegistryKey() : m_hKey(0) { } virtual ~CRegistryKey() { } void* m_hKey; void Close() { if (m_hKey) { RegCloseKeyStub(m_hKey); m_hKey = 0; } } };
int main()
{
    int key; CRegistryKey k; k.m_hKey = &key;
    k.Close();
    int ok = g_closed == 1 && g_last == &key && k.m_hKey == 0;
    k.Close(); ok = ok && g_closed == 1;
    std::printf(ok ? "00c73800_TEST PASS\n" : "00c73800_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for 00c73790: CRegistryKey::CRegistryKey -- no key open
#include <cstdio>
#include <cstring>
#include <new>
class CRegistryKey { public: CRegistryKey() : m_hKey(0) { } virtual ~CRegistryKey() { } void* m_hKey; };
int main()
{
    unsigned char raw[sizeof(CRegistryKey)]; std::memset(raw, 0x77, sizeof raw);
    CRegistryKey* k = new (raw) CRegistryKey;
    int ok = k->m_hKey == 0 && sizeof(CRegistryKey) == 8;
    std::printf(ok ? "00c73790_TEST PASS\n" : "00c73790_TEST FAIL\n");
    return ok ? 0 : 1;
}

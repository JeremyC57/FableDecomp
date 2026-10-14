#include <cstdio>
struct SKeyPair { long first; long second; };
struct CKeyResolver { virtual void v00() {} virtual long Resolve(long first, long second) { return first * 100 + second; } };
struct CHandleMap { virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual long Lookup(const long& handle) { return handle + 1; } };
struct CMappedValueOwner_00c2a340 { unsigned char _pad0[0x68]; CHandleMap m_map; unsigned char _pad1[0x60]; bool m_bDisabled; unsigned char _pad2[3]; CKeyResolver* m_pResolver; long LookupMappedValue(const SKeyPair& key); };
long CMappedValueOwner_00c2a340::LookupMappedValue(const SKeyPair& key)
{
    if (m_pResolver != 0 && !m_bDisabled) return m_map.Lookup(m_pResolver->Resolve(key.first, key.second));
    return 0;
}
int main()
{
    CKeyResolver r; CMappedValueOwner_00c2a340 o; o.m_bDisabled = false; o.m_pResolver = 0; SKeyPair k = { 3, 7 };
    if (o.LookupMappedValue(k) != 0) { std::printf("00c2a340_TEST FAIL\n"); return 1; }
    o.m_pResolver = &r; if (o.LookupMappedValue(k) != 308) { std::printf("00c2a340_TEST FAIL\n"); return 1; }
    o.m_bDisabled = true; if (o.LookupMappedValue(k) != 0) { std::printf("00c2a340_TEST FAIL\n"); return 1; }
    std::printf("00c2a340_TEST PASS\n");
    return 0;
}
// Behaviour test for 00c19680: CObjectMap::GetScope -- the record's set at +0xc, else the owner's default set at +0x3c
#include <cstdio>
struct SScopeRecord { unsigned char _pad[0xc]; unsigned char m_set[4]; };
struct CObjectMapOwner { unsigned char _pad[0x3c]; unsigned char m_defaultSet[4]; };
struct CObjectMap { long m_0; CObjectMapOwner* m_pOwner;
    void* GetScope(SScopeRecord* record) { void* scope = m_pOwner->m_defaultSet; if (record) scope = record->m_set; return scope; } };
int main()
{
    CObjectMapOwner owner; SScopeRecord record; CObjectMap map; map.m_pOwner = &owner;
    if (map.GetScope(0) != (char*)&owner + 0x3c || map.GetScope(&record) != (char*)&record + 0xc) { std::printf("00c19680_TEST FAIL\n"); return 1; }
    std::printf("00c19680_TEST PASS\n");
    return 0;
}

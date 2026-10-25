#include <cstdio>
#include <cstring>
struct CValueSource { virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual long GetValue() { return 777; } };
struct CEntry { unsigned char _pad0[0x10]; unsigned long m_flags; unsigned char _pad1[0x3c]; CValueSource* m_pSource; unsigned char _pad2[0x9c]; long m_cachedValue; };
extern "C" long __stdcall CODA_GetSourcePosition(CEntry* entry)
{
    if (entry->m_pSource != 0 && (entry->m_flags & 0x216) != 0) return entry->m_pSource->GetValue();
    return entry->m_cachedValue;
}
int main()
{
    CValueSource src; CEntry e; std::memset(&e, 0, sizeof(e)); e.m_cachedValue = 5;
    if (CODA_GetSourcePosition(&e) != 5) { std::printf("00c35cc0_TEST FAIL\n"); return 1; }
    e.m_flags = 0x10; if (CODA_GetSourcePosition(&e) != 5) { std::printf("00c35cc0_TEST FAIL\n"); return 1; }
    e.m_pSource = &src; if (CODA_GetSourcePosition(&e) != 777) { std::printf("00c35cc0_TEST FAIL\n"); return 1; }
    e.m_flags = 0x100; if (CODA_GetSourcePosition(&e) != 5) { std::printf("00c35cc0_TEST FAIL\n"); return 1; }
    std::printf("00c35cc0_TEST PASS\n");
    return 0;
}
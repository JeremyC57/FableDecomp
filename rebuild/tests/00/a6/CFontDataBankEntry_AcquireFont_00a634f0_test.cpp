#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_created = 0; static long g_a = 0, g_b = 0; static void* g_bank = 0; static void* g_reset = 0; static long g_count = 1;
struct CStaticFontBank { unsigned char _body[0x40]; CStaticFontBank(long a, long b) { ++g_created; g_a = a; g_b = b; g_bank = this; } };
struct CCountedPointer_00a634f0 { CStaticFontBank* m_pObject; long* m_pCount;
    CCountedPointer_00a634f0() { }
    CCountedPointer_00a634f0(const CCountedPointer_00a634f0& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++*m_pCount; }
    void Reset(CStaticFontBank* o) { g_reset = o; m_pObject = o; m_pCount = &g_count; } };
class CFontDataBankEntry { public: virtual void v00() { } CCountedPointer_00a634f0 m_font; long Bank; long m_b;
    static CStaticFontBank* CreateBank(long a, long b) { return new CStaticFontBank(a, b); }
    CCountedPointer_00a634f0 AcquireFont() { if (!m_font.m_pObject) m_font.Reset(CreateBank(Bank, m_b)); return m_font; } };
int main() {
    CFontDataBankEntry e; e.m_font.m_pObject = 0; e.m_font.m_pCount = 0; e.Bank = 5; e.m_b = 6; long count = 1;
    { CCountedPointer_00a634f0 p = e.AcquireFont(); if (!g_bank || g_a != 5 || g_b != 6 || g_reset != g_bank || e.m_font.m_pObject != g_bank || p.m_pObject != g_bank || p.m_pCount != e.m_font.m_pCount || *p.m_pCount != 2) { std::printf("00a634f0_TEST FAIL\n"); return 1; } }
    { CCountedPointer_00a634f0 p = e.AcquireFont(); if (g_created != 1 || *p.m_pCount != 3) { std::printf("00a634f0_TEST FAIL\n"); return 1; } }
    e.m_font.m_pObject = (CStaticFontBank*)1; e.m_font.m_pCount = &count; { CCountedPointer_00a634f0 p = e.AcquireFont(); if (g_created != 1 || count != 2 || p.m_pObject != (CStaticFontBank*)1) { std::printf("00a634f0_TEST FAIL\n"); return 1; } }
    std::printf("00a634f0_TEST PASS\n"); return 0;
}
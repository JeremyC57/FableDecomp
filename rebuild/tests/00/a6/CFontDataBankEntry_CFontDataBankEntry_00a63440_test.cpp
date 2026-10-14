#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CCountedPointer_00a63440 { void* m_pObject; void* m_pCount; CCountedPointer_00a63440() : m_pObject(0), m_pCount(0) { } };
struct CFontDataBankEntry : CBase { CCountedPointer_00a63440 m_font; long m_c; long m_b; long m_a; virtual void v00() { } CFontDataBankEntry(long a, long b, long c) : CBase(), m_c(c), m_b(b), m_a(a) { } };
int main() {
    CFontDataBankEntry* e = new CFontDataBankEntry(1, 2, 3); if (g_base != 1 || e->m_font.m_pObject != 0 || e->m_font.m_pCount != 0 || e->m_c != 3 || e->m_b != 2 || e->m_a != 1) { std::printf("00a63440_TEST FAIL\n"); return 1; } delete e;
    std::printf("00a63440_TEST PASS\n"); return 0;
}
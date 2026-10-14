#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_section = 0; static void InitializeCriticalSection(void* s) { g_section = s; }
struct CCS { virtual void v00() { } unsigned char m_section[24]; CCS() { InitializeCriticalSection(m_section); } }; struct CHandler { virtual void v00() { } long m_4, m_8, m_c, m_10; unsigned char _pad[8]; CCS m_lock; CHandler() : m_lock() { m_8 = 0; m_4 = 0; m_c = 0; m_10 = 0; } };

int main() {
    CHandler* h = new CHandler; if (h->m_4 || h->m_8 || h->m_c || h->m_10 || g_section != h->m_lock.m_section) { std::printf("00c5d750_TEST FAIL\n"); return 1; } delete h;
    std::printf("00c5d750_TEST PASS\n"); return 0;
}

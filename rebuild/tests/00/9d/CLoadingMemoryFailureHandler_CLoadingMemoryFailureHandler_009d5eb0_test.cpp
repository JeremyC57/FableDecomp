#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_section = 0; static void InitializeCriticalSection(void* s) { g_section = s; }
struct SState { long m_count, m_limit, m_pending; unsigned char m_section[24]; SState() : m_count(0), m_limit(0), m_pending(0) { InitializeCriticalSection(m_section); } }; struct CHandler { virtual void v00() { } SState m_state; CHandler() : m_state() { } };

int main() {
    CHandler* h = new CHandler; if (h->m_state.m_count || h->m_state.m_limit || h->m_state.m_pending || g_section != h->m_state.m_section) { std::printf("009d5eb0_TEST FAIL\n"); return 1; } delete h;
    std::printf("009d5eb0_TEST PASS\n"); return 0;
}
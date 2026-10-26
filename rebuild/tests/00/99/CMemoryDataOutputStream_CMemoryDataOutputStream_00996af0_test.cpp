#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; static const char* g_text = 0; static long g_len = 0; struct CBaseNC { virtual void v00() { } CBaseNC() { ++g_base; } }; class CCharString { public: void* m_p; CCharString(const char* t, long n) : m_p(0) { g_text = t; g_len = n; } };
class CDataOutputStream : public CBaseNC { public: virtual void v01() { } CCharString m_lineTerminator; CDataOutputStream() : CBaseNC(), m_lineTerminator("\r\n", -1) { } };
struct SState { unsigned char* m_pFirst; unsigned char* m_pLast; long m_10; unsigned long m_position; SState() : m_pFirst(0), m_pLast(0), m_10(0), m_position(0) { } };
class CMemoryDataOutputStream : public CDataOutputStream { public: virtual void v01() { } SState m_state; CMemoryDataOutputStream() : CDataOutputStream(), m_state() { } };

int main() {
    CMemoryDataOutputStream s; if (g_base != 1 || std::strcmp(g_text, "\r\n") || g_len != -1 || s.m_state.m_pFirst || s.m_state.m_pLast || s.m_state.m_10 || s.m_state.m_position) { std::printf("00996af0_TEST FAIL\n"); return 1; }
    std::printf("00996af0_TEST PASS\n"); return 0;
}
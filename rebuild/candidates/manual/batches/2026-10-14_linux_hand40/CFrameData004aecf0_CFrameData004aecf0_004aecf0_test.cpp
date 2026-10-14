#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_cs = 0; static void* g_ns[2]; static int g_n = 0;
struct CCharString { void* m_pData; CCharString() { g_cs = this; } };
struct CNameString_0099aed0 { void* m_pData; CNameString_0099aed0() { g_ns[g_n++] = this; } };
struct CFrameData_004aecf0 { unsigned char _pad[0xc]; CCharString m_name; CNameString_0099aed0 m_textA; CNameString_0099aed0 m_textB; unsigned char _pad18; bool m_flagA; bool m_flagB; unsigned char _pad1b; long m_value;
    CFrameData_004aecf0() : m_flagA(false), m_flagB(false), m_value(0) { } };
int main() {
    CFrameData_004aecf0* d = new CFrameData_004aecf0; if (g_cs != &d->m_name || g_ns[0] != &d->m_textA || g_ns[1] != &d->m_textB || d->m_flagA || d->m_flagB || d->m_value != 0) { std::printf("004aecf0_TEST FAIL\n"); return 1; } delete d;
    std::printf("004aecf0_TEST PASS\n"); return 0;
}

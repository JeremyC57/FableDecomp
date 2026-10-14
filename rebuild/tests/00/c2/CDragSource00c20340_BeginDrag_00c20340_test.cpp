#include <cstdio>
struct CDragSource;
struct SDragDescriptor { long m_a, m_b, m_c; SDragDescriptor() { } SDragDescriptor(const SDragDescriptor& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { } };
static const void* g_submitted = 0; static long g_arg = 0; static CDragSource* g_src = 0;
static SDragDescriptor Build(CDragSource* s, long arg) { SDragDescriptor d; d.m_a = 1; d.m_b = arg; d.m_c = 3; g_src = s; return d; }
static void Submit(const void* p) { g_submitted = p; }
struct CDragSource { unsigned char _pad[0x0c]; unsigned char m_ready;
    void BeginDrag(long arg) { if (m_ready) { Submit(this); } else { SDragDescriptor d = Build(this, arg); g_arg = d.m_b; Submit(&d); } } };
int main() { CDragSource s; s.m_ready = 1; s.BeginDrag(5); if (g_submitted != &s || g_src) { std::printf("00c20340_TEST FAIL\n"); return 1; } s.m_ready = 0; s.BeginDrag(8); if (g_submitted == &s || g_src != &s || g_arg != 8) { std::printf("00c20340_TEST FAIL\n"); return 1; } std::printf("00c20340_TEST PASS\n"); return 0; }
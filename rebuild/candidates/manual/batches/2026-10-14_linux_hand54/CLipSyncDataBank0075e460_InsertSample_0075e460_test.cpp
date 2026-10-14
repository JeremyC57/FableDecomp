#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct C3DVector { float x, y, z; }; struct CCharString { void* m_p; ~CCharString() { std::strcat(g_seq, "s"); } }; struct SSource { C3DVector m_position; unsigned char _pad[0xc]; C3DVector m_direction; };
static const SSource* g_src = 0; static const C3DVector* g_dir = 0; static long g_a = -1, g_b = -1, g_flags = -1; static float g_scale = 0, g_range = 0;
struct SSample { unsigned char m_body[0x1c]; long m_1c; unsigned char _p[4]; long m_24; unsigned char _q[8]; CCharString m_name; long m_34;
    SSample() { } SSample(const SSource* s, const C3DVector* d, long a, long b, float sc, float r, long f) { g_src = s; g_dir = d; g_a = a; g_b = b; g_scale = sc; g_range = r; g_flags = f; std::strcat(g_seq, "C"); } };
struct CVec { SSample* m_first; SSample* m_last; SSample* m_end; SSample* insert(SSample* where, const SSample& v) { std::strcat(g_seq, "I"); (void)v; return where + 2; } };
struct CBank { unsigned char _pad[0x68]; CVec m_samples; SSample* m_pInsertAt; void Refresh() { std::strcat(g_seq, "U"); }
    void InsertSample(const SSource* src) { if (m_pInsertAt != m_samples.m_last) { m_pInsertAt = m_samples.insert(m_pInsertAt, SSample(src, &src->m_direction, m_pInsertAt->m_1c, m_pInsertAt->m_24, 1.0f, 2.0f, 0)); } Refresh(); } };

int main() {
    CBank b; SSample buf[4]; b.m_samples.m_first = buf; b.m_samples.m_last = buf + 2; b.m_samples.m_end = buf + 4; b.m_pInsertAt = buf + 1; buf[1].m_1c = 11; buf[1].m_24 = 22; SSource src; src.m_direction.x = 5;
    b.InsertSample(&src); if (b.m_pInsertAt != buf + 3 || g_src != &src || g_dir != &src.m_direction || g_a != 11 || g_b != 22 || g_scale != 1.0f || g_range != 2.0f || g_flags != 0 || std::strcmp(g_seq, "CIsU")) { std::printf("0075e460_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; b.m_pInsertAt = b.m_samples.m_last; b.InsertSample(&src); if (std::strcmp(g_seq, "U")) { std::printf("0075e460_TEST FAIL\n"); return 1; }
    std::printf("0075e460_TEST PASS\n"); return 0;
}

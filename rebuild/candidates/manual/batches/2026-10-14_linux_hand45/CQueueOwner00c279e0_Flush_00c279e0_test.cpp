#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[16] = ""; static const unsigned short* g_name = 0; static void* g_queue = 0; static long g_a = 1, g_b = 1;
static const unsigned short g_emptyName_0143bafc[1] = { 0 };
struct CScopedNamedLock_00c0ffc0 { unsigned char _body[8]; CScopedNamedLock_00c0ffc0(const unsigned short* n) { std::strcat(g_seq, "L"); g_name = n; } ~CScopedNamedLock_00c0ffc0() { std::strcat(g_seq, "U"); } };
static void ProcessQueue_00c35c50(void* q, long a, long b) { std::strcat(g_seq, "P"); g_queue = q; g_a = a; g_b = b; }
struct CQueueOwner_00c279e0 { unsigned char _pad[0xc]; void* m_pQueue; long m_pending; void Flush() { m_pending = 0; CScopedNamedLock_00c0ffc0 lock(g_emptyName_0143bafc); ProcessQueue_00c35c50(m_pQueue, 0, 0); } };
int main() {
    CQueueOwner_00c279e0 q; int queue; q.m_pQueue = &queue; q.m_pending = 9; q.Flush(); if (q.m_pending != 0 || std::strcmp(g_seq, "LPU") || g_name != g_emptyName_0143bafc || g_queue != &queue || g_a != 0 || g_b != 0) { std::printf("00c279e0_TEST FAIL\n"); return 1; }
    std::printf("00c279e0_TEST PASS\n"); return 0;
}

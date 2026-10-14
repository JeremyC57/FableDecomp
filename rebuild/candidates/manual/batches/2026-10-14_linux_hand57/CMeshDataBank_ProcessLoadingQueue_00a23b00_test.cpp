#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_ctx = 0; static double g_time = 0; static const void* g_qa = 0; struct CQueue { unsigned char m_body[4]; void Process(void* c, double t) { g_ctx = c; g_time = t; std::strcat(g_seq, (char*)(this == g_qa ? "A" : (const char*)m_body)); } };
struct CBank { unsigned char _pad[0x170]; CQueue m_queueA; unsigned char _p[0xf4]; CQueue m_queueC; unsigned char _q[0x78]; CQueue m_queueB; CBank() { g_qa = &m_queueA; std::strcpy((char*)m_queueB.m_body, "B"); std::strcpy((char*)m_queueC.m_body, "C"); }
    void ProcessLoadingQueue(void* c, double t) { m_queueA.Process(c, t); m_queueB.Process(c, t); m_queueC.Process(c, t); } };

int main() {
    CBank* b = new CBank; b->ProcessLoadingQueue((void*)5, 1.5); if (std::strcmp(g_seq, "ABC") || g_ctx != (void*)5 || g_time != 1.5) { std::printf("00a23b00_TEST FAIL\n"); return 1; } delete b;
    std::printf("00a23b00_TEST PASS\n"); return 0;
}

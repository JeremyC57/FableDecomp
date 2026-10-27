#include <cstdio>

// Behaviour model of the PostRequest slot (0x00C26A60): normalise the flag byte into the request (+0x76, short) and push it.
struct Request { short m_flag; Request* m_next; };
struct Queue { Request* m_head; int m_count; void Push(Request* r) { r->m_next = m_head; m_head = r; ++m_count; } };
struct Poster { Queue m_worker; void PostRequest(unsigned char flag, Request* r) { r->m_flag = flag != 0; m_worker.Push(r); } };

int main()
{

    Poster p; p.m_worker.m_head = 0; p.m_worker.m_count = 0;
    Request r1; r1.m_flag = 5; Request r2; r2.m_flag = 0;
    p.PostRequest(0, &r1); p.PostRequest(0x80, &r2);
    if (r1.m_flag != 0 || r2.m_flag != 1) { std::printf("00c26a60_TEST FAIL\n"); return 1; }
    if (p.m_worker.m_count != 2 || p.m_worker.m_head != &r2 || r2.m_next != &r1) { std::printf("00c26a60_TEST FAIL\n"); return 1; }
    std::printf("00c26a60_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00C6BFF0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>
#include <new>

struct P { void* m_pFirst; unsigned int m_middle; void* m_pLast; P() : m_pFirst(0), m_pLast(0) {} };
int main()
{
    unsigned char raw[sizeof(P)]; std::memset(raw, 0xAB, sizeof(raw));
    P* p = new ((void*)raw) P;
    if (p->m_pFirst != 0 || p->m_pLast != 0 || p->m_middle != 0xABABABABu) { std::printf("00c6bff0_TEST FAIL\n"); return 1; }
    std::printf("00c6bff0_TEST PASS\n");
    return 0;
}
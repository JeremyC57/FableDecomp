#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CLinkedItem_00c4cef0 { long m_0; CLinkedItem_00c4cef0* m_pPrev; CLinkedItem_00c4cef0* m_pNext;
    void Unlink() { if (m_pPrev) m_pPrev->m_pNext = m_pNext; if (m_pNext) m_pNext->m_pPrev = m_pPrev; m_pPrev = 0; m_pNext = 0; } };
int main() {
    CLinkedItem_00c4cef0 a, b, c; a.m_pPrev = 0; a.m_pNext = &b; b.m_pPrev = &a; b.m_pNext = &c; c.m_pPrev = &b; c.m_pNext = 0; b.Unlink();
    if (a.m_pNext != &c || c.m_pPrev != &a || b.m_pPrev || b.m_pNext) { std::printf("00c4cef0_TEST FAIL\n"); return 1; }
    a.Unlink(); if (a.m_pNext || c.m_pPrev) { std::printf("00c4cef0_TEST FAIL\n"); return 1; }
    std::printf("00c4cef0_TEST PASS\n"); return 0;
}

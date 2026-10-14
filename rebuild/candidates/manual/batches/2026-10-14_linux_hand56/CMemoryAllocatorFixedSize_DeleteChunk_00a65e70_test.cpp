#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { unsigned char _pad[8]; SChunk* m_pPrev; SChunk* m_pNext; unsigned char _p[4]; unsigned char* m_pFirstArea; };
struct CAlloc { unsigned char _pad[0xc1c]; SChunk* m_pFirstChunk; SChunk* m_pLastChunk; void DeleteChunk(SChunk* c) { if (c->m_pPrev) c->m_pPrev->m_pNext = c->m_pNext; if (c->m_pNext) c->m_pNext->m_pPrev = c->m_pPrev; if (m_pFirstChunk == c) m_pFirstChunk = c->m_pPrev; if (m_pLastChunk == c) m_pLastChunk = c->m_pNext; } };

int main() {
    SChunk a, b, c; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); std::memset(&c, 0, sizeof c); a.m_pNext = &b; b.m_pPrev = &a; b.m_pNext = &c; c.m_pPrev = &b; CAlloc* al = new CAlloc; al->m_pFirstChunk = &b; al->m_pLastChunk = &b;
    al->DeleteChunk(&b); if (a.m_pNext != &c || c.m_pPrev != &a || al->m_pFirstChunk != &a || al->m_pLastChunk != &c) { std::printf("00a65e70_TEST FAIL\n"); return 1; } al->DeleteChunk(&a); if (c.m_pPrev != 0 || al->m_pFirstChunk != 0) { std::printf("00a65e70_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a65e70_TEST PASS\n"); return 0;
}

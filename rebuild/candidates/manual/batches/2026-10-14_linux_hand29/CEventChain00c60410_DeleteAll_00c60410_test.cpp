#include <cstdio>
struct CChainEntry { unsigned char _pad[0x94]; CChainEntry* m_pNext; };
static CChainEntry* g_destroyed[4]; static int g_n = 0;
static void DestroyChainEntry(CChainEntry* e) { g_destroyed[g_n++] = e; }
struct CEventChain { unsigned char _pad[0x3c4]; CChainEntry* m_pFirst; void DeleteAll() { while (m_pFirst != 0) { CChainEntry* e = m_pFirst; m_pFirst = e->m_pNext; DestroyChainEntry(e); } } };
int main() { CChainEntry a, b; a.m_pNext = &b; b.m_pNext = 0; CEventChain c; c.m_pFirst = &a; c.DeleteAll(); if (c.m_pFirst || g_n != 2 || g_destroyed[0] != &a || g_destroyed[1] != &b) { std::printf("00c60410_TEST FAIL\n"); return 1; } std::printf("00c60410_TEST PASS\n"); return 0; }

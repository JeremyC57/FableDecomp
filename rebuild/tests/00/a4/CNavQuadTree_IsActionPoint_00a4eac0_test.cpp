#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_key = 0; static void* g_found = 0; struct CTree { void* m_pHead; void* Find(long k) { g_key = k; return g_found; } }; struct CNavQuadTree { unsigned char _pad[0x20]; CTree m_actionPoints; bool IsActionPoint(long k) { return m_actionPoints.Find(k) != m_actionPoints.m_pHead ? true : false; } };

int main() {
    CNavQuadTree* t = new CNavQuadTree; int head; t->m_actionPoints.m_pHead = &head; g_found = &head; if (t->IsActionPoint(3) || g_key != 3) { std::printf("00a4eac0_TEST FAIL\n"); return 1; } int node; g_found = &node; if (!t->IsActionPoint(4) || g_key != 4) { std::printf("00a4eac0_TEST FAIL\n"); return 1; } delete t;
    std::printf("00a4eac0_TEST PASS\n"); return 0;
}
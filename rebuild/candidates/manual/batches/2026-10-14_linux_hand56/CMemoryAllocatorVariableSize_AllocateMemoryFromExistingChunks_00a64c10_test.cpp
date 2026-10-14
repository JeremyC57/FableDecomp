#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SNode { long m_0; SNode* m_pLeft; unsigned char _pad[8]; SNode* m_pRight; long m_14; long m_largestFree; };
struct CAlloc { unsigned char _pad[0xc34]; SNode* m_pRoot; SNode* Find(long size) { SNode* best = 0; for (SNode* node = m_pRoot; node; ) { long key = node->m_largestFree; if (key == size) return node; if (key > size) { if (!best || key < best->m_largestFree) best = node; node = node->m_pRight; } else { node = node->m_pLeft; } } return best; } };

int main() {
    SNode n[5]; std::memset(n, 0, sizeof n); /* the search walks right for smaller keys: root 50, left 80, right 20 (left 30, right 10) */ n[0].m_largestFree = 50; n[1].m_largestFree = 80; n[2].m_largestFree = 20; n[3].m_largestFree = 30; n[4].m_largestFree = 10; n[0].m_pLeft = &n[1]; n[0].m_pRight = &n[2]; n[2].m_pLeft = &n[3]; n[2].m_pRight = &n[4];
    CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_pRoot = &n[0]; if (al->Find(50) != &n[0] || al->Find(25) != &n[3] || al->Find(15) != &n[2] || al->Find(60) != &n[1] || al->Find(90) != 0 || al->Find(5) != &n[4]) { std::printf("00a64c10_TEST FAIL\n"); return 1; } al->m_pRoot = 0; if (al->Find(1) != 0) { std::printf("00a64c10_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a64c10_TEST PASS\n"); return 0;
}

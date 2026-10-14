#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SNode { long m_0; SNode* m_pLeft; long m_8; SNode* m_pParent; SNode* m_pRight; };
struct CAlloc { unsigned char _pad[0xc34]; SNode* m_pRoot; void RotateLeft(SNode* node) { SNode* child = node->m_pRight; node->m_pRight = child->m_pLeft; if (child->m_pLeft) child->m_pLeft->m_pParent = node; child->m_pParent = node->m_pParent; if (!node->m_pParent) m_pRoot = child; else if (node == node->m_pParent->m_pRight) node->m_pParent->m_pRight = child; else node->m_pParent->m_pLeft = child; child->m_pLeft = node; node->m_pParent = child; } };

int main() {
    SNode p, n, r, rl; std::memset(&p, 0, sizeof p); std::memset(&n, 0, sizeof n); std::memset(&r, 0, sizeof r); std::memset(&rl, 0, sizeof rl); CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al);
    p.m_pLeft = &n; n.m_pParent = &p; n.m_pRight = &r; r.m_pParent = &n; r.m_pLeft = &rl; rl.m_pParent = &r; al->RotateLeft(&n);
    if (n.m_pRight != &rl || rl.m_pParent != &n || r.m_pParent != &p || p.m_pLeft != &r || r.m_pLeft != &n || n.m_pParent != &r) { std::printf("00a646e0_TEST FAIL\n"); return 1; }
    std::memset(&n, 0, sizeof n); std::memset(&r, 0, sizeof r); n.m_pRight = &r; r.m_pParent = &n; al->m_pRoot = &n; al->RotateLeft(&n); if (al->m_pRoot != &r || r.m_pLeft != &n || n.m_pParent != &r || n.m_pRight != 0) { std::printf("00a646e0_TEST FAIL\n"); return 1; }
    std::memset(&p, 0, sizeof p); std::memset(&n, 0, sizeof n); std::memset(&r, 0, sizeof r); p.m_pRight = &n; n.m_pParent = &p; n.m_pRight = &r; r.m_pParent = &n; al->RotateLeft(&n); if (p.m_pRight != &r) { std::printf("00a646e0_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a646e0_TEST PASS\n"); return 0;
}
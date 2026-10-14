#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct SPair { long m_first, m_second; }; struct SNode { unsigned char m_links[0x10]; SPair m_value; }; struct CTree { SNode* BuyNode(const SPair& value) { SNode* node = (SNode*)std::malloc(sizeof(SNode)); new (&node->m_value) SPair(value); return node; } };

int main() {
    CTree t; SPair v; v.m_first = 7; v.m_second = 9; SNode* n = t.BuyNode(v); if (!n || n->m_value.m_first != 7 || n->m_value.m_second != 9) { std::printf("0067b0b0_TEST FAIL\n"); return 1; } std::free(n);
    std::printf("0067b0b0_TEST PASS\n"); return 0;
}
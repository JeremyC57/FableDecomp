#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

static inline unsigned char* CopyBytes(const unsigned char* first, const unsigned char* last, unsigned char* result) { if (first == last) return result; unsigned int n = last - first; return (unsigned char*)std::memmove(result, first, n) + n; }
struct CRetailByteVector { unsigned char* m_pStart; unsigned char* m_pFinish; unsigned char* m_pEndOfStorage; void Erase(unsigned char* f, unsigned char* l) { m_pFinish = CopyBytes(l, m_pFinish, f); } };
static int g_erased = 0;
struct CRetailTree { struct node { long c; node* m_pParent; node* m_pLeft; node* m_pRight; }; node* m_pHeader; unsigned long m_nodeCount; void EraseSubtree(node* r) { if (r) g_erased++; }
    void Clear() { if (m_nodeCount != 0) { EraseSubtree(m_pHeader->m_pParent); m_pHeader->m_pLeft = m_pHeader; m_pHeader->m_pParent = 0; m_pHeader->m_pRight = m_pHeader; m_nodeCount = 0; } } };
struct CPair { CRetailByteVector m_bytes; CRetailTree m_tree; void Clear() { m_bytes.Erase(m_bytes.m_pStart, m_bytes.m_pFinish); CRetailTree* tree = &m_tree; tree->Clear(); } };
int main() { unsigned char buf[8] = {1,2,3,4,5,6,7,8}; CRetailTree::node hdr, root; hdr.m_pParent = &root; hdr.m_pLeft = &root; hdr.m_pRight = &root;
    CPair p; p.m_bytes.m_pStart = buf; p.m_bytes.m_pFinish = buf + 5; p.m_bytes.m_pEndOfStorage = buf + 8; p.m_tree.m_pHeader = &hdr; p.m_tree.m_nodeCount = 1; p.Clear();
    if (p.m_bytes.m_pFinish != buf || p.m_bytes.m_pStart != buf || g_erased != 1 || hdr.m_pParent || hdr.m_pLeft != &hdr || hdr.m_pRight != &hdr || p.m_tree.m_nodeCount) { std::printf("00ad90c0_TEST FAIL\n"); return 1; }
    p.Clear(); if (g_erased != 1 || p.m_bytes.m_pFinish != buf) { std::printf("00ad90c0_TEST FAIL\n"); return 1; }
    std::printf("00ad90c0_TEST PASS\n"); return 0; }
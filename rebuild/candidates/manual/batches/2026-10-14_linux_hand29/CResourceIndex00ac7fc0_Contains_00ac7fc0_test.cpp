#include <cstdio>
struct CRetailTreeIterator { void* m_pNode; CRetailTreeIterator(void* n) : m_pNode(n) { } CRetailTreeIterator(const CRetailTreeIterator& o) : m_pNode(o.m_pNode) { } };
static void* g_hit = 0;
struct CRetailTree { void* m_pHead; unsigned long m_size; CRetailTreeIterator FindGreaterEqual(const unsigned long& key) const { return CRetailTreeIterator(key < 10 ? g_hit : m_pHead); } void* End() const { return m_pHead; } };
struct CResourceIndex { unsigned char _pad[0xa8]; CRetailTree m_tree; bool Contains(unsigned long key) const { CRetailTreeIterator f = m_tree.FindGreaterEqual(key); return f.m_pNode != m_tree.End(); } };
int main() { int head, hit; g_hit = &hit; CResourceIndex idx; idx.m_tree.m_pHead = &head; unsigned long k = 3, k2 = 50;
    if (!idx.Contains(k) || idx.Contains(k2)) { std::printf("00ac7fc0_TEST FAIL\n"); return 1; } std::printf("00ac7fc0_TEST PASS\n"); return 0; }

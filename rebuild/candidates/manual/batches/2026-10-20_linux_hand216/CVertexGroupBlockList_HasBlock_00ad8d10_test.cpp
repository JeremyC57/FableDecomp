// Behaviour test for 0x00AD8D10 CVertexGroupBlockList::HasBlock: a block matches when the group count is equal and
// every group is in the candidate's map; tree built by hand.
#include <cstdio>
struct SGroupMapNode { long Color; SGroupMapNode* Parent; SGroupMapNode* Left; SGroupMapNode* Right; unsigned long Key; long Value; };
struct CGroupToIndexMap {
    SGroupMapNode* Header; unsigned long NodeCount; unsigned long Compare;
    SGroupMapNode* find(unsigned long key) const
    {
        SGroupMapNode* y = Header; SGroupMapNode* x = Header->Parent;
        while (x != 0) { if (!(x->Key < key)) { y = x; x = x->Left; } else x = x->Right; }
        if (y == Header || key < y->Key) y = Header;
        return y;
    }
};
struct CGroupArray {
    unsigned long* m_first; unsigned long* m_last; unsigned long* m_end;
    unsigned long* begin() const { return m_first; }
    unsigned long size() const { return (unsigned long)(m_last - m_first); }
};
struct CVertexGroupBlock {
    CGroupArray Groups; CGroupToIndexMap GroupToIndexMap;
    bool ContainsAll(const unsigned long* groups, unsigned long count) const
    {
        for (unsigned long i = 0; i < count; ++i, ++groups)
            if (GroupToIndexMap.find(*groups) == GroupToIndexMap.Header) return false;
        return true;
    }
};
struct CVertexGroupBlockList {
    CVertexGroupBlock* m_first; CVertexGroupBlock* m_last;
    bool HasBlock(const CVertexGroupBlock& block) const
    {
        unsigned long count = block.Groups.size();
        const unsigned long* groups = block.Groups.begin();
        for (const CVertexGroupBlock* cur = m_last; cur != m_first; --cur)
            if (count == (cur - 1)->Groups.size() && (cur - 1)->ContainsAll(groups, count)) return true;
        return false;
    }
};
// build a map holding keys k[0..n) (n <= 3) as a small balanced tree
static void BuildMap(CGroupToIndexMap& m, SGroupMapNode* header, SGroupMapNode* nodes, const unsigned long* k, int n)
{
    m.Header = header; header->Parent = 0; header->Left = header->Right = header;
    for (int i = 0; i < n; ++i) { nodes[i].Key = k[i]; nodes[i].Left = nodes[i].Right = 0; nodes[i].Value = i; }
    if (n == 1) header->Parent = &nodes[0];
    if (n == 2) { header->Parent = &nodes[0]; nodes[0].Right = &nodes[1]; }
    if (n == 3) { header->Parent = &nodes[1]; nodes[1].Left = &nodes[0]; nodes[1].Right = &nodes[2]; }
}
int main()
{
    unsigned long a[3] = { 2, 5, 9 }, b[2] = { 3, 7 };
    SGroupMapNode ha, hb, na[3], nb[2];
    CVertexGroupBlock blocks[2];
    blocks[0].Groups.m_first = a; blocks[0].Groups.m_last = a + 3; BuildMap(blocks[0].GroupToIndexMap, &ha, na, a, 3);
    blocks[1].Groups.m_first = b; blocks[1].Groups.m_last = b + 2; BuildMap(blocks[1].GroupToIndexMap, &hb, nb, b, 2);
    CVertexGroupBlockList list; list.m_first = blocks; list.m_last = blocks + 2;
    unsigned long q1[3] = { 9, 2, 5 }, q2[2] = { 7, 3 }, q3[2] = { 7, 4 }, q4[2] = { 2, 5 };
    CVertexGroupBlock probe;
    bool ok = true;
    probe.Groups.m_first = q1; probe.Groups.m_last = q1 + 3; ok = ok && list.HasBlock(probe);
    probe.Groups.m_first = q2; probe.Groups.m_last = q2 + 2; ok = ok && list.HasBlock(probe);
    probe.Groups.m_first = q3; probe.Groups.m_last = q3 + 2; ok = ok && !list.HasBlock(probe);
    probe.Groups.m_first = q4; probe.Groups.m_last = q4 + 2; ok = ok && !list.HasBlock(probe);   // subset, wrong count
    list.m_last = blocks; probe.Groups.m_first = q1; probe.Groups.m_last = q1 + 3; ok = ok && !list.HasBlock(probe);
    if (!ok) { std::printf("00ad8d10_TEST FAIL\n"); return 1; }
    std::printf("00ad8d10_TEST PASS\n");
    return 0;
}

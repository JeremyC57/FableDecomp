// 00ad9470 behaviour test: CVertexGroupBlock::RemoveLast (final.cpp body over standard containers)
#include <stdio.h>
#include <map>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
typedef std::multimap<void*, int> CVertexGroupTree;
class CVertexGroupBlock { public:
    std::vector<void*> Entries;
    CVertexGroupTree m_groups;
    void*& back() { return Entries.back(); }
    void RemoveLast(unsigned long count);
};
void CVertexGroupBlock::RemoveLast(unsigned long count)
{
    while (count > 0) {
        std::pair<CVertexGroupTree::iterator, CVertexGroupTree::iterator> range = m_groups.equal_range(back());
        m_groups.erase(range.first, range.second);
        Entries.pop_back();
        --count;
    }
}
int main()
{
    int a, b, c;
    CVertexGroupBlock blk;
    blk.Entries.push_back(&a); blk.Entries.push_back(&b); blk.Entries.push_back(&c);
    blk.m_groups.insert(std::make_pair((void*)&a, 1));
    blk.m_groups.insert(std::make_pair((void*)&b, 2));
    blk.m_groups.insert(std::make_pair((void*)&c, 3));
    blk.m_groups.insert(std::make_pair((void*)&c, 4));
    blk.RemoveLast(0);
    CHECK(blk.Entries.size() == 3 && blk.m_groups.size() == 4);
    blk.RemoveLast(2);
    CHECK(blk.Entries.size() == 1 && blk.Entries[0] == &a);
    CHECK(blk.m_groups.size() == 1 && blk.m_groups.begin()->second == 1);
    blk.RemoveLast(1);
    CHECK(blk.Entries.empty() && blk.m_groups.empty());
    if (!g_fail) printf("00ad9470_TEST PASS\n");
    return g_fail;
}
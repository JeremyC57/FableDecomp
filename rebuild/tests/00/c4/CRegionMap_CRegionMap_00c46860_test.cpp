// Behaviour test for 00c46860: CRegionMap::~CRegionMap -- pop and delete every region node
#include <cstdio>

static int g_deleted = 0;
struct SRegionNode { int m_visited, m_x, m_y; float m_length; SRegionNode* m_next; ~SRegionNode() { ++g_deleted; } };
struct CList {
    SRegionNode* m_head;
    SRegionNode* GetFirst() { return m_head; }
    void Remove(SRegionNode* n) { if (m_head == n) m_head = n->m_next; }
    SRegionNode* PopFirst()
    {
        SRegionNode* item = GetFirst();
        if (item)
            Remove(item);
        return item;
    }
};
struct CRegionMap {
    CList m_regions;
    void Destroy()
    {
        SRegionNode* node;
        while ((node = m_regions.PopFirst()) != 0)
            delete node;
    }
};

int main()
{
    CRegionMap map; map.m_regions.m_head = 0;
    for (int i = 0; i < 4; ++i) { SRegionNode* n = new SRegionNode; n->m_next = map.m_regions.m_head; map.m_regions.m_head = n; }
    map.Destroy();
    int ok = g_deleted == 4 && map.m_regions.m_head == 0;
    map.Destroy();
    ok &= g_deleted == 4;
    if (!ok) { std::printf("00c46860_TEST FAIL\n"); return 1; }
    std::printf("00c46860_TEST PASS\n");
    return 0;
}
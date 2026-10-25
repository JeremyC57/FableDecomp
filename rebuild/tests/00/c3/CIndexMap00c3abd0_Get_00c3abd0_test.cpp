// Behaviour test for 00c3abd0: CIndexMap::Get -- the value stored under an index, or 0 when the index is absent
#include <cstdio>

struct SIndexNode {
    void* m_pValue;
    unsigned long m_index;
};

class CIndexMap {
public:
    SIndexNode m_nodes[2];
    SIndexNode* Lookup(const unsigned long* index)
    {
        for (int i = 0; i < 2; ++i)
            if (m_nodes[i].m_index == *index) return &m_nodes[i];
        return 0;
    }
    void* Get(unsigned long index)
    {
        SIndexNode* node = Lookup(&index);
        if (!node)
            return 0;
        return node->m_pValue;
    }
};

int main()
{
    int a, b;
    CIndexMap map;
    map.m_nodes[0].m_pValue = &a; map.m_nodes[0].m_index = 3;
    map.m_nodes[1].m_pValue = &b; map.m_nodes[1].m_index = 9;
    if (map.Get(3) != &a || map.Get(9) != &b || map.Get(4) != 0) { std::printf("00c3abd0_TEST FAIL\n"); return 1; }
    std::printf("00c3abd0_TEST PASS\n");
    return 0;
}
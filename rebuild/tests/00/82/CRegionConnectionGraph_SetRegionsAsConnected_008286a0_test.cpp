// Behaviour test: CRegionConnectionGraph::SetRegionsAsConnected -- both regions' graph nodes are told about each other; a region missing from the graph is skipped
#include <cstdio>

class CRegionConnectionNode { public:
    long lastRegion;
    bool lastConnected;
    int  calls;
    void SetAsConnectedToRegion(long region, bool connected) { lastRegion = region; lastConnected = connected; ++calls; }
};
struct SEntry {                                         // pair<long, CCountedPointer<CRegionConnectionNode> >
    long first;
    CRegionConnectionNode* second;
};
struct CRegionNodeMap {
    SEntry* m_first;
    SEntry* m_last;
    SEntry* LowerBound(const long& key)                 // stand-in for 0x00827B00 (sorted vector lower bound)
    {
        SEntry* it = m_first;
        while (it != m_last && it->first < key)
            ++it;
        return it;
    }
    SEntry* Find(const long& key)
    {
        SEntry* it = LowerBound(key);
        return (it == m_last || key < it->first) ? m_last : it;
    }
};
class CRegionConnectionGraph { public:
    CRegionNodeMap GraphNodes;
    void SetRegionsAsConnected(long region1, long region2, bool connected)
    {
        SEntry* it = GraphNodes.Find(region1);
        if (it != GraphNodes.m_last)
            it->second->SetAsConnectedToRegion(region2, connected);
        it = GraphNodes.Find(region2);
        if (it != GraphNodes.m_last)
            it->second->SetAsConnectedToRegion(region1, connected);
    }
};

int main()
{
    CRegionConnectionNode nodes[3];
    for (int i = 0; i < 3; ++i) {
        nodes[i].lastRegion = -1;
        nodes[i].lastConnected = false;
        nodes[i].calls = 0;
    }
    SEntry entries[3] = { { 2, &nodes[0] }, { 5, &nodes[1] }, { 9, &nodes[2] } };
    CRegionConnectionGraph g;
    g.GraphNodes.m_first = entries;
    g.GraphNodes.m_last = entries + 3;
    g.SetRegionsAsConnected(2, 9, true);
    if (nodes[0].lastRegion != 9 || !nodes[0].lastConnected || nodes[2].lastRegion != 2 || nodes[1].calls != 0)
        { std::printf("008286a0_TEST FAIL\n"); return 1; }
    g.SetRegionsAsConnected(5, 7, false);                // 7 is not in the graph: only node 5 hears about it
    if (nodes[1].lastRegion != 7 || nodes[1].lastConnected || nodes[1].calls != 1 || nodes[0].calls != 1 || nodes[2].calls != 1)
        { std::printf("008286a0_TEST FAIL\n"); return 1; }
    std::printf("008286a0_TEST PASS\n");
    return 0;
}
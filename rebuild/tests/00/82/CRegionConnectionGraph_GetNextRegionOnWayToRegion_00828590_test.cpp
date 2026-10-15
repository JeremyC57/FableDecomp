// Behaviour test for CRegionConnectionGraph::GetNextRegionOnWayToRegion @ 0x00828590.
#include <cstdio>

struct CRegionConnectionGraph;
struct CRegionConnectionNode {
    long m_next; const CRegionConnectionGraph* m_lastGraph;
    long GetNextRegionOnWayToRegion(long toRegion, CRegionConnectionGraph* graph) { m_lastGraph = graph; return m_next * 100 + toRegion; }
};
template<class T> struct CCountedPointer { T* Object; void* PBlock; T* operator->() const { return Object; } };
struct SRegionNodeEntry { long first; CCountedPointer<CRegionConnectionNode> second; };
struct CRegionNodeMap {
    SRegionNodeEntry* m_first; SRegionNodeEntry* m_last; SRegionNodeEntry* m_end;
    SRegionNodeEntry* LowerBound(const long& key) { SRegionNodeEntry* p = m_first; while (p != m_last && p->first < key) ++p; return p; }
    SRegionNodeEntry* Find(const long& key) { SRegionNodeEntry* it = LowerBound(key); return (it == m_last || key < it->first) ? m_last : it; }
};
struct CRegionConnectionGraph {
    CRegionNodeMap GraphNodes;
    long GetNextRegionOnWayToRegion(long fromRegion, long toRegion)
    {
        SRegionNodeEntry* it = GraphNodes.Find(fromRegion);
        if (it != GraphNodes.m_last)
            return it->second->GetNextRegionOnWayToRegion(toRegion, this);
        return -1;
    }
};

int main()
{
    CRegionConnectionNode n2 = { 7, 0 }, n5 = { 9, 0 };
    SRegionNodeEntry entries[2];
    entries[0].first = 2; entries[0].second.Object = &n2; entries[0].second.PBlock = 0;
    entries[1].first = 5; entries[1].second.Object = &n5; entries[1].second.PBlock = 0;
    CRegionConnectionGraph g; g.GraphNodes.m_first = entries; g.GraphNodes.m_last = entries + 2; g.GraphNodes.m_end = entries + 2;
    if (g.GetNextRegionOnWayToRegion(5, 3) != 903 || n5.m_lastGraph != &g) { std::printf("00828590_TEST FAIL\n"); return 1; }
    if (g.GetNextRegionOnWayToRegion(2, 1) != 701) { std::printf("00828590_TEST FAIL\n"); return 1; }
    if (g.GetNextRegionOnWayToRegion(4, 1) != -1 || g.GetNextRegionOnWayToRegion(8, 1) != -1 || g.GetNextRegionOnWayToRegion(1, 1) != -1) {
        std::printf("00828590_TEST FAIL\n"); return 1;
    }
    std::printf("00828590_TEST PASS\n");
    return 0;
}
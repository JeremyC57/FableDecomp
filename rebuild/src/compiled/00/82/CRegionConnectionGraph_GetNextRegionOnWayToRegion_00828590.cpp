// CRegionConnectionGraph::GetNextRegionOnWayToRegion(long fromRegion, long toRegion) -- find the graph node of the start region and ask it for the next region towards the target (-1 when the start region is not in the graph)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00828590, length 61 bytes (`ret 8`). Called from 0x004FB4D7.
// Callees: CVectorMap<long, CCountedPointer<CRegionConnectionNode> >::LowerBound(const long&) @ 0x00827B60 (returns the
// element pointer in eax), the node's next-region query @ 0x00828360 (thiscall on the node with the target and the graph;
// manifest "Navigation_SortRegionConnectionTree" is a description).
// Layout: Ego_r.pdb CRegionConnectionGraph: GraphNodes (CVectorMap) at +0x00 -- the vector's first/last at +0x00/+0x04;
// each element is {long key, CCountedPointer<CRegionConnectionNode>} with the node object at +4.
// Shape: the map's inline `find` -- LowerBound on the key's own argument slot (`lea eax,[esp+8]`), then
// `(it == end || key < it->first) ? end : it` (signed: `jge`) -- and the caller's test on the same end register, written
// `if (it != end) return ...; return -1;` so the found path comes first; the node is reached through the counted pointer's
// inline `operator->` (`mov eax,[eax+4] ; ... mov ecx,eax`; reading `.Object` directly loads straight into ecx).
// Leaf: GetNextRegionOnWayToRegion
class CRegionConnectionGraph;

class CRegionConnectionNode { public:
    long GetNextRegionOnWayToRegion(long toRegion, CRegionConnectionGraph* graph);   // 0x00828360
};

template<class T> class CCountedPointer { public:
    T*    Object;                                       // +0x00
    void* PBlock;                                       // +0x04
    T* operator->() const { return Object; }
};

struct SRegionNodeEntry {                               // pair<long, CCountedPointer<CRegionConnectionNode> >
    long                                    first;      // +0x00
    CCountedPointer<CRegionConnectionNode>  second;     // +0x04
};

struct CRegionNodeMap {                                 // CVectorMap<long, CCountedPointer<CRegionConnectionNode> >
    SRegionNodeEntry* m_first;                          // +0x00
    SRegionNodeEntry* m_last;                           // +0x04
    SRegionNodeEntry* m_end;                            // +0x08
    SRegionNodeEntry* LowerBound(const long& key);      // 0x00827B60
    SRegionNodeEntry* Find(const long& key)
    {
        SRegionNodeEntry* it = LowerBound(key);
        return (it == m_last || key < it->first) ? m_last : it;
    }
};

class CRegionConnectionGraph { public:
    CRegionNodeMap GraphNodes;                          // +0x00
    long GetNextRegionOnWayToRegion(long fromRegion, long toRegion);
};

long CRegionConnectionGraph::GetNextRegionOnWayToRegion(long fromRegion, long toRegion)
{
    SRegionNodeEntry* it = GraphNodes.Find(fromRegion);
    if (it != GraphNodes.m_last)
        return it->second->GetNextRegionOnWayToRegion(toRegion, this);
    return -1;
}
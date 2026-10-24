// CRegionConnectionGraph::SetRegionsAsConnected(long region1, long region2, bool connected) -- mark two regions as (dis)connected in both directions: each one's graph node adds or drops the other
// Leaf: ?SetRegionsAsConnected@CRegionConnectionGraph
// Retail: Fable.exe (TLC, VC7.1) @ 0x008286A0, 101 bytes (+11 int3), thiscall `ret 0Ch`.
// Identity: Ego_r.pdb name set ?SetRegionsAsConnected@CRegionConnectionGraph@@QAEXJJ_N@Z; it calls
//   CRegionConnectionNode::SetAsConnectedToRegion (QAEXJ_N) @ 0x008285D0 twice. The catalog label
//   "Navigation_ConnectRegionsInGraph" is descriptive.
// Callees: CVectorMap<long, CCountedPointer<CRegionConnectionNode> >::LowerBound(const long&) @ 0x00827B00 (the non-const
//   twin of the 0x00827B60 the landed GetNextRegionOnWayToRegion 0x00828590 calls), SetAsConnectedToRegion @ 0x008285D0.
// Layout: Ego_r.pdb CRegionConnectionGraph: GraphNodes (CVectorMap) at +0 -- vector first/last at +0/+4; each element
//   {long key, CCountedPointer<CRegionConnectionNode>} with the node object at +4.
// Shape: two inline map finds -- LowerBound on the key's own argument slot, then `(it == end || key < it->first) ? end
//   : it` (signed `jge`) -- each followed by `if (it != end) it->second->SetAsConnectedToRegion(other, connected);`.
//   The region ids and the flag live in ebp/edi/ebx only with a same-TU noinline LowerBound body (below).
struct CRegionConnectionNode {
    void SetAsConnectedToRegion(long region, bool connected);   // 0x008285D0
};

template<class T> struct CCountedPointer {
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
    __declspec(noinline) SRegionNodeEntry* LowerBound(const long& key);   // 0x00827B00
    SRegionNodeEntry* Find(const long& key)
    {
        SRegionNodeEntry* it = LowerBound(key);
        return (it == m_last || key < it->first) ? m_last : it;
    }
};

// Same-TU stand-in for the map's LowerBound 0x00827B00 (only the target row is graded): with the body in the TU VC7.1
// sees that the key is only read, so it keeps both region ids in registers across the calls (retail ebp / edi).
SRegionNodeEntry* CRegionNodeMap::LowerBound(const long& key)
{
    SRegionNodeEntry* first = m_first;
    int len = m_last - m_first;
    while (len > 0) {
        int half = len >> 1;
        SRegionNodeEntry* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

struct CRegionConnectionGraph {
    CRegionNodeMap GraphNodes;                          // +0x00
    void SetRegionsAsConnected(long region1, long region2, bool connected);
};

void CRegionConnectionGraph::SetRegionsAsConnected(long region1, long region2, bool connected)
{
    SRegionNodeEntry* it = GraphNodes.Find(region1);
    if (it != GraphNodes.m_last)
        it->second->SetAsConnectedToRegion(region2, connected);
    it = GraphNodes.Find(region2);
    if (it != GraphNodes.m_last)
        it->second->SetAsConnectedToRegion(region1, connected);
}
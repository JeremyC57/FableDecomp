// CNavQuadTree::GetMemoryUsage(long& nodeBytes, long& lineBytes, long& switchableBytes, long& otherBytes) -- zero the four tallies, size the region and line lists, sum the usage of every layer-0 node, then add layer 1's nodes through local tallies; returns the total
// Leaf: ?GetMemoryUsage@CNavQuadTree
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77E80, length 408 bytes, `ret 10h`.
// Callees: CANavQuadTreeNode virtual slot 15 (+0x3c) GetMemoryUsage(long&, long&, long&, long&) only.
// Layout (retail; Ego_r.pdb CNavQuadTree with 12-byte STLport vectors and 4-byte STLport lists): NavigationLayers +0x1c
//   (0x28-byte CNavigationLayer: Nodes +0x00, LineLists +0x0c, SwitchableLineLists +0x18), Regions list +0x74.
//   engine/CNavQuadTree.h is the PDB view (16-byte vectors), so the class is modelled locally.
// Shape: the region and line-list counts are STLport list::size() calls whose results are unused -- VC7.1 keeps the
//   empty node walks (`mov eax,[eax] ; cmp eax,ecx ; jne`) and drops the counters; the layer count is recomputed
//   (`imul 66666667h ; sar 4`) on every outer iteration; the line lists are reached through a layer reference (a named
//   vector reference strength-reduces to `_M_start` instead of retail's `_M_finish`-based pointer); layer 1's tallies are four zeroed locals added back at the end.
template<class T> struct CListNode {
    CListNode* _M_next;
    CListNode* _M_prev;
    T          _M_data;
};
template<class T> struct CRetailList {               // STLport list<T>: the sentinel node pointer only
    CListNode<T>* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (CListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
template<class T> struct CRetailVector {             // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
struct C2DLineF { float X0, Y0, X1, Y1; };
struct CNavRegion { unsigned char _body[0x10]; };

struct CANavQuadTreeNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual long GetMemoryUsage(long& nodeBytes, long& lineBytes, long& switchableBytes, long& otherBytes);   // slot 15
};

struct CNavQuadTree {
    struct CNavigationLayer {                                        // 0x28 bytes
        CRetailVector<CANavQuadTreeNode*>        Nodes;              // +0x00
        CRetailVector<CRetailList<C2DLineF> >    LineLists;          // +0x0c
        unsigned char                            SwitchableLineLists[0x10];   // +0x18 CVectorMap
    };
    void*                             __vftable;                     // +0x00
    unsigned char                     _pad04[0x1c - 0x04];           // flags, MapPos, MapWidth/Height, MapIndex
    CRetailVector<CNavigationLayer>   NavigationLayers;              // +0x1c
    unsigned char                     _pad28[0x74 - 0x28];           // TransitionPoints .. RegionSeeds
    CRetailList<CNavRegion>           Regions;                       // +0x74
    long GetMemoryUsage(long& nodeBytes, long& lineBytes, long& switchableBytes, long& otherBytes);
};

long CNavQuadTree::GetMemoryUsage(long& nodeBytes, long& lineBytes, long& switchableBytes, long& otherBytes)
{
    nodeBytes = 0;
    lineBytes = 0;
    switchableBytes = 0;
    otherBytes = 0;
    Regions.size();
    for (unsigned long i = 0; i < NavigationLayers.size(); ++i) {
        CNavigationLayer& layer = NavigationLayers[i];
        for (CRetailList<C2DLineF>* it = layer.LineLists._M_start; it != layer.LineLists._M_finish; ++it)
            it->size();
    }
    long total = 0;
    for (CANavQuadTreeNode** it = NavigationLayers[0].Nodes._M_start; it != NavigationLayers[0].Nodes._M_finish; ++it)
        total += (*it)->GetMemoryUsage(nodeBytes, lineBytes, switchableBytes, otherBytes);
    if (NavigationLayers.size() > 1) {
        long layerNodeBytes = 0;
        long layerLineBytes = 0;
        long layerSwitchableBytes = 0;
        long layerOtherBytes = 0;
        long layerTotal = 0;
        for (CANavQuadTreeNode** it = NavigationLayers[1].Nodes._M_start; it != NavigationLayers[1].Nodes._M_finish; ++it)
            layerTotal += (*it)->GetMemoryUsage(layerNodeBytes, layerLineBytes, layerSwitchableBytes, layerOtherBytes);
        nodeBytes += layerNodeBytes;
        lineBytes += layerLineBytes;
        switchableBytes += layerSwitchableBytes;
        otherBytes += layerOtherBytes;
        total += layerTotal;
    }
    return total;
}

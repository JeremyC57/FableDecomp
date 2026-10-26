// CNavNavigableLeafNode::CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, list<CNavRegion>& regions, bool readPreferability) -- load a navigable leaf node: the quad-tree node base (leaf, not blocked), its region (1-based index into the region list, 0 = none), the preferability cost (or the default 0x40) and the neighbour indices
// Leaf: ??0CNavNavigableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A772D0, 258 bytes (+ int3 padding), `ret 18h`.
// Callees: CBaseClassNonCopyable() 0x0099A2F0 (the root base, out of line), CAFile slot 3 Read(void*, size, flags) (+0x0c),
//   CSmallVector<const CANavQuadTreeNode*, 8>::resize(unsigned long, const T&) 0x00A7DB10.
// Vtables: 0x0129CBBC CANavQuadTreeNode (inline base ctor), 0x0129DDA4 CNavNavigableLeafNode.
// Layout: engine/CNavNavigableLeafNode.h (Ego_r.pdb, retail equal): PRegion +0x10, Neighbours +0x14 (data pointer, signed
//   byte count at +0x18), PreferabilityCost +0x1c; CANavQuadTreeNode Position +4, QuadTreeLevel +0xc, NavLayer +0xd,
//   LeafNode +0xe, Blocked +0xf. Modelled locally (the header is a flat pad record).
// Shape: std::advance over the bidirectional region iterator; the resize fill value and each neighbour index are homed in
//   the dead `level` argument slot.
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
class CAFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Read(void* data, unsigned long size, long flags);   // slot 3 (+0x0c)
};
struct CBaseClassNonCopyable {                     // vtable 0x0129A7C4
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};
class CANavQuadTreeNode : public CBaseClassNonCopyable {   // vtable 0x0129CBBC
public:
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    virtual void v00();
    CANavQuadTreeNode(const C2DVector& position, unsigned char level, unsigned char layer, bool leaf, bool blocked)
        : Position(position), QuadTreeLevel(level), NavLayer(layer), LeafNode(leaf), Blocked(blocked) { }
};
template <class T, int N> class CSmallVector { public:
    T*          Data;                              // +0
    signed char Count;                             // +4
    CSmallVector() : Data(0), Count(0) { }
    void resize(unsigned long size, const T& value);   // 0x00A7DB10
};
class CNavRegion { public: long RegionID; void* Connections; long MostRecentSearchID; };
struct SRegionNode {                               // _List_node<CNavRegion>
    SRegionNode* _M_next;
    SRegionNode* _M_prev;
    CNavRegion   _M_data;
};
struct CRegionIterator {
    SRegionNode* _M_node;
    CRegionIterator(SRegionNode* n) : _M_node(n) { }
    CNavRegion& operator*() const { return _M_node->_M_data; }
    CRegionIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    CRegionIterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};
inline void advance(CRegionIterator& it, long n)   // SGI __advance (bidirectional)
{
    if (n > 0)
        while (n--)
            ++it;
    else
        while (n++)
            --it;
}
struct CRegionList {                               // STLport list<CNavRegion> (retail: the sentinel pointer)
    SRegionNode* _M_node;
    CRegionIterator begin() const { return CRegionIterator(_M_node->_M_next); }
};

class CNavNavigableLeafNode : public CANavQuadTreeNode {   // vtable 0x0129DDA4
public:
    CNavRegion*                                  PRegion;             // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8>    Neighbours;          // +0x14
    unsigned char                                PreferabilityCost;   // +0x1c
    virtual void v00();
    CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, CRegionList& regions, bool readPreferability);
};

CNavNavigableLeafNode::CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, CRegionList& regions, bool readPreferability)
    : CANavQuadTreeNode(position, level, layer, true, false), PRegion(0)
{
    long regionIndex;
    file.Read(&regionIndex, 4, 0);
    if (regionIndex > 0) {
        CRegionIterator it = regions.begin();
        advance(it, regionIndex - 1);
        PRegion = &*it;
    }
    if (readPreferability)
        file.Read(&PreferabilityCost, 1, 0);
    else
        PreferabilityCost = 0x40;
    unsigned long numNeighbours;
    file.Read(&numNeighbours, 4, 0);
    Neighbours.resize(numNeighbours, 0);
    for (unsigned long i = 0; i < numNeighbours; ++i) {
        long index;
        file.Read(&index, 4, 0);
        Neighbours.Data[i] = (const CANavQuadTreeNode*)index;
    }
}
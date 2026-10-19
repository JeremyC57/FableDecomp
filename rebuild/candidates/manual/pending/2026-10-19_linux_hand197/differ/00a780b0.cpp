// CNavConnectorNode::~CNavConnectorNode() -- unlink the two leaf nodes: each drops the other from its neighbour list (swap-with-last then shrink) and their regions are disconnected
// Leaf: ??1CNavConnectorNode
// Grade: DIFFER(236v238) -- register allocation only: retail keeps Neighbour1 in ebx and its data in ebp (end built as mov edx,ebp ; lea edx,[edx+ecx*4]) and pushes the shared tag first; ours swaps ebx/ebp (tried member begin/end, cached data local, uninitialised shared tag local, end += size)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A780B0, 238 bytes (+ int3 padding). __thiscall, `ret` (no vtable: a plain dtor).
// Callees: the Dinkumware find over 4-byte elements Find_00a7d050 @ 0x00A7D050 (__fastcall first/last, value by reference, an
//   empty iterator-category tag), CSmallVector<const CANavQuadTreeNode*, 8>::resize(long, const T&) @ 0x00A7DB10, the leaf
//   node's virtual slot 7 (+0x1c, region getter), CNavRegion::RemoveConnectionToRegion(CNavRegion*) @ 0x00A78060.
// Layout = Ego_r.pdb: CNavConnectorNode {Neighbour1 +0, Neighbour2 +4}; CNavNavigableLeafNode {PRegion +0x10, Neighbours +0x14
//   (CSmallVector: data +0, signed char size +4)}; the model is the landed ctor's (0x00A78A90).
// Shape: the mirror of the ctor's push_back pair: both finds run first (the first result is spilled to the frame), then each
//   hit that is not already last is std::swap'ed with the last element (the temporary reuses the spilled slot), then each
//   list is resized to size-1 with the default T() fill homed in the dead argument slot; finally the first region is a named
//   local disconnected from the second's.
struct random_access_iterator_tag { };
long* __fastcall Find_00a7d050(long* first, long* last, const long& value, const random_access_iterator_tag& tag);   // 0x00A7D050
struct CNavRegion {
    void RemoveConnectionToRegion(CNavRegion* other);      // 0x00A78060
};
struct CANavQuadTreeNode;
template <class T, int N> struct CSmallVector {
    T*          m_data;                                    // +0
    signed char m_size;                                    // +4
    signed char m_capacity;                                // +5
    void resize(long size, const T& fill = T());           // 0x00A7DB10
    T* begin() { return m_data; }
    T* end() { return m_data + m_size; }
    void erase_unordered(T* it)                            // move the hit to the back (std::swap); the caller shrinks
    {
        if (it - m_data < m_size - 1) {
            T tmp = *it;
            *it = m_data[m_size - 1];
            m_data[m_size - 1] = tmp;
        }
    }
    void pop_back() { resize(m_size - 1); }
};
struct C2DVector { float X, Y; };
struct CANavQuadTreeNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual CNavRegion* GetRegion() const;                 // slot 7 (+0x1c)
    C2DVector     Position;                                // +0x04
    unsigned char QuadTreeLevel;                           // +0x0c
    unsigned char NavLayer;                                // +0x0d
    bool          LeafNode;                                // +0x0e
    bool          Blocked;                                 // +0x0f
};
typedef CSmallVector<const CANavQuadTreeNode*, 8> CNeighbourList;
struct CNavNavigableLeafNode : CANavQuadTreeNode {
    CNavRegion*    PRegion;                                // +0x10
    CNeighbourList Neighbours;                             // +0x14
    unsigned char  PreferabilityCost;                      // +0x1c
};
struct CNavConnectorNode {
    CNavNavigableLeafNode* Neighbour1;                     // +0
    CNavNavigableLeafNode* Neighbour2;                     // +4
    ~CNavConnectorNode();
};

template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
static inline const CANavQuadTreeNode** FindNeighbour(const CANavQuadTreeNode** first, const CANavQuadTreeNode** last, CNavNavigableLeafNode* const& node, const random_access_iterator_tag& tag)
{
    return (const CANavQuadTreeNode**)Find_00a7d050((long*)first, (long*)last, (const long&)node, tag);
}

CNavConnectorNode::~CNavConnectorNode()
{
    random_access_iterator_tag tag;
    CNavNavigableLeafNode* node1 = Neighbour1;
    const CANavQuadTreeNode** data1 = node1->Neighbours.m_data;
    const CANavQuadTreeNode** it1 = FindNeighbour(data1, data1 + node1->Neighbours.m_size, Neighbour2, tag);
    const CANavQuadTreeNode** it2 = FindNeighbour(Neighbour2->Neighbours.m_data, Neighbour2->Neighbours.m_data + Neighbour2->Neighbours.m_size, Neighbour1, tag);
    long last1 = node1->Neighbours.m_size;
    if (it1 - data1 < last1 - 1)
        swap(*it1, data1[last1 - 1]);
    if (it2 - Neighbour2->Neighbours.m_data < Neighbour2->Neighbours.m_size - 1)
        swap(*it2, Neighbour2->Neighbours.m_data[Neighbour2->Neighbours.m_size - 1]);
    Neighbour1->Neighbours.pop_back();
    Neighbour2->Neighbours.pop_back();
    CNavRegion* region = Neighbour1->GetRegion();
    region->RemoveConnectionToRegion(Neighbour2->GetRegion());
}

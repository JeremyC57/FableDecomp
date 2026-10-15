// CNavNavigableLeafNode::MakeRegion(const CNavRegion*, list<CANavQuadTreeNode*>&) -- claim this leaf for the region and queue every neighbour that has no region yet
// Grade: DIFFER(122v125) -- body identical; ours pushes ebp/esi/edi lazily (ebp after the region store, esi/edi inside the loop guard) where retail pushes all three at entry and keeps the early `xor al,al` exit at the tail
// Leaf: ?MakeRegion@CNavNavigableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77B00, 123 bytes (+ int3 padding; `ret 8`). Vtable 0x0129DDA4 slot 12.
// Callees: virtual slot 5 GetRegionID (+0x14) on this and on each neighbour, operator new @ 0x00BFEA0E (12-byte STLport list node).
// Shape: STLport `list::push_back` inlined: `end()` read before the node allocation, the value placement-constructed behind a null
//   test of its address, then the four link stores (next, prev, prev->next, head->prev).
struct CNavRegion;
struct CAFile;
struct CANavQuadTreeNode;
void* __cdecl operator new(unsigned int size);             // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace std {
template <class T> struct allocator { };
template <class T> struct _List_node {                    // STLport list node: next, prev, data
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T, class A = allocator<T> > struct list { // STLport list: the head node pointer only
    _List_node<T>* _M_node;
    struct iterator {
        _List_node<T>* _M_node;
        iterator(_List_node<T>* node) : _M_node(node) { }
    };
    iterator end() { return iterator(_M_node); }
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* p = (_List_node<T>*)operator new(sizeof(_List_node<T>));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node<T>* n = position._M_node;
        _List_node<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};
template <class T> struct less { };
template <class K, class V, class C = less<K>, class A = allocator<V> > struct map;
}
template <class T, int N> struct CSmallVector {
    T*          m_data;                                    // +0
    signed char m_size;                                    // +4
    signed char m_capacity;                                // +5
    void resize(long size, const T& fill = T());           // 0x00A7DB10
    T* begin() { return m_data; }
    T* end() { return m_data + m_size; }
    void push_back(const T& value)
    {
        resize(m_size + 1);
        m_data[m_size - 1] = value;
    }
};
typedef CSmallVector<const CANavQuadTreeNode*, 8> CNeighbourVector;
typedef std::list<CANavQuadTreeNode*> CNodeList;
struct C2DVector { float X, Y; };
enum ENavNodeType { NAV_NODE_QUADTREE = 0, NAV_NODE_NAVIGABLE = 1, NAV_NODE_BLOCKED = 2, NAV_NODE_SWITCHABLE = 3 };
struct CANavQuadTreeNode {
    virtual ~CANavQuadTreeNode();                                                   // slot 0
    virtual const CNeighbourVector* GetNeighbours() const;                          // slot 2 (overloads sit in reverse order)
    virtual CNeighbourVector* GetNeighbours();                                      // slot 1
    virtual bool HasNeighbours() const;                                             // slot 3
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const;                // slot 4
    virtual long GetRegionID() const;                                               // slot 5 (+0x14)
    virtual long GetNodeType() const;                                               // slot 6 (+0x18)
    virtual CNavRegion* GetRegion() const;                                          // slot 7 (+0x1c)
    virtual unsigned char GetPreferabilityCost() const;                             // slot 8
    virtual bool IsSwitchable() const;                                              // slot 9
    virtual bool IsSwitchableOn() const;                                            // slot 10
    virtual CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos);                 // slot 11
    virtual bool MakeRegion(const CNavRegion* region, CNodeList& nodes);            // slot 12 (+0x30)
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion* region, CNodeList& nodes);   // slot 13
    virtual bool RemoveRegionZero();                                                // slot 14
    virtual long GetMemoryUsage(long& a, long& b, long& c, long& d) const;          // slot 15 (+0x3c)
    virtual void SetUpNeighboursWithLeaf(long unused, CANavQuadTreeNode* leaf);     // slot 16 (+0x40)
    virtual void SetUpNeighbours(long direction, CANavQuadTreeNode* node);          // slot 17 (+0x44)
    virtual void CalculateChildrenNeighbours();                                     // slot 18 (+0x48)
    virtual void RemoveNeighbours();                                                // slot 19 (+0x4c)
    virtual void AddToIndexTable(long& index, std::map<const CANavQuadTreeNode*, long>& table) const;   // slot 20 (+0x50)
    virtual void SaveToFile(CAFile& file, const std::map<const CANavQuadTreeNode*, long>& table, bool b, const C2DVector& v) const;   // slot 21
    virtual void ConvertIndexesToPointers();                                        // slot 22
    C2DVector     Position;                                // +0x04
    unsigned char QuadTreeLevel;                           // +0x0c
    unsigned char NavLayer;                                // +0x0d
    bool          LeafNode;                                // +0x0e
    bool          Blocked;                                 // +0x0f
};
struct CNavNavigableLeafNode : CANavQuadTreeNode {
    CNavRegion*      PRegion;                              // +0x10
    CNeighbourVector Neighbours;                           // +0x14
    unsigned char    PreferabilityCost;                    // +0x1c
    bool MakeRegion(const CNavRegion* region, CNodeList& nodes);
    bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion* region, CNodeList& nodes);
    void SetUpNeighboursWithLeaf(long unused, CANavQuadTreeNode* leaf);
};
bool CNavNavigableLeafNode::MakeRegion(const CNavRegion* region, CNodeList& nodes)
{
    if (GetRegionID() == 0) {
        PRegion = (CNavRegion*)region;
        for (const CANavQuadTreeNode** it = Neighbours.begin(); it != Neighbours.end(); ++it) {
            CANavQuadTreeNode* neighbour = (CANavQuadTreeNode*)*it;
            if (neighbour->GetRegionID() == 0)
                nodes.push_back(neighbour);
        }
        return true;
    }
    return false;
}

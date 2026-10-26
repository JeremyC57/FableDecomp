// CNavQuadTreeNode::GetMemoryUsage(unsigned long&, unsigned long&, unsigned long&, unsigned long&) const -- count this inner node and sum its own size with the memory used by its four children
// Leaf: ?GetMemoryUsage@CNavQuadTreeNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A761C0, 122 bytes (+ int3 padding; `ret 10h`). Vtable 0x0129DCD4 slot 15.
// Callees: each child's virtual slot 15 GetMemoryUsage (+0x3c), recursively.
// Layout = Ego_r.pdb CNavQuadTreeNode (0x20): CANavQuadTreeNode (0x10) + TLNode +0x10, TRNode +0x14, BLNode +0x18, BRNode +0x1c.
// Shape: the two counters are incremented first (the first and the last); the sum is built statement by statement starting with
//   sizeof(CNavQuadTreeNode) = 0x20 plus the TR child (one expression loads all four children up front).
class CNavRegion;
class CAFile;
class CANavQuadTreeNode;
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
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) { }
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
struct _Rb_tree_node_base;
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > struct map {
    typedef pair<const K, V> value_type;
    struct iterator { _Rb_tree_node_base* _M_node; };
    struct _Rep_type {                                       // _Rb_tree: header, node count, comparator
        _Rb_tree_node_base* _M_header;
        unsigned int        _M_node_count;
        pair<iterator, bool> insert_unique(const value_type& v);   // 0x00A819B0 for <const CANavQuadTreeNode*, long>
    };
    _Rep_type _M_t;
    pair<iterator, bool> insert(const value_type& v) { return _M_t.insert_unique(v); }
};
}
template <class T, int N> class CSmallVector { public:
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
class C2DVector { public: float X, Y; };
enum ENavNodeType { NAV_NODE_QUADTREE = 0, NAV_NODE_NAVIGABLE = 1, NAV_NODE_BLOCKED = 2, NAV_NODE_SWITCHABLE = 3 };
class CANavQuadTreeNode { public:
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
    virtual unsigned long GetMemoryUsage(unsigned long& quadTreeNodes, unsigned long& leafNodes, unsigned long& blockedNodes, unsigned long& totalNodes) const;   // slot 15 (+0x3c)
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
class CNavQuadTreeNode : public CANavQuadTreeNode { public:
    CANavQuadTreeNode* TLNode;                             // +0x10
    CANavQuadTreeNode* TRNode;                             // +0x14
    CANavQuadTreeNode* BLNode;                             // +0x18
    CANavQuadTreeNode* BRNode;                             // +0x1c
    unsigned long GetMemoryUsage(unsigned long& quadTreeNodes, unsigned long& leafNodes, unsigned long& blockedNodes, unsigned long& totalNodes) const;
    void AddToIndexTable(long& index, std::map<const CANavQuadTreeNode*, long>& table) const;
};
unsigned long CNavQuadTreeNode::GetMemoryUsage(unsigned long& quadTreeNodes, unsigned long& leafNodes, unsigned long& blockedNodes, unsigned long& totalNodes) const
{
    ++quadTreeNodes;
    ++totalNodes;
    unsigned long usage = sizeof(CNavQuadTreeNode) + TRNode->GetMemoryUsage(quadTreeNodes, leafNodes, blockedNodes, totalNodes);
    usage += TLNode->GetMemoryUsage(quadTreeNodes, leafNodes, blockedNodes, totalNodes);
    usage += BLNode->GetMemoryUsage(quadTreeNodes, leafNodes, blockedNodes, totalNodes);
    usage += BRNode->GetMemoryUsage(quadTreeNodes, leafNodes, blockedNodes, totalNodes);
    return usage;
}
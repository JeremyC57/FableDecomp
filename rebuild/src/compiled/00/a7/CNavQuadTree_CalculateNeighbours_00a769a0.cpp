// CNavQuadTree::CalculateNeighbours(vector<CANavQuadTreeNode*>&) const -- link the top-level grid of quad-tree nodes: each node computes its children's neighbours, then is joined to the node to its right (2) and the node above (1)
// Leaf: ?CalculateNeighbours@CNavQuadTree
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A769A0, 135 bytes (+ int3 padding; `ret 4`).
// Callees: __ftol2 @ 0x00BFEA70, the node's virtual slots 18 CalculateChildrenNeighbours (+0x48) and 17 SetUpNeighbours(long, CANavQuadTreeNode*) (+0x44).
// Layout = Ego_r.pdb CNavQuadTree: MapWidth +0x10 (the grid is MapWidth / 32 nodes wide; 0x0129DD98 = 0.03125f).
// Shape: the width is an unsigned long local (the `push ecx` frame word, used as the divisor from memory); STLport
//   `size()` is the pointer difference `sar 2` re-evaluated every iteration; `operator[]` is `*(begin() + n)`; VC strength-reduces
//   the `i - width - 1` index into the negated-width byte offset in ebp; the index is advanced right after the children call
//   (a `for` with `++i` in the header computes `i + 1` separately and indexes differently).
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
template <class K, class V, class C = less<K>, class A = allocator<V> > struct map;
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
namespace std {
template <class T, class A = allocator<T> > struct vector {           // STLport vector: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() { return end() - begin(); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
class CNavQuadTree { public:
    void*         __vftable;                       // +0x00
    bool          Initialised;                     // +0x04
    bool          Activated;                       // +0x05
    bool          Initialising;                    // +0x06
    unsigned char _pad07[0x09];
    float         MapWidth;                        // +0x10
    float         MapHeight;                       // +0x14
private:
    void CalculateNeighbours(std::vector<CANavQuadTreeNode*>& nodes) const;
};

void CNavQuadTree::CalculateNeighbours(std::vector<CANavQuadTreeNode*>& nodes) const
{
    unsigned long width = (unsigned long)(MapWidth * 0.03125f);
    for (unsigned long i = 0; i < nodes.size(); ) {
        CANavQuadTreeNode* node = nodes[i];
        node->CalculateChildrenNeighbours();
        ++i;
        if (i % width != 0)
            node->SetUpNeighbours(2, nodes[i]);
        if (i > width)
            node->SetUpNeighbours(1, nodes[i - width - 1]);
    }
}
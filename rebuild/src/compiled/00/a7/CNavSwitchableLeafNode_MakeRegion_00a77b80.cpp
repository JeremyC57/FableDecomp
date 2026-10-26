// CNavSwitchableLeafNode::MakeRegion(const CNavRegion*, list<CANavQuadTreeNode*>&) -- claim an unassigned switchable leaf for the region being flooded and queue every unassigned switchable neighbour on the open list
// Leaf: ?MakeRegion@CNavSwitchableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77B80, 168 bytes (+ int3 padding; `ret 8`). Protected virtual (MAE) slot 12 of the
//   CNavSwitchableLeafNode vtable 0x0129DE04.
// Callees: virtual slots 1 GetNeighbours (+4, 0x00A7F2A0 `lea eax,[ecx+0x14]`), 5 GetRegionID (+0x14, 0x00A75BE0) and 6 GetNodeType
//   (+0x18; the switchable leaf's 0x00A84E00 returns 3); malloc 0x00BFEA0E for the 12-byte STLport list node.
// Layout = Ego_r.pdb: CANavQuadTreeNode {vptr, Position +4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe, Blocked +0xf};
//   CNavNavigableLeafNode {PRegion +0x10, Neighbours +0x14 (CSmallVector: data +0, signed char size +4)}.
// Shape: a node that already has a region ID refuses; otherwise PRegion is set and Blocked cleared (VC reuses the zero in al).
//   The neighbour walk takes begin() once and re-calls GetNeighbours() for every end() test; push_back is the inline STLport
//   insert(end(), x): node from the allocator (malloc), placement-constructed value, linked before the header.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class C2DVector { public: float X, Y; };
class CNavRegion;
class CANavQuadTreeNode;
namespace std {
template <class T> struct allocator { };
template <class T> struct _List_node {
    _List_node* _M_next;                           // +0x00
    _List_node* _M_prev;                           // +0x04
    T           _M_data;                           // +0x08
};
template <class T, class A = allocator<T> > class list {
public:
    _List_node<T>* _M_node;                        // the header node
    struct iterator {
        _List_node<T>* _M_node;
        iterator(_List_node<T>* node) : _M_node(node) { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
    };
    iterator end() { return iterator(_M_node); }
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* p = (_List_node<T>*)malloc(sizeof(_List_node<T>));
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
}
template <class T, int N> class CSmallVector { public:
    T*          m_data;                            // +0
    signed char m_size;                            // +4
    signed char m_capacity;                        // +5
    T* begin() const { return m_data; }
    T* end() const { return m_data + m_size; }
};
typedef CSmallVector<const CANavQuadTreeNode*, 8> CNeighbourVector;
class CANavQuadTreeNode {
public:
    virtual void v000();
    virtual const CNeighbourVector* GetNeighbours() const;                    // slot 2 (overloads sit in reverse order)
    virtual CNeighbourVector* GetNeighbours();                                // slot 1 (+0x04)
    virtual void v003();
    virtual void v004();
    virtual long GetRegionID() const;                                         // slot 5 (+0x14)
    virtual long GetNodeType() const;                                         // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
protected:
    virtual bool MakeRegion(const CNavRegion* region, std::list<CANavQuadTreeNode*>& openList);   // slot 12 (+0x30)
public:
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    const CNavRegion* PRegion;                     // +0x10
    CNeighbourVector  Neighbours;                  // +0x14
    unsigned char     PreferabilityCost;           // +0x1c
};
class CNavSwitchableLeafNode : public CNavNavigableLeafNode {
protected:
    virtual bool MakeRegion(const CNavRegion* region, std::list<CANavQuadTreeNode*>& openList);
};

bool CNavSwitchableLeafNode::MakeRegion(const CNavRegion* region, std::list<CANavQuadTreeNode*>& openList)
{
    if (GetRegionID() == 0) {
        PRegion = region;
        Blocked = false;
        for (const CANavQuadTreeNode** it = GetNeighbours()->begin(); it != GetNeighbours()->end(); ++it) {
            CANavQuadTreeNode* neighbour = (CANavQuadTreeNode*)*it;
            if (neighbour->GetRegionID() == 0 && neighbour->GetNodeType() == 3)
                openList.push_back(neighbour);
        }
        return true;
    }
    return false;
}
// Behaviour test for 00a77b00: src/00a77b00.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CNavNavigableLeafNode::MakeRegion(const CNavRegion*, list<CANavQuadTreeNode*>&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CNavNavigableLeafNode::MakeRegion(const CNavRegion*, list<CANavQuadTreeNode*>&) -- claim this leaf for the region and queue every neighbour that has no region yet
// Leaf: ?MakeRegion@CNavNavigableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77B00, 123 bytes (+ int3 padding; `ret 8`). Vtable 0x0129DDA4 slot 12.
// Callees: virtual slot 5 GetRegionID (+0x14) on this and on each neighbour, operator new @ 0x00BFEA0E (12-byte STLport list node).
// Shape: STLport `list::push_back` inlined: `end()` read before the node allocation, the value placement-constructed behind a null
//   test of its address, then the four link stores (next, prev, prev->next, head->prev).
class CNavRegion;
class CAFile;
class CANavQuadTreeNode;
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
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
class CNavNavigableLeafNode : public CANavQuadTreeNode { public:
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

// ---- stubs and driver ----

#include <stdlib.h>
static CNavRegion* const REGION = (CNavRegion*)0x3131;
CANavQuadTreeNode::~CANavQuadTreeNode() {}
const CNeighbourVector* CANavQuadTreeNode::GetNeighbours() const { return 0; }
CNeighbourVector* CANavQuadTreeNode::GetNeighbours() { return 0; }
bool CANavQuadTreeNode::HasNeighbours() const { return false; }
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*) const { return false; }
long CANavQuadTreeNode::GetRegionID() const { return ((const CNavNavigableLeafNode*)this)->PRegion ? 9 : 0; }
long CANavQuadTreeNode::GetNodeType() const { return 1; }
CNavRegion* CANavQuadTreeNode::GetRegion() const { return 0; }
unsigned char CANavQuadTreeNode::GetPreferabilityCost() const { return 0; }
bool CANavQuadTreeNode::IsSwitchable() const { return false; }
bool CANavQuadTreeNode::IsSwitchableOn() const { return false; }
CANavQuadTreeNode* CANavQuadTreeNode::GetLeafNodeAt(const C2DVector&) { return 0; }
bool CANavQuadTreeNode::MakeRegion(const CNavRegion*, CNodeList&) { return false; }
bool CANavQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*, CNodeList&) { return false; }
bool CANavQuadTreeNode::RemoveRegionZero() { return false; }
long CANavQuadTreeNode::GetMemoryUsage(long&, long&, long&, long&) const { return 0; }
void CANavQuadTreeNode::SetUpNeighboursWithLeaf(long, CANavQuadTreeNode*) {}
void CANavQuadTreeNode::SetUpNeighbours(long, CANavQuadTreeNode*) {}
void CANavQuadTreeNode::CalculateChildrenNeighbours() {}
void CANavQuadTreeNode::RemoveNeighbours() {}
void CANavQuadTreeNode::AddToIndexTable(long&, std::map<const CANavQuadTreeNode*, long>&) const {}
void CANavQuadTreeNode::SaveToFile(CAFile&, const std::map<const CANavQuadTreeNode*, long>&, bool, const C2DVector&) const {}
void CANavQuadTreeNode::ConvertIndexesToPointers() {}
bool CNavNavigableLeafNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*, CNodeList&) { return false; }
void CNavNavigableLeafNode::SetUpNeighboursWithLeaf(long, CANavQuadTreeNode*) {}
static int Count(std::_List_node<CANavQuadTreeNode*>* h) { int c = 0; for (std::_List_node<CANavQuadTreeNode*>* n = h->_M_next; n != h; n = n->_M_next) ++c; return c; }
int main()
{
    static CNavNavigableLeafNode nodes[4];
    const CANavQuadTreeNode* nb[3] = { &nodes[1], &nodes[2], &nodes[3] };
    nodes[0].Neighbours.m_data = nb; nodes[0].Neighbours.m_size = 3; nodes[0].PRegion = 0;
    nodes[1].PRegion = 0; nodes[2].PRegion = REGION; nodes[3].PRegion = 0;
    std::_List_node<CANavQuadTreeNode*> head; head._M_next = &head; head._M_prev = &head;
    CNodeList open; open._M_node = &head;
    CHECK(nodes[0].MakeRegion(REGION, open) == true);
    CHECK(nodes[0].PRegion == REGION);
    CHECK(Count(&head) == 2); CHECK(head._M_next->_M_data == &nodes[1]); CHECK(head._M_prev->_M_data == &nodes[3]);
    CHECK(nodes[0].MakeRegion(REGION, open) == false); CHECK(Count(&head) == 2);
    if (fails == 0) printf("00a77b00_TEST PASS\n"); else printf("00a77b00_TEST FAIL\n");
    return fails;
}
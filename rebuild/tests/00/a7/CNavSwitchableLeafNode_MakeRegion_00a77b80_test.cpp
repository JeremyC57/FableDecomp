// Behaviour test for 00a77b80: src/00a77b80.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CNavSwitchableLeafNode::MakeRegion(const CNavRegion*, list<CANavQuadTreeNode*>&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
struct C2DVector { float X, Y; };
struct CNavRegion;
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
template <class T, int N> struct CSmallVector {
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

// ---- stubs and driver ----
void CANavQuadTreeNode::v000() {}
void CANavQuadTreeNode::v003() {}
void CANavQuadTreeNode::v004() {}
void CANavQuadTreeNode::v007() {}
void CANavQuadTreeNode::v008() {}
void CANavQuadTreeNode::v009() {}
void CANavQuadTreeNode::v010() {}
void CANavQuadTreeNode::v011() {}
#include <stdlib.h>
static CNavRegion* const REGION = (CNavRegion*)0x3131;
long CANavQuadTreeNode::GetRegionID() const { return ((const CNavNavigableLeafNode*)this)->PRegion ? 7 : 0; }
long CANavQuadTreeNode::GetNodeType() const { return QuadTreeLevel; }
const CNeighbourVector* CANavQuadTreeNode::GetNeighbours() const { return &((const CNavNavigableLeafNode*)this)->Neighbours; }
CNeighbourVector* CANavQuadTreeNode::GetNeighbours() { return &((CNavNavigableLeafNode*)this)->Neighbours; }
bool CANavQuadTreeNode::MakeRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&) { return false; }
struct TestNode : CNavSwitchableLeafNode {
    bool Call(const CNavRegion* r, std::list<CANavQuadTreeNode*>& l) { return CNavSwitchableLeafNode::MakeRegion(r, l); }
};
static int Count(std::_List_node<CANavQuadTreeNode*>* h) { int c = 0; for (std::_List_node<CANavQuadTreeNode*>* n = h->_M_next; n != h; n = n->_M_next) ++c; return c; }
int main()
{
    static TestNode nodes[4]; memset(nodes, 0, sizeof nodes);
    for (int i = 0; i < 4; ++i) new (&nodes[i]) TestNode;
    const CANavQuadTreeNode* nb[3] = { &nodes[1], &nodes[2], &nodes[3] };
    nodes[0].Neighbours.m_data = nb; nodes[0].Neighbours.m_size = 3; nodes[0].Blocked = true;
    nodes[1].QuadTreeLevel = 3;                         // switchable, no region: queued
    nodes[2].QuadTreeLevel = 3; nodes[2].PRegion = REGION;   // already has a region
    nodes[3].QuadTreeLevel = 1;                         // not switchable
    std::_List_node<CANavQuadTreeNode*> head; head._M_next = &head; head._M_prev = &head;
    std::list<CANavQuadTreeNode*> open; open._M_node = &head;
    CHECK(nodes[0].Call(REGION, open) == true);
    CHECK(nodes[0].PRegion == REGION); CHECK(nodes[0].Blocked == false);
    CHECK(Count(&head) == 1); CHECK(head._M_next->_M_data == &nodes[1]); CHECK(head._M_prev == head._M_next);
    CHECK(nodes[0].Call(REGION, open) == false); CHECK(Count(&head) == 1);
    if (fails == 0) printf("00a77b80_TEST PASS\n"); else printf("00a77b80_TEST FAIL\n");
    return fails;
}
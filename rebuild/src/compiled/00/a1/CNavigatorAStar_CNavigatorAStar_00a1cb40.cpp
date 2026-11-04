// CNavigatorAStar::~CNavigatorAStar() -- take this navigator out of its manager's route-calculation queue, then destroy the way-point Route, the OpenList heap, the three blocked quad-tree nodes and the A* States map, then ~CNavigatorBase
// Leaf: ??1CNavigatorAStar
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1CB40, 164 bytes. Sole caller: the ??_G 0x00A224D0 (slot 0 of the CNavigatorAStar
//   vtable 0x0129CCCC).
// Identity: first store = vtable 0x0129CCCC (RTTI .?AVCNavigatorAStar@@ : CNavigatorBase : CClass : CBaseObject : CBaseClass),
//   whose slot 0 ??_G is the only caller; Ego_r.pdb ??1CNavigatorAStar@@UAE@XZ (user-declared virtual: the store stays).
// Callees: std::list<std::pair<long,const CNavigatorBase*> >::remove_if<CIsNavigator> 0x00A155C0 (Ego_r/FableWin.pdb
//   ??$remove_if@VCIsNavigator@@@?$list@U?$pair@JPBVCNavigatorBase@@@std@@..; the CIsNavigator predicate is passed by value,
//   `push esi`); std::_List_base<CNavWayPoint>::~_List_base 0x00A214C0 (kept out of line by VC7.1, as retail);
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (three times: the inline ~CANavQuadTreeNode of each blocked node);
//   free 0x00BFEA14; the States tree's _M_erase 0x00A21640; ~CNavigatorBase 0x00A47180 (tail jump).
// Shape: the body is the inline CNavigatorQueueManager::RemoveNavigatorFromQueue(const CNavigatorBase&) (Ego_r 0x00B82AB0,
//   expanded) reached through CNavigatorManager::GetNavigatorQueueManager() (FableWin.pdb) on NavigationManager +0x1C (the
//   queue at manager +0x2C). Members then die in reverse order: Route +0x8C (call), OpenList's heap vector +0x60 (free),
//   BlockedNodeAskedToGetTo +0x50 / BlockedEndNode +0x40 / BlockedStartNode +0x30 (CNavBlockedQuadTreeNode has no dtor of
//   its own -- no vtable store of its own -- so the user inline ~CANavQuadTreeNode stores 0x0129CBBC and calls the base
//   dtor), States +0x24 (STLport _Rb_tree::clear + header free).
// Layout (Ego_r.pdb less the Dinkumware container growth; engine/CNavigatorBase.h / CNavigatorManager.h agree): CNavigatorBase
//   PInterface +0x14, OnRoute +0x18, Interrupted +0x19, NavigationManager +0x1C, SteeringAgent +0x20; CNavigatorAStar States
//   +0x24 .. CullBack +0x94 (members after CullBack are not touched).
// Header debt: engine/CNavigatorBase.h, CClass.h, CNavBlockedQuadTreeNode.h, CANavQuadTreeNode.h and CNavigatorManager.h are
//   flattened (vptr words, opaque containers), so the chain is modelled locally with the PDB names; CBaseObject and
//   CBaseClassNonCopyable come from the retail headers. STLport 4.6 text (stl/_tree.h, _list.h/_list.c, _vector.h) in
//   namespace std, reduced to the members this destructor instantiates.
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CBaseClassNonCopyable.h"

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
// ---- vector (stl/_vector.h)
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
// ---- list (stl/_list.h, stl/_list.c)
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    template <class _Predicate> void remove_if(_Predicate __pred);
};
// ---- tree (stl/_tree.h)
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                                       // stl/_tree.c (out of line: 0x00A21640)
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
} // namespace std

class CClass : public CBaseObject {
public:
    virtual ~CClass();
    std::_List_node_base* ClassListPos;                                  // +0x08 list<CClass*>::iterator
    std::_List_node_base* TypeListPos;                                   // +0x0c
    unsigned char Type;                                                  // +0x10
};
class CNavigatorExternalInterface;
class CNavigatorManager;
class CSteeringAgent;
class CNavigatorBase : public CClass {
public:
    virtual ~CNavigatorBase();                                           // 0x00A47180
    CNavigatorExternalInterface* PInterface;                             // +0x14
    bool OnRoute;                                                        // +0x18
    bool Interrupted;                                                    // +0x19
    CNavigatorManager* NavigationManager;                                // +0x1c
    CSteeringAgent* SteeringAgent;                                       // +0x20
};
class CIsNavigator {
public:
    CIsNavigator(const CNavigatorBase& navigator) : Navigator(const_cast<CNavigatorBase*>(&navigator)) { }
    bool operator()(const std::pair<long, const CNavigatorBase*>& entry) const { return entry.second == Navigator; }
    CNavigatorBase* Navigator;
};
class CNavigatorQueueManager {
public:
    std::list<std::pair<long, const CNavigatorBase*> > Navigators;       // +0x00
    void RemoveNavigatorFromQueue(const CNavigatorBase& navigator) { Navigators.remove_if(CIsNavigator(navigator)); }
};
class CNavigatorManager {
public:
    unsigned char _pad_0x00[0x2c];
    CNavigatorQueueManager QueueManager;                                 // +0x2c
    CNavigatorQueueManager& GetNavigatorQueueManager() { return QueueManager; }
};
class C2DVector { public: float X, Y; };
#include "engine/C3DVector.h"
class CANavQuadTreeNode : public CBaseClassNonCopyable {
public:
    virtual ~CANavQuadTreeNode() { }
    C2DVector Position;                                                  // +0x04
    unsigned char QuadTreeLevel;                                         // +0x0c
    unsigned char NavLayer;                                              // +0x0d
    bool LeafNode;                                                       // +0x0e
    bool Blocked;                                                        // +0x0f
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
};
template <class T> class CPriorityQueue {
public:
    std::vector<T> Heap;                                                 // +0x00
};
#pragma pack(push, 1)
class CNavWayPoint {
public:
    C2DVector Position;
    unsigned char NavLayer;
    bool Removable;
};
#pragma pack(pop)
class CNavigatorAStar : public CNavigatorBase {
public:
    class CAStarState {
    public:
        CAStarState* ParentState;
        CANavQuadTreeNode* Node;
        float CostToThisState;
        float TotalCost;
        bool OnOpenList;
        bool OnClosedList;
    };
    virtual ~CNavigatorAStar();
    std::map<long, CAStarState> States;                                  // +0x24
    CNavBlockedQuadTreeNode BlockedStartNode;                            // +0x30
    CNavBlockedQuadTreeNode BlockedEndNode;                              // +0x40
    CNavBlockedQuadTreeNode BlockedNodeAskedToGetTo;                     // +0x50
    CPriorityQueue<CAStarState*> OpenList;                               // +0x60
    CANavQuadTreeNode* GoalNode;                                         // +0x6c
    C2DVector GoalPosition;                                              // +0x70
    CANavQuadTreeNode* NodeAskedToGetTo;                                 // +0x78
    C3DVector PosAskedToGetTo;                                           // +0x7c
    CANavQuadTreeNode* LastNavigableNodeVisited;                         // +0x88
    std::list<CNavWayPoint> Route;                                       // +0x8c
    std::_List_node_base* CullFront;                                     // +0x90
    std::_List_node_base* CullBack;                                      // +0x94
};
CNavigatorAStar::~CNavigatorAStar()
{
    NavigationManager->GetNavigatorQueueManager().RemoveNavigatorFromQueue(*this);
}
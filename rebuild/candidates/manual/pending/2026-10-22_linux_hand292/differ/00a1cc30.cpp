// Grade: DIFFER(632v632) -- one x87 store scheduled differently: retail sinks the radius 'fstp [esp+10h]' below the statistics 'mov eax,[13D27B8h] ; cmp eax,[13D27C4h]' (the lane stores it straight after the GetRadius call); everything else, including relocations, byte-exact. Fixed on the way: a same-TU auto_inline(off) GetConnectingPoints stand-in with direct initialisation of the named result (659v632 -> 632v632) and the real STLport list insert/_M_create_node/push_front shape (keeps ebx/ebp pushed at entry). Tried for the store: a separate declaration, a const local, the statistics in an inline helper.
// CNavigatorAStar::ConstructFinalRoute(const CAStarState* goal) -- rebuild the way-point list by walking the solved A* states back from the goal: each node centre is pushed on the front, plus (between same-layer nodes) the connecting points when the node is wider than the navigator; then drop the search state and the manager's active entry
// Leaf: ?ConstructFinalRoute@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1CC30, 640 bytes (`ret 4`).
// Callees: the interface's slot 6 GetRadius (+0x18), free 0x00BFEA14, malloc 0x00BFEA0E, GetConnectingPoints 0x00A1A7E0
//   (16-byte result by hidden pointer), CANavQuadTreeNode::GetSize 0x00A75B30, the state map's _M_erase 0x00A21640,
//   memmove 0x00BFEAE6, list<pair<long, const CNavigatorBase*> >::remove_if<CIsNavigator> 0x00A155C0; the search
//   statistics 0x013D27B0..0x013D27CC.
// Layout: as 0x00A1E370 (States +0x24, OpenList +0x60, Route +0x8c); CAStarState {Parent +0, Node +4}.
#include <string.h>
#include <stdlib.h>
#define CNavigatorBase CNavigatorBaseLayout
#include "engine/CNavigatorBase.h"
#undef CNavigatorBase
struct CAStarState;
inline void* operator new(unsigned int, void* where) { return where; }
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
struct C3DVector {
    float X, Y, Z;
};
#pragma pack(push, 1)
struct SRoutePoint {                               // std::pair<C2DVector, unsigned char> (9 bytes)
    C2DVector     first;
    unsigned char second;
};
struct CNavWayPoint {                              // 10 bytes
    C2DVector     Position;
    unsigned char Type;
    bool          Valid;
    CNavWayPoint(const C2DVector& p, unsigned char type) : Position(p), Type(type), Valid(true) {}
};
#pragma pack(pop)
struct CRoutePointVector {                         // STLport vector<SRoutePoint>
    SRoutePoint* _M_start;
    SRoutePoint* _M_finish;
    SRoutePoint* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const SRoutePoint& operator[](unsigned long n) const { return _M_start[n]; }
};
struct SWayPointNode {                             // _List_node<CNavWayPoint>
    SWayPointNode* _M_next;
    SWayPointNode* _M_prev;
    CNavWayPoint   _M_data;
};
inline void __distance(SWayPointNode* first, SWayPointNode* last, unsigned long& n)
{
    while (first != last) {
        first = first->_M_next;
        ++n;
    }
}
struct CWayPointIterator {                         // STLport _List_iterator
    SWayPointNode* _M_node;
    CWayPointIterator(SWayPointNode* n) : _M_node(n) {}
    CWayPointIterator(const CWayPointIterator& o) : _M_node(o._M_node) {}
};
struct CWayPointList {                             // STLport list<CNavWayPoint>
    SWayPointNode* _M_node;
    CWayPointIterator begin() { return CWayPointIterator(_M_node->_M_next); }
    void clear()
    {
        SWayPointNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SWayPointNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    SWayPointNode* _M_create_node(const CNavWayPoint& x)
    {
        SWayPointNode* tmp = (SWayPointNode*)malloc(sizeof(SWayPointNode));
        new (&tmp->_M_data) CNavWayPoint(x);
        return tmp;
    }
    CWayPointIterator insert(CWayPointIterator position, const CNavWayPoint& x)
    {
        SWayPointNode* tmp = _M_create_node(x);
        SWayPointNode* prev = position._M_node->_M_prev;
        tmp->_M_next = position._M_node;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        position._M_node->_M_prev = tmp;
        return CWayPointIterator(tmp);
    }
    void push_front(const CNavWayPoint& x) { insert(begin(), x); }
};
struct SStateTreeNode {                            // _Rb_tree_node_base
    long            _M_color;
    SStateTreeNode* _M_parent;
    SStateTreeNode* _M_left;
    SStateTreeNode* _M_right;
};
struct CStateMap {                                 // STLport map<long, CAStarState>
    SStateTreeNode* _M_header;
    unsigned long   _M_node_count;
    unsigned long   _M_key_compare;
    void _M_erase(SStateTreeNode* x);              // 0x00A21640
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
         : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct __true_type { __true_type() {} };
template <class T> inline T* __copy_ptrs(const T* first, const T* last, T* result, const __true_type&)
{
    return (T*)__copy_trivial(first, last, result);
}
struct CStatePtrVector {                           // STLport vector<CAStarState*>
    CAStarState** _M_start;
    CAStarState** _M_finish;
    CAStarState** _M_end_of_storage;
    CAStarState** erase(CAStarState** first, CAStarState** last)
    {
        CAStarState** i = __copy_ptrs(last, _M_finish, first, __true_type());
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
};
struct CANavQuadTreeNode {
    void*         __vftable;                       // +0x00
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    float GetSize() const;                         // 0x00A75B30
};
struct CAStarState {
    const CAStarState*       Parent;               // +0x00
    const CANavQuadTreeNode* Node;                 // +0x04
};
struct SConnectingPoints {
    C2DVector First;
    C2DVector Second;
};
struct CIsNavigator {
    const CNavigatorBaseLayout* Navigator;
    CIsNavigator(const CNavigatorBaseLayout* n) : Navigator(n) {}
};
struct CNavigatorList {
    void remove_if(CIsNavigator pred);             // 0x00A155C0
};
struct CNavigatorManager {
    unsigned char  _pad00[0x2c];
    CNavigatorList Navigators;                     // +0x2c
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, long layer);   // 0x00A14F70
};
struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual float GetRadius(bool b);               // slot 6 (+0x18)
};
struct CSteeringAgent {
    void SetGoal(const CNavWayPoint& goal, const CNavWayPoint& next);   // 0x00A20970
};
extern long g_AStarSearchesInProgress;             // 0x013D27B0
extern long g_AStarStat_b4;                        // 0x013D27B4
extern long g_AStarStat_b8;                        // 0x013D27B8
extern long g_AStarStat_bc;                        // 0x013D27BC
extern long g_AStarStat_c0;                        // 0x013D27C0
extern long g_AStarStat_c4;                        // 0x013D27C4
extern long g_AStarStat_c8;                        // 0x013D27C8
extern long g_AStarStat_cc;                        // 0x013D27CC
struct CNavigatorAStar {
    // +0x00 vptr
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;          // +0x18
    bool                         Interrupted;      // +0x19
    CNavigatorManager*           NavigationManager;// +0x1c
    void*                        SteeringAgent;    // +0x20
    CStateMap                    States;           // +0x24
    unsigned char                _pad30[0x30];     // +0x30 BlockedStartNode / BlockedEndNode / BlockedNodeAskedToGetTo
    CStatePtrVector              OpenList;         // +0x60
    unsigned char                _pad6c[0x20];     // +0x6c
    CWayPointList                Route;            // +0x8c
    virtual void v00();
    SConnectingPoints GetConnectingPoints(const CANavQuadTreeNode* from, const CANavQuadTreeNode* to, float radius);   // 0x00A1A7E0
    void ConstructFinalRoute(const CAStarState* goal);
};

void* FindRef_GetConnectingPoints_0();
#pragma auto_inline(off)
__declspec(noinline) SConnectingPoints CNavigatorAStar::GetConnectingPoints(const CANavQuadTreeNode* from, const CANavQuadTreeNode* to, float radius)
{
    SConnectingPoints p;
    p.First = from->Position;
    p.Second = to->Position;
    if (FindRef_GetConnectingPoints_0())
        p.First.X = radius;
    return p;
}
#pragma auto_inline(on)

void CNavigatorAStar::ConstructFinalRoute(const CAStarState* goal)
{
    float radius = PInterface->GetRadius(false);
    if (g_AStarStat_b8 > g_AStarStat_c4) {
        g_AStarStat_c4 = g_AStarStat_b8;
        g_AStarStat_c8 = g_AStarStat_c0;
        g_AStarStat_cc = g_AStarStat_bc;
        g_AStarStat_b4 = g_AStarSearchesInProgress;
    }
    Route.clear();
    Route.push_front(CNavWayPoint(goal->Node->Position, goal->Node->NavLayer));
    const CAStarState* state = goal;
    for (const CAStarState* parent = goal->Parent; parent != 0; parent = parent->Parent) {
        if (state->Node->NavLayer == parent->Node->NavLayer) {
            SConnectingPoints points = GetConnectingPoints(state->Node, parent->Node, radius);
            float diameter = radius + radius;
            if (state->Node->GetSize() > diameter)
                Route.push_front(CNavWayPoint(points.First, state->Node->NavLayer));
            if (parent->Node->GetSize() > diameter)
                Route.push_front(CNavWayPoint(points.Second, parent->Node->NavLayer));
        }
        Route.push_front(CNavWayPoint(parent->Node->Position, parent->Node->NavLayer));
        state = parent;
    }
    States.clear();
    OpenList.clear();
    --g_AStarSearchesInProgress;
    NavigationManager->Navigators.remove_if(CIsNavigator((const CNavigatorBaseLayout*)this));
}

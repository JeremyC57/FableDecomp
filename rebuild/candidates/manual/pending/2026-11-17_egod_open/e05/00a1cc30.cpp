// CNavigatorAStar::ConstructFinalRoute(const CAStarState* goal) -- rebuild the way-point list by walking the solved A* states back from the goal: each node centre is pushed on the front, plus (between same-layer nodes) the connecting points when the node is wider than the navigator; then drop the search state and the manager's active entry
// Grade: DIFFER(632v632) -- not landable: RELOCATION_MATCH only with a STAND-IN body of the same-TU callee GetIntersectionPointsBetweenNodes 0x00A1A7E0 (placeholder; the real row is RESERVED / parked, Codex pass23 DIFFER 862v862) and on local models (engine/CNavigatorAStar.h has no includer). e05 re-checked 2026-11-17: still RELOCATION_MATCH with Ego_d's names (goal_state, const float navigator_radius, previous_state at function scope, next_state / intersection_points in the loop block, no diameter local: `navigator_radius + navigator_radius` written twice). Pair job with 0x00A1A7E0.
// Leaf: ?ConstructFinalRoute@CNavigatorAStar@@AAEXPBVCAStarState@1@@Z
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
template <class T> struct CStatePtrVector {       // STLport vector<CAStarState*> (CPriorityQueue<CAStarState*>::Heap)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last)
    {
        T* i = __copy_ptrs(last, _M_finish, first, __true_type());
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
struct SPointPair {                                // std::pair<C2DVector, C2DVector> (16 bytes)
    C2DVector first;
    C2DVector second;
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
extern long g_AStarSearchesInProgress;             // 0x013D27B0 (descriptive, as the landed 0x00A1E370; no PDB data name)
extern long g_AStarSearchesAtPeak;                 // 0x013D27B4 (descriptive: the search count recorded with a new state peak)
extern long g_PeakNoOfStates;                      // 0x013D27C4 (descriptive: the highest NoOfStates seen)
extern long g_PeakNoOfStatesOnClosedList;          // 0x013D27C8 (descriptive: NoOfStatesOnClosedList at that peak)
extern long g_PeakNoOfStatesOnOpenList;            // 0x013D27CC (descriptive: NoOfStatesOnOpenList at that peak)
class CNavigatorAStar { public:
    // +0x00 vptr
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;          // +0x18
    bool                         Interrupted;      // +0x19
    CNavigatorManager*           NavigationManager;// +0x1c
    void*                        SteeringAgent;    // +0x20
    CStateMap                    States;           // +0x24
    unsigned char                _pad30[0x30];     // +0x30 BlockedStartNode / BlockedEndNode / BlockedNodeAskedToGetTo
    class CAStarState { public:                    // Ego_r.pdb CNavigatorAStar::CAStarState
        const CAStarState*       Parent;           // +0x00 ParentState
        const CANavQuadTreeNode* Node;             // +0x04
        static long NoOfStates;                    // 0x013D27B8 ?NoOfStates@CAStarState@CNavigatorAStar@@2JA
        static long NoOfStatesOnOpenList;          // 0x013D27BC
        static long NoOfStatesOnClosedList;        // 0x013D27C0
    };
    CStatePtrVector<CAStarState*> OpenList;        // +0x60
    unsigned char                _pad6c[0x20];     // +0x6c
    CWayPointList                Route;            // +0x8c
    virtual void v00();
private:
    SPointPair GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& from, const CANavQuadTreeNode& to, float radius) const;   // 0x00A1A7E0 ?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z
    void ConstructFinalRoute(const CAStarState* goal);
};

// STAND-IN (not landable): a placeholder same-TU body for 0x00A1A7E0. Retail's TU holds the real body; it must be replaced by the
// faithful GetIntersectionPointsBetweenNodes text once that row (Codex pass 23 RESERVED, hand721/differ/00a1a7e0.cpp) lands.
void* FindRef_GetIntersectionPointsBetweenNodes_0();
#pragma auto_inline(off)
__declspec(noinline) SPointPair CNavigatorAStar::GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& from, const CANavQuadTreeNode& to, float radius) const
{
    SPointPair p;
    p.first = from.Position;
    p.second = to.Position;
    if (FindRef_GetIntersectionPointsBetweenNodes_0())
        p.first.X = radius;
    return p;
}
#pragma auto_inline(on)

void CNavigatorAStar::ConstructFinalRoute(const CAStarState* goal_state)
{
    const float navigator_radius = (PInterface->GetRadius(false));
    if (CAStarState::NoOfStates > g_PeakNoOfStates) {
        g_PeakNoOfStates = CAStarState::NoOfStates;
        g_PeakNoOfStatesOnClosedList = CAStarState::NoOfStatesOnClosedList;
        g_PeakNoOfStatesOnOpenList = CAStarState::NoOfStatesOnOpenList;
        g_AStarSearchesAtPeak = g_AStarSearchesInProgress;
    }
    Route.clear();
    Route.push_front(CNavWayPoint(goal_state->Node->Position, goal_state->Node->NavLayer));
    const CAStarState* previous_state = goal_state;
    for (const CAStarState* next_state = goal_state->Parent; next_state != 0; next_state = next_state->Parent) {
        if (previous_state->Node->NavLayer == next_state->Node->NavLayer) {
            SPointPair intersection_points = GetIntersectionPointsBetweenNodes(*previous_state->Node, *next_state->Node, navigator_radius);
            if (previous_state->Node->GetSize() > navigator_radius + navigator_radius)
                Route.push_front(CNavWayPoint(intersection_points.first, previous_state->Node->NavLayer));
            if (next_state->Node->GetSize() > navigator_radius + navigator_radius)
                Route.push_front(CNavWayPoint(intersection_points.second, next_state->Node->NavLayer));
        }
        Route.push_front(CNavWayPoint(next_state->Node->Position, next_state->Node->NavLayer));
        previous_state = next_state;
    }
    States.clear();
    OpenList.clear();
    --g_AStarSearchesInProgress;
    NavigationManager->Navigators.remove_if(CIsNavigator((const CNavigatorBaseLayout*)this));
}

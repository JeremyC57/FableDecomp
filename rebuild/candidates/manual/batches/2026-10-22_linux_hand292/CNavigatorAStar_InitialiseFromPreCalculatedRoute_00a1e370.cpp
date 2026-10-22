// CNavigatorAStar::InitialiseFromPreCalculatedRoute(const C3DVector& from, const vector<pair<C2DVector, unsigned char> >& route, const C3DVector& to) -- drop any search in progress (state map, open list, the manager's active-navigator entry), rebuild the way-point list from the given route, hand the first leg to the steering agent and record the goal node/position
// Leaf: ?InitialiseFromPreCalculatedRoute@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1E370, 592 bytes (`ret 0Ch`).
// Callees: list<pair<long, const CNavigatorBase*> >::remove_if<CIsNavigator> 0x00A155C0 (on the manager +0x2c), the state
//   map's _M_erase 0x00A21640, free 0x00BFEA14, memmove 0x00BFEAE6, malloc 0x00BFEA0E, CalcWayPointWidths 0x00A1DE30,
//   CSteeringAgent::SetGoal 0x00A20970, the interface's slot 16 GetLayerAt (+0x40), CNavigatorManager::GetNodeAt 0x00A14F70;
//   the in-progress search counter 0x013D27B0.
// Layout (retail, Ego_r.pdb less 4 bytes per STLport container before the list, 8 for the 4-byte list): States map +0x24
//   (header, count), OpenList heap vector +0x60, GoalNode +0x6c, GoalPosition +0x70, NodeAskedToGetTo +0x78,
//   PosAskedToGetTo +0x7c, Route +0x8c, LastWayPointVisited +0x98, StringLeadPosition +0xa0, flags +0xbd..+0xc3.
//   CNavWayPoint is a packed 10-byte {C2DVector, type, flag}; the route element a packed 9-byte pair.
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
    C2DVector(const struct C3DVector& v);
    void operator=(const struct C3DVector& v);
};
struct C3DVector {
    float X, Y, Z;
};
inline C2DVector::C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
inline void C2DVector::operator=(const C3DVector& v) { X = v.X; Y = v.Y; }
#pragma pack(push, 1)
struct SRoutePoint {                               // std::pair<C2DVector, unsigned char> (9 bytes)
    C2DVector     first;
    unsigned char second;
};
struct CNavWayPoint {                              // 10 bytes
    C2DVector     Position;
    unsigned char Type;
    bool          Valid;
    CNavWayPoint(const SRoutePoint& p) : Position(p.first), Type(p.second), Valid(true) {}
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
struct CWayPointList {                             // STLport list<CNavWayPoint>
    SWayPointNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        __distance(_M_node->_M_next, _M_node, n);
        return n;
    }
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
    void push_back(const CNavWayPoint& x)
    {
        SWayPointNode* pos = _M_node;
        SWayPointNode* tmp = (SWayPointNode*)malloc(sizeof(SWayPointNode));
        new (&tmp->_M_data) CNavWayPoint(x);
        SWayPointNode* prev = pos->_M_prev;
        tmp->_M_next = pos;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
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
struct CAStarState;
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
struct CANavQuadTreeNode;
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
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetLayerAt(const C3DVector& position);   // slot 16 (+0x40)
};
struct CSteeringAgent {
    void SetGoal(const CNavWayPoint& goal, const CNavWayPoint& next);   // 0x00A20970
};
extern long g_AStarSearchesInProgress;             // 0x013D27B0
struct CNavigatorAStar {
    // +0x00 vptr
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;          // +0x18
    bool                         Interrupted;      // +0x19
    CNavigatorManager*           NavigationManager;// +0x1c
    CSteeringAgent*              SteeringAgent;    // +0x20
    CStateMap                    States;           // +0x24
    unsigned char                _pad30[0x30];     // +0x30 BlockedStartNode / BlockedEndNode / BlockedNodeAskedToGetTo
    CStatePtrVector              OpenList;         // +0x64
    CANavQuadTreeNode*           GoalNode;         // +0x6c
    C2DVector                    GoalPosition;     // +0x70
    CANavQuadTreeNode*           NodeAskedToGetTo; // +0x78
    C3DVector                    PosAskedToGetTo;  // +0x7c
    CANavQuadTreeNode*           LastNavigableNodeVisited;   // +0x88
    CWayPointList                Route;            // +0x8c
    SWayPointNode*               CullFront;        // +0x90
    SWayPointNode*               CullBack;         // +0x94
    C2DVector                    LastWayPointVisited;        // +0x98
    C2DVector                    StringLeadPosition;         // +0xa0
    C3DVector                    LastKnownNavPos;  // +0xa8
    float                        LastKnownNavSpeed;// +0xb4
    long                         LastKnownNavLayer;// +0xb8
    bool                         HeadingForActionPoint;      // +0xbc
    bool                         Initialised;      // +0xbd
    bool                         GotRoute;         // +0xbe
    bool                         SlowedDownForLastPoint;     // +0xbf
    bool                         Culled;           // +0xc0
    bool                         PartWayThroughAStar;        // +0xc1
    unsigned char                NoOfClockwiseTurns;         // +0xc2
    unsigned char                NoOfAntiClockwiseTurns;     // +0xc3
    void CalcWayPointWidths(const C3DVector& from);          // 0x00A1DE30
    virtual void InitialiseFromPreCalculatedRoute(const C3DVector& from, const CRoutePointVector& route, const C3DVector& to);
};

void CNavigatorAStar::InitialiseFromPreCalculatedRoute(const C3DVector& from, const CRoutePointVector& route, const C3DVector& to)
{
    if (States._M_node_count != 0)
        --g_AStarSearchesInProgress;
    NavigationManager->Navigators.remove_if(CIsNavigator((const CNavigatorBaseLayout*)this));
    States.clear();
    Route.clear();
    OpenList.clear();
    GotRoute = true;
    Culled = true;
    PartWayThroughAStar = false;
    for (unsigned long i = 0; i < route.size(); ++i)
        Route.push_back(CNavWayPoint(route[i]));
    CalcWayPointWidths(from);
    SteeringAgent->SetGoal(Route._M_node->_M_next->_M_data,
                           Route.size() > 1 ? Route._M_node->_M_next->_M_next->_M_data : Route._M_node->_M_next->_M_data);
    StringLeadPosition = route[0].first;
    SlowedDownForLastPoint = true;
    LastWayPointVisited = route[0].first;
    {
        C2DVector to2D(to);
        GoalNode = NavigationManager->GetNodeAt(to2D, PInterface->GetLayerAt(to));
    }
    GoalPosition = to;
    NodeAskedToGetTo = GoalNode;
    PosAskedToGetTo = to;
    Initialised = true;
    OnRoute = true;
    Interrupted = false;
    NoOfClockwiseTurns = 0;
    NoOfAntiClockwiseTurns = 0;
}

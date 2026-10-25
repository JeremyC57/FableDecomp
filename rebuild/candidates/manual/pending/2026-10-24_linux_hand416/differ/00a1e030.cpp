// CNavigatorAStar::UpdateGoal(const C3DVector& pos, C2DVector previous, C2DVector next) -- shrink the agent's best side clearances by the distance travelled (re-measuring the navigable width when either falls below five radii), then aim the agent at the next one or two way points
// Grade: DIFFER(216v217) -- one instruction short: retail copies the cached steering agent (`mov eax,edi`) and loads the navigator interface through eax before the PeekNavigatorRadius call; ours loads it straight from edi, and the 2 bytes saved let VC7.1 pad the Route-size loop head with a `nop`. Everything else (x87 schedule, compare/CSE shape, SetGoal tails) is byte-identical.
// Leaf: ?UpdateGoal@CNavigatorAStar@@QAEXABVC3DVector@@VC2DVector@@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1E030, 217 bytes, `ret 14h`. Only caller 0x00A1EF1E (the big CNavigatorAStar
//   move/route update 0x00A1E5C0, passing two C2DVector values by value). Catalog label "waypoint_update_with_width_calc".
// Identity: Ego_r.pdb `?UpdateGoal@CNavigatorAStar@@QAEXABVC3DVector@@VC2DVector@@1@Z`. Callees: CSteeringAgent's
//   NavigatorInterface slot 6 (+0x18) = PeekNavigatorRadius(bool) (Ego_r.pdb CObstacle/CGround overrides 0x004DF3EB /
//   0x006CDA10), CNavigatorAStar::CalculateNavigableWidthOfLine(const C3DVector&) 0x00A1DE30, CSteeringAgent::SetGoal
//   0x00A20970. Retail layout: CNavigatorBase::SteeringAgent +0x20, Route +0x8C (STLport one-word list); CSteeringAgent
//   BestDistLeft/Right +0x40/+0x44 (Ego_r.pdb order with one-word STLport lists).
// Levers found: the clearance refs (`float& bestLeft/bestRight`) stop VC7.1 CSE-ing `bestRight - distance` into the
//   else arm, which is what keeps the 5-radius limit in st(1) instead of a frame slot; the SetGoal tail is one call with
//   the `size() > 1 ? *++begin() : front()` ternary (tail-duplicated, `lea ecx,[eax+8]` in the short arm).
#include <math.h>
#pragma intrinsic(sqrt)

class C3DVector { public: float X, Y, Z; };

class C2DVector {
public:
    float X, Y;
};

class CNavWayPoint {                                    // 0x0C bytes
public:
    C2DVector     Position;                             // +0x00
    unsigned char NavLayer;                             // +0x08
    bool          Removable;                            // +0x09
};

class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual float PeekNavigatorRadius(bool padded) const;   // slot 6 (+0x18)
};

class CSteeringAgent {
public:
    const CNavigatorExternalInterface* NavigatorInterface;   // +0x00
    unsigned char _pad04[0x3C];                          // DummyObstacle .. Steer
    float         BestDistLeft;                          // +0x40
    float         BestDistRight;                         // +0x44
    void SetGoal(const C2DVector& goal, const C2DVector& nextGoal);   // 0x00A20970
};

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _InputIterator>
inline int distance(const _InputIterator& __first, const _InputIterator& __last)
{
    int __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}
template <class _Tp> class list {                       // STLport: one header pointer
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    unsigned int size() { return distance(begin(), end()); }
    _Tp& front() { return *begin(); }
};
}

class CNavigatorAStar {
public:
    void*           VTable;                             // +0x00
    unsigned char   _pad04[0x1C];                       // CClass registration, PInterface, OnRoute, NavigationManager
    CSteeringAgent* SteeringAgent;                      // +0x20 (CNavigatorBase)
    unsigned char   _pad24[0x68];                       // States .. LastNavigableNodeVisited
    std::list<CNavWayPoint> Route;                     // +0x8C
    void CalculateNavigableWidthOfLine(const C3DVector& pos);   // 0x00A1DE30
    void UpdateGoal(const C3DVector& pos, C2DVector previous, C2DVector next);
};

void CNavigatorAStar::UpdateGoal(const C3DVector& pos, C2DVector previous, C2DVector next)
{
    float dx = next.X - previous.X;
    float dy = next.Y - previous.Y;
    float distance = (float)sqrt(dx * dx + dy * dy);
    CSteeringAgent* agent = SteeringAgent;
    float limit = agent->NavigatorInterface->PeekNavigatorRadius(false) * 5.0f;
    float& bestLeft = agent->BestDistLeft;
    float& bestRight = agent->BestDistRight;
    float left = bestLeft - distance;
    if (left < limit || bestRight - distance < limit)
        CalculateNavigableWidthOfLine(pos);
    else {
        bestLeft = left;
        bestRight -= distance;
    }
    SteeringAgent->SetGoal(Route.front().Position, (Route.size() > 1 ? *++Route.begin() : Route.front()).Position);
}

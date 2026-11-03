// CNavigatorAStar::UpdateGoal(const C3DVector& position, C2DVector oldGoal, C2DVector newGoal) -- the goal moved by |newGoal - oldGoal|: shrink the steering agent's two best side distances by that much, or re-measure the way-point widths from the current position when either would drop below five navigator radii; then hand the agent the first route point and the next one (the first again when the route has one point)
// Grade: RELOCATION_MATCH (217 bytes; needs the engine/C2DVector.h FABLE_RETAIL_C2DVECTOR overlay in hdr/)
// Flags: none
// Leaf: ?UpdateGoal@CNavigatorAStar@@QAEXABVC3DVector@@VC2DVector@@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1E030, 217 bytes, thiscall `ret 14h` (the reference plus two by-value C2DVectors).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?UpdateGoal@CNavigatorAStar@@QAEXABVC3DVector@@VC2DVector@@1@Z -- (1) the 20 bytes of
//   arguments are exactly one reference and two C2DVectors, the reference forwarded to CalcWayPointWidths(const C3DVector&); (2) Ego_r
//   donor order puts UpdateGoal right after CalculateNavigableWidthOfLine, as retail 0x00A1DF60 -> 0x00A1E030; (3) its body uses the
//   CSteeringAgent goal API (SetGoal) and the best-distance pair UpdateGoal maintains. The manifest's "waypoint_update_with_width_calc"
//   is descriptive.
// Callees: CNavigatorAStar::CalcWayPointWidths(const C3DVector&) 0x00A1DE30; CSteeringAgent::SetGoal(const C2DVector&, const C2DVector&)
//   0x00A20970 (both PDBs; the landed binding's CNavWayPoint spelling is wrong, NAME_FIXES.tsv); CNavigatorExternalInterface virtual
//   slot 6 PeekNavigatorRadius(bool) const (+0x18) through the agent's inline GetRadius. Constant 5.0f @ 0x012462A8.
// Layout: CNavigatorBase from the retail branch of engine/CNavigatorBase.h (SteeringAgent +0x20); CNavigatorAStar::Route +0x8c is the
//   retail 4-byte STLport list<CNavWayPoint> (as InitialiseFromPreCalculatedRoute 0x00A1E370); CSteeringAgent (no engine header) is
//   Ego_r.pdb with retail's one-word lists (BestDistLeft +0x40, BestDistRight +0x44, as the landed CalcWayPointWidths 0x00A1DE30).
// Shape (what matched): the two best-distance references bound before the length and the radius call (retail keeps the agent in
//   edi and re-derives GetRadius's `this` with `mov eax,edi`; a named agent pointer or binding them after the call does not); the
//   length is C2DVector::GetMagnitude through the float sqrt overload (engine/C2DVector.h retail branch); the next goal is one
//   ternary of references, which VC7.1 tail-duplicates into two SetGoal calls with separately addressed arguments (if/else of two
//   calls CSEs the front() address into one push register). Route.size() is STLport's distance loop.
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CNAVIGATORBASE
#include "engine/CNavigatorBase.h"
class CNavWayPoint {                                   // Ego_r.pdb: C2DVector Position +0, NavLayer +8, Removable +9 (10 bytes)
public:
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::pointer pointer;
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _InputIterator>
inline int __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    int __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it; ++__n;
    }
    return __n;
}
template <class _InputIterator>
inline int distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, bidirectional_iterator_tag());
}
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    unsigned int size() const { unsigned int __result = distance(begin(), end()); return __result; }   // STLport 4.6 _list.h
    _Tp& front() { return *begin(); }
};
}
class CNavigatorExternalInterface {                    // CGroundNavigatorExternalInterface vtable 0x0125EC9C slots 0..6
public:
    virtual bool IsMovementBlockedAt(const C3DVector&, const C3DVector&, bool&, bool&) const = 0;
    virtual float GetTerrainDesirabilityAt(const C3DVector&) const = 0;
    virtual float GetTerrainHeightAt(const C3DVector&) const = 0;
    virtual float GetThemeHeightAt(const C3DVector&) const = 0;
    virtual float FindPosZAt(const C3DVector&, bool) const = 0;
    virtual float FindPosZAtIncludingThemes(const C3DVector&, bool) const = 0;
    virtual float PeekNavigatorRadius(bool physical) const = 0;                       // slot 6 (+0x18)
};
class CSteeringAgent {
public:
    CNavigatorExternalInterface* NavigatorInterface;          // +0x00
    bool                         DummyObstacle;               // +0x04
    float                        RepulsionMultiplier;         // +0x08
    void*                        SteeringBehaviours;          // +0x0C list<CSB*> (STLport, one word)
    void*                        SteeringBehaviourInputs;     // +0x10 list<CSBInput*>
    void*                        SteeringBehaviourConstraints;// +0x14 list<CSBConstraint*>
    float                        CurrentMaxSpeed;             // +0x18
    float                        CurrentSpeed;                // +0x1C
    C2DVector                    Goal;                        // +0x20
    C2DVector                    NextGoal;                    // +0x28
    int                          TimeToReacquireGoal;         // +0x30
    float                        DistCutoff;                  // +0x34
    C2DVector                    Steer;                       // +0x38
    float                        BestDistLeft;                // +0x40
    float                        BestDistRight;               // +0x44
    // FableWin.pdb inline accessors
    float GetRadius(bool physical) const { return NavigatorInterface->PeekNavigatorRadius(physical); }   // ?GetRadius@CSteeringAgent@@QBEM_N@Z
    float& GetBestDistLeft() { return BestDistLeft; }                                                    // ?GetBestDistLeft@CSteeringAgent@@QAEAAMXZ
    float& GetBestDistRight() { return BestDistRight; }                                                  // ?GetBestDistRight@CSteeringAgent@@QAEAAMXZ
    void SetGoal(const C2DVector& goal, const C2DVector& nextGoal);                                      // 0x00A20970
};
class CNavigatorAStar : public CNavigatorBase {
public:
    unsigned char                _pad24[0x8c - 0x24];        // +0x24 States .. LastNavigableNodeVisited
    std::list<CNavWayPoint>      Route;                       // +0x8c
    void CalcWayPointWidths(const C3DVector& position);       // 0x00A1DE30
    void UpdateGoal(const C3DVector& position, C2DVector oldGoal, C2DVector newGoal);
};

void CNavigatorAStar::UpdateGoal(const C3DVector& position, C2DVector oldGoal, C2DVector newGoal)
{
    float& bestDistLeft = SteeringAgent->GetBestDistLeft();
    float& bestDistRight = SteeringAgent->GetBestDistRight();
    float distance = (newGoal - oldGoal).GetMagnitude();
    float minWidth = SteeringAgent->GetRadius(false) * 5.0f;
    if (bestDistLeft - distance < minWidth || bestDistRight - distance < minWidth)
        CalcWayPointWidths(position);
    else {
        bestDistLeft -= distance;
        bestDistRight -= distance;
    }
    const C2DVector& nextGoal = Route.size() > 1 ? (*++Route.begin()).Position : Route.front().Position;
    SteeringAgent->SetGoal(Route.front().Position, nextGoal);
}
// CNavigatorAStar::Initialise(const CANavQuadTreeNode* startNode, const CANavQuadTreeNode* goalNode, const C2DVector& startPos, const C2DVector& goalPos, const CANavQuadTreeNode* nodeAskedToGetTo, const C3DVector& posAskedToGetTo, bool useAStar, bool urgent, float priority) -- reset the navigator (state map, way-point list, open list, queue entry) and either start an A* search from the start node or take the direct two-point route, then record the goal and the position asked for
// Grade: DIFFER(992v992) -- one instruction of x87 scheduling: retail stores the heuristic TotalCost (`fstp [esp+2Ch]`) between the loads of CAStarState::NoOfStates and NoOfStatesOnOpenList for the make_pair copy, ours after both loads; every other byte matches
// Leaf: ?Initialise@CNavigatorAStar@@AAEXPBVCANavQuadTreeNode@@0ABVC2DVector@@10ABVC3DVector@@_N3M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1CEB0, 992 bytes, thiscall `ret 24h`. Caller 0x00A1F1D0.
// Identity: Ego_r.pdb (0xB87CD0) and FableWin.pdb `?Initialise@CNavigatorAStar@@AAEXPBVCANavQuadTreeNode@@0ABVC2DVector@@10ABVC3DVector@@_N3M@Z`;
//   nine dword arguments (`ret 24h`), the reset block is the landed InitialiseFromPreCalculatedRoute 0x00A1E370 twin,
//   the A* start inserts make_pair(node, state) into the state map and pushes it on the open list. The catalog
//   "AStar_initialize_search" is descriptive. Bool/float parameter names are descriptive (AddNavigatorToQueue takes them).
// Callees: CNavigatorQueueManager::RemoveNavigatorFromQueue 0x00A155C0 / AddNavigatorToQueue 0x00A154D0 (manager +0x2c),
//   _Rb_tree::_M_erase 0x00A21640 / insert_unique 0x00A221B0, pair converting ctor 0x00A20240, CPriorityQueue::Push
//   0x00A22460, Distance2D 0x009E2460, CANavQuadTreeNode::operator== 0x00A763F0, CNavBlockedQuadTreeNode::operator=
//   0x00A76240, CSteeringAgent::SetGoal 0x00A20970, malloc 0x00BFEA0E, free 0x00BFEA14, memmove 0x00BFEAE6.
#include <string.h>
#include <stdlib.h>
inline void* operator new(unsigned int, void* where) { return where; }

class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
class C3DVector { public:
    float X, Y, Z;
};
float __fastcall Distance2D(const C2DVector& a, const C2DVector& b);   // 0x009E2460

#pragma pack(push, 1)
class CNavWayPoint { public:                       // 10 bytes
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
    CNavWayPoint(const C2DVector& position, unsigned char navLayer, bool removable)
        : Position(position), NavLayer(navLayer), Removable(removable) {}
};
#pragma pack(pop)

enum ENavQuadTreeNodeType { };
class CANavQuadTreeNode { public:
    virtual ~CANavQuadTreeNode();
    virtual void VSlot01();
    virtual void VSlot02();
    virtual void VSlot03();
    virtual void VSlot04();
    virtual void VSlot05();
    virtual ENavQuadTreeNodeType GetNodeType() const;   // slot 6 (+0x18)
    C2DVector     Position;        // +0x04
    unsigned char QuadTreeLevel;   // +0x0c
    unsigned char NavLayer;        // +0x0d
    bool          LeafNode;        // +0x0e
    bool          Blocked;         // +0x0f
    bool operator==(const CANavQuadTreeNode& other) const;   // 0x00A763F0
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode { public:
    const CNavBlockedQuadTreeNode& operator=(const CNavBlockedQuadTreeNode& other);   // 0x00A76240
};

class CNavigatorAStar;
class CNavigatorBase;

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p);   // 0x00A20240 (explicitly instantiated elsewhere)
};
template <class _T1, class _T2> inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y) { return pair<_T1, _T2>(__x, __y); }
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_node<_Value>* _Link_type;
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
};
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Value _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Value value_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                                // 0x00A21640
public:
    pair<iterator, bool> insert_unique(const value_type& __x);    // 0x00A221B0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
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
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    unsigned int size() const { return _M_t._M_node_count; }
    void clear() { _M_t.clear(); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
};
}

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
class CAStarStateTotalGreaterThan { public: CAStarStateTotalGreaterThan() {} };
template <class T> class CPriorityQueue { public:
    T* _M_start;                                      // Heap (STLport vector<T>)
    T* _M_finish;
    T* _M_end_of_storage;
    template <class C> void Push(const T& element, const C& compare);   // 0x00A22460
    T* erase(T* first, T* last)
    {
        T* i = __copy_ptrs(last, _M_finish, first, __true_type());
        _M_finish = i;
        return first;
    }
    void Clear() { erase(_M_start, _M_finish); }
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
    void insert(SWayPointNode* pos, const CNavWayPoint& x)
    {
        SWayPointNode* tmp = (SWayPointNode*)malloc(sizeof(SWayPointNode));
        new (&tmp->_M_data) CNavWayPoint(x);
        SWayPointNode* prev = pos->_M_prev;
        tmp->_M_next = pos;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
    void push_front(const CNavWayPoint& x) { insert(_M_node->_M_next, x); }
    void push_back(const CNavWayPoint& x) { insert(_M_node, x); }
};

class CNavigatorQueueManager { public:
    void RemoveNavigatorFromQueue(const CNavigatorBase& navigator);                       // 0x00A155C0
    void AddNavigatorToQueue(const CNavigatorBase& navigator, bool urgent, float priority);   // 0x00A154D0
};
class CNavigatorManager { public:
    unsigned char          _pad00[0x2c];
    CNavigatorQueueManager QueueManager;           // +0x2c
    CNavigatorQueueManager& GetNavigatorQueueManager() { return QueueManager; }
};
class CSteeringAgent { public:
    void SetGoal(const CNavWayPoint& goal, const CNavWayPoint& next);   // 0x00A20970
};
extern long g_AStarSearchesInProgress;             // 0x013D27B0

class CNavigatorExternalInterface;
class CNavigatorBase { public:
    virtual ~CNavigatorBase();
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;          // +0x18
    bool                         Interrupted;      // +0x19
    CNavigatorManager*           NavigationManager;// +0x1c
    CSteeringAgent*              SteeringAgent;    // +0x20
};

class CNavigatorAStar : public CNavigatorBase {
public:
    class CAStarState { public:                    // 0x14 bytes
        CAStarState*             ParentState;      // +0x00
        const CANavQuadTreeNode* Node;             // +0x04
        float                    CostToThisState;  // +0x08
        float                    TotalCost;        // +0x0c
        bool                     OnOpenList;       // +0x10
        bool                     OnClosedList;     // +0x11
        static long NoOfStates;                    // 0x013D27B8
        static long NoOfStatesOnOpenList;          // 0x013D27BC
        static long NoOfStatesOnClosedList;        // 0x013D27C0
        CAStarState(const CANavQuadTreeNode* node, CAStarState* parent)
            : ParentState(parent), Node(node), CostToThisState(0.0f), TotalCost(0.0f), OnOpenList(false), OnClosedList(false)
        {
            ++NoOfStates;
        }
        CAStarState(const CAStarState& other)
            : ParentState(other.ParentState), Node(other.Node), CostToThisState(other.CostToThisState),
              TotalCost(other.TotalCost), OnOpenList(other.OnOpenList), OnClosedList(other.OnClosedList)
        {
            ++NoOfStates;
            if (OnOpenList)
                ++NoOfStatesOnOpenList;
            if (OnClosedList)
                ++NoOfStatesOnClosedList;
        }
        ~CAStarState()
        {
            --NoOfStates;
            if (OnOpenList)
                --NoOfStatesOnOpenList;
            if (OnClosedList)
                --NoOfStatesOnClosedList;
        }
        void SetOnOpen()
        {
            if (!OnOpenList)
                ++NoOfStatesOnOpenList;
            if (OnClosedList)
                --NoOfStatesOnClosedList;
            OnOpenList = true;
            OnClosedList = false;
        }
        void SetTotalCost(float cost) { TotalCost = cost; }
    };
    std::map<long, CAStarState>    States;                    // +0x24
    CNavBlockedQuadTreeNode        BlockedStartNode;          // +0x30
    CNavBlockedQuadTreeNode        BlockedEndNode;            // +0x40
    CNavBlockedQuadTreeNode        BlockedNodeAskedToGetTo;   // +0x50
    CPriorityQueue<CAStarState*>   OpenList;                  // +0x60
    const CANavQuadTreeNode*       GoalNode;                  // +0x6c
    C2DVector                      GoalPosition;              // +0x70
    const CANavQuadTreeNode*       NodeAskedToGetTo;          // +0x78
    C3DVector                      PosAskedToGetTo;           // +0x7c
    const CANavQuadTreeNode*       LastNavigableNodeVisited;  // +0x88
    CWayPointList                  Route;                     // +0x8c
    SWayPointNode*                 CullFront;                 // +0x90
    SWayPointNode*                 CullBack;                  // +0x94
    C2DVector                      LastWayPointVisited;       // +0x98
    C2DVector                      StringLeadPosition;        // +0xa0
    C3DVector                      LastKnownNavPos;           // +0xa8
    float                          LastKnownNavSpeed;         // +0xb4
    long                           LastKnownNavLayer;         // +0xb8
    bool                           HeadingForActionPoint;     // +0xbc
    bool                           Initialised;               // +0xbd
    bool                           GotRoute;                  // +0xbe
    bool                           SlowedDownForLastPoint;    // +0xbf
    bool                           Culled;                    // +0xc0
    bool                           PartWayThroughAStar;       // +0xc1
    unsigned char                  NoOfClockwiseTurns;        // +0xc2
    unsigned char                  NoOfAntiClockwiseTurns;    // +0xc3
private:
    float GetHeuristicCost(const CANavQuadTreeNode* from, const CANavQuadTreeNode* to)
    {
        return Distance2D(from->Position, to->Position) * 2.0f;
    }
    bool IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b)
    {
        if (a == b)
            return true;
        if (a && b && *a == *b)
            return true;
        return false;
    }
    void Initialise(const CANavQuadTreeNode* startNode, const CANavQuadTreeNode* goalNode, const C2DVector& startPos,
                    const C2DVector& goalPos, const CANavQuadTreeNode* nodeAskedToGetTo, const C3DVector& posAskedToGetTo,
                    bool useAStar, bool urgent, float priority);
};

void CNavigatorAStar::Initialise(const CANavQuadTreeNode* startNode, const CANavQuadTreeNode* goalNode, const C2DVector& startPos,
                                 const C2DVector& goalPos, const CANavQuadTreeNode* nodeAskedToGetTo, const C3DVector& posAskedToGetTo,
                                 bool useAStar, bool urgent, float priority)
{
    if (States.size() != 0)
        --g_AStarSearchesInProgress;
    NavigationManager->GetNavigatorQueueManager().RemoveNavigatorFromQueue(*this);
    States.clear();
    Route.clear();
    OpenList.Clear();
    CullFront = Route._M_node->_M_next;
    CullBack = Route._M_node;
    if (useAStar && !IsEqual(startNode, goalNode)) {
        CAStarState startState(startNode, 0);
        startState.SetOnOpen();
        startState.SetTotalCost(GetHeuristicCost(startNode, goalNode));
        std::pair<std::map<long, CAStarState>::iterator, bool> result = States.insert(std::make_pair((long)startNode, startState));
        OpenList.Push(&result.first->second, CAStarStateTotalGreaterThan());
        GotRoute = false;
        Culled = false;
        PartWayThroughAStar = true;
        ++g_AStarSearchesInProgress;
        NavigationManager->GetNavigatorQueueManager().AddNavigatorToQueue(*this, urgent, priority);
    }
    else {
        GotRoute = true;
        Culled = true;
        PartWayThroughAStar = false;
        Route.push_front(CNavWayPoint(startPos, startNode->NavLayer, true));
        Route.push_back(CNavWayPoint(goalPos, goalNode->NavLayer, true));
        SteeringAgent->SetGoal(Route._M_node->_M_prev->_M_data,
                               Route.size() > 1 ? Route._M_node->_M_next->_M_next->_M_data : Route._M_node->_M_next->_M_data);
    }
    StringLeadPosition = startPos;
    SlowedDownForLastPoint = true;
    LastWayPointVisited = startPos;
    GoalNode = goalNode;
    GoalPosition = goalPos;
    if (nodeAskedToGetTo->GetNodeType() != 2)
        NodeAskedToGetTo = nodeAskedToGetTo;
    else {
        BlockedNodeAskedToGetTo = *(const CNavBlockedQuadTreeNode*)nodeAskedToGetTo;
        NodeAskedToGetTo = &BlockedNodeAskedToGetTo;
    }
    PosAskedToGetTo = posAskedToGetTo;
    Initialised = true;
    OnRoute = true;
    Interrupted = false;
    NoOfClockwiseTurns = 0;
    NoOfAntiClockwiseTurns = 0;
}

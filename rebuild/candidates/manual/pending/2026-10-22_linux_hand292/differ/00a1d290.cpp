// Grade: DIFFER(817v817) -- same size; residue: (1) frame-slot packing -- retail shares the find key's word (esp+10h) with the relaxed cost and gives the neighbour iterator/find result/neighbour state 14h/18h/1Ch, the lane shares the key with the neighbour state (14h) and shifts the others by 4; (2) the budget-exhausted 'PartWayThroughAStar = true' block -- retail stores it before the shared pop/xor al,al epilogue at the very end, the lane pops first and places it before the goal block; (3) register choice for the goal position pointer (retail mov ecx/lea edx, lane mov edx/add edx) and the TotalCost fadd/fstp scheduled earlier around the insert pushes. Fixed on the way (854v817 -> 817v817): same-TU auto_inline(off) stand-ins for the open list's UpdateElement/Push, a user-declared ctor on the empty comparator (no zero-byte store; its temporaries home in the live bool argument slot like retail), pop_heap through by-value first/last. Tried: early-continue vs if/else for the find result, inline vs helper lookup, named key, widened neighbourState scope, break/continue/do-while budget exits, goal test as else-branch, distance wrappers, find/insert stand-ins.
// CNavigatorAStar::CalculateRoute(bool ignorePreferability) -- run the A* search for this frame: pop the cheapest open state, finish the route when it reaches the goal node, otherwise relax or open a state for each unblocked neighbour, then close it; stops (part way) when the manager's per-frame node budget runs out
// Leaf: ?CalculateRoute@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1D290, 817 bytes (`ret 4`).
// Callees: CNavigatorQueueManager::IsNavigatorAllowedToCalculateRoute 0x00A15420, the open list's __adjust_heap 0x00A20750
//   (`__fastcall`), UpdateElement 0x00A21C30 and Push 0x00A22460, the state map's find 0x00A216A0 and insert 0x00A221B0,
//   CANavQuadTreeNode::operator== 0x00A763F0, the node's slot 2 GetNeighbours (+8), GetCostFromNodeToNode 0x00A1A0A0, the
//   2D distance helper 0x009E2460 (`__fastcall`), ConstructFinalRoute 0x00A1CC30; the A* statistics 0x013D27B0..0x013D27CC
//   and the countdown 0x0139713C.
// Layout (retail; Ego_r.pdb less 4 bytes from the 12-byte STLport map on): CNavigatorAStar NavigationManager +0x1c, States
//   +0x24, OpenList +0x60, GoalNode +0x6c, GotRoute +0xbe, PartWayThroughAStar +0xc1; CAStarState {ParentState +0,
//   Node +4, CostToThisState +8, TotalCost +0xc, OnOpenList +0x10, OnClosedList +0x11}; CNavigatorManager
//   NumberOfNodeExaminationsLeftThisFrame +0x20, QueueManager +0x2c; CSmallVector {Data +0, signed char Size +4}.
struct C2DVector {
    float X, Y;
};
float __fastcall Distance2D(const C2DVector& a, const C2DVector& b);   // 0x009E2460
struct CANavQuadTreeNode;
struct CNeighbourList {                                  // CSmallVector<const CANavQuadTreeNode*, 8>
    const CANavQuadTreeNode** Data;                      // +0x00
    signed char               Size;                      // +0x04
    const CANavQuadTreeNode** begin() const { return Data; }
    const CANavQuadTreeNode** end() const { return Data + Size; }
};
struct CANavQuadTreeNode {
    virtual void v00();
    virtual void v01();
    virtual const CNeighbourList* GetNeighbours() const; // slot 2 (+8)
    C2DVector     Position;                              // +0x04
    unsigned char QuadTreeLevel;                         // +0x0c
    unsigned char NavLayer;                              // +0x0d
    bool          LeafNode;                              // +0x0e
    bool          Blocked;                               // +0x0f
    bool operator==(const CANavQuadTreeNode& other) const;   // 0x00A763F0
};
extern long g_AStarSearchesInProgress;                   // 0x013D27B0
extern long g_AStarStat_b4;                              // 0x013D27B4
extern long g_AStarStateCount;                           // 0x013D27B8
extern long g_AStarOpenCount;                            // 0x013D27BC
extern long g_AStarClosedCount;                          // 0x013D27C0
extern long g_AStarMaxStateCount;                        // 0x013D27C4
extern long g_AStarClosedAtMax;                          // 0x013D27C8
extern long g_AStarOpenAtMax;                            // 0x013D27CC
extern long g_AStarCountdown;                            // 0x0139713C
struct CAStarState {
    CAStarState*             ParentState;                // +0x00
    const CANavQuadTreeNode* Node;                       // +0x04
    float                    CostToThisState;            // +0x08
    float                    TotalCost;                  // +0x0c
    bool                     OnOpenList;                 // +0x10
    bool                     OnClosedList;               // +0x11
    CAStarState() : OnOpenList(false), OnClosedList(false) { ++g_AStarStateCount; }
    CAStarState(const CAStarState& o)
        : ParentState(o.ParentState), Node(o.Node), CostToThisState(o.CostToThisState), TotalCost(o.TotalCost),
          OnOpenList(o.OnOpenList), OnClosedList(o.OnClosedList)
    {
        ++g_AStarStateCount;
        if (OnOpenList)
            ++g_AStarOpenCount;
        if (OnClosedList)
            ++g_AStarClosedCount;
    }
    ~CAStarState()
    {
        --g_AStarStateCount;
        if (OnOpenList)
            --g_AStarOpenCount;
        if (OnClosedList)
            --g_AStarClosedCount;
    }
    void SetOnOpenList()
    {
        OnOpenList = true;
        ++g_AStarOpenCount;
    }
    void SetOnClosedList()
    {
        if (OnOpenList)
            --g_AStarOpenCount;
        OnClosedList = true;
        OnOpenList = false;
        ++g_AStarClosedCount;
    }
};
struct CAStarStateTotalGreaterThan {
    CAStarStateTotalGreaterThan() {}
    bool operator()(const CAStarState* a, const CAStarState* b) const { return a->TotalCost > b->TotalCost; }
};
void __fastcall __adjust_heap(CAStarState** first, int holeIndex, int len, CAStarState* value, CAStarStateTotalGreaterThan comp);   // 0x00A20750
inline void __pop_heap(CAStarState** first, CAStarState** last, CAStarState** result, CAStarState* value, CAStarStateTotalGreaterThan comp)
{
    *result = *first;
    __adjust_heap(first, 0, (int)(last - first), value, comp);
}
inline void pop_heap(CAStarState** first, CAStarState** last, CAStarStateTotalGreaterThan comp)
{
    __pop_heap(first, last - 1, last - 1, *(last - 1), comp);
}
struct CPriorityQueue {                                  // CPriorityQueue<CAStarState*>: STLport vector heap
    CAStarState** _M_start;
    CAStarState** _M_finish;
    CAStarState** _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    void UpdateElement(CAStarState* const& element, const CAStarStateTotalGreaterThan& comp);   // 0x00A21C30
    void Push(CAStarState* const& element, const CAStarStateTotalGreaterThan& comp);            // 0x00A22460
    CAStarState* Pop(CAStarStateTotalGreaterThan comp)
    {
        CAStarState* top = *_M_start;
        pop_heap(_M_start, _M_finish, comp);
        --_M_finish;
        return top;
    }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
struct SStateTreeNode {                                  // _Rb_tree_node<pair<const long, CAStarState> >
    long                          _M_color;
    SStateTreeNode*               _M_parent;
    SStateTreeNode*               _M_left;
    SStateTreeNode*               _M_right;
    pair<const long, CAStarState> _M_value_field;        // +0x10 (state +0x14)
};
struct CStateIterator {
    SStateTreeNode* _M_node;
    CStateIterator() {}
    CStateIterator(SStateTreeNode* n) : _M_node(n) {}
    CStateIterator(const CStateIterator& o) : _M_node(o._M_node) {}
    pair<const long, CAStarState>* operator->() const { return &_M_node->_M_value_field; }
    bool operator==(const CStateIterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const CStateIterator& o) const { return _M_node != o._M_node; }
};
struct CStateMap {                                       // STLport map<long, CAStarState>
    SStateTreeNode* _M_header;
    unsigned long   _M_node_count;
    unsigned long   _M_key_compare;
    CStateIterator end() { return CStateIterator(_M_header); }
    CStateIterator find(const long& key);                // 0x00A216A0
    pair<CStateIterator, bool> insert(const pair<const long, CAStarState>& value);   // 0x00A221B0
};
struct CNavigatorBase;
struct CNavigatorQueueManager {
    bool IsNavigatorAllowedToCalculateRoute(const CNavigatorBase& navigator) const;   // 0x00A15420
};
struct CNavigatorManager {
    unsigned char          _pad00[0x20];
    long                   NumberOfNodeExaminationsLeftThisFrame;   // +0x20
    unsigned char          _pad24[0x8];
    CNavigatorQueueManager QueueManager;                 // +0x2c
};
struct CNavigatorAStar {
    virtual void v00();
    unsigned char            _pad04[0x18];
    CNavigatorManager*       NavigationManager;          // +0x1c
    unsigned char            _pad20[0x4];
    CStateMap                States;                     // +0x24
    unsigned char            _pad30[0x30];
    CPriorityQueue           OpenList;                   // +0x60
    const CANavQuadTreeNode* GoalNode;                   // +0x6c
    unsigned char            _pad70[0xbe - 0x70];
    bool                     GotRoute;                   // +0xbe
    bool                     SlowedDownForLastPoint;     // +0xbf
    bool                     Culled;                     // +0xc0
    bool                     PartWayThroughAStar;        // +0xc1
    float GetCostFromNodeToNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode* to, bool ignorePreferability) const;   // 0x00A1A0A0
    void ConstructFinalRoute(const CAStarState* goal);   // 0x00A1CC30
    CAStarState* GetState(const CANavQuadTreeNode* node)
    {
        CStateIterator it = States.find((long)node);
        if (it != States.end())
            return &it->second;
        return 0;
    }
    float GetHeuristic(const CANavQuadTreeNode* node) const { return Distance2D(node->Position, GoalNode->Position); }
    bool CalculateRoute(bool ignorePreferability);
};

void* FindRef_CPriorityQueue_0();   // stand-in callee of the stand-in bodies below
// Stand-in bodies (the real ones are out of line in retail): they never write through their reference parameters, so
// VC7.1 knows the caller's address-taken locals do not escape the calls.
#pragma auto_inline(off)
__declspec(noinline) void CPriorityQueue::UpdateElement(CAStarState* const& element, const CAStarStateTotalGreaterThan& comp)
{
    if (element == FindRef_CPriorityQueue_0())
        _M_finish = 0;
}
__declspec(noinline) void CPriorityQueue::Push(CAStarState* const& element, const CAStarStateTotalGreaterThan& comp)
{
    if (element == FindRef_CPriorityQueue_0())
        _M_finish = 0;
}
#pragma auto_inline(on)

bool CNavigatorAStar::CalculateRoute(bool ignorePreferability)
{
    if (g_AStarStateCount > g_AStarMaxStateCount) {
        g_AStarMaxStateCount = g_AStarStateCount;
        g_AStarClosedAtMax = g_AStarClosedCount;
        g_AStarOpenAtMax = g_AStarOpenCount;
        g_AStarStat_b4 = g_AStarSearchesInProgress;
    }
    if (--g_AStarCountdown <= 0)
        g_AStarCountdown = 50;
    if (!NavigationManager->QueueManager.IsNavigatorAllowedToCalculateRoute(*(const CNavigatorBase*)this)) {
        PartWayThroughAStar = true;
        return false;
    }
    while (!OpenList.empty()) {
        CAStarState* state = OpenList.Pop(CAStarStateTotalGreaterThan());
        if (state->Node == GoalNode || (state->Node != 0 && GoalNode != 0 && *state->Node == *GoalNode)) {
            ConstructFinalRoute(state);
            GotRoute = true;
            return true;
        }
        const CNeighbourList* neighbours = state->Node->GetNeighbours();
        for (const CANavQuadTreeNode** it = neighbours->begin(); it != neighbours->end(); ++it) {
            --NavigationManager->NumberOfNodeExaminationsLeftThisFrame;
            const CANavQuadTreeNode* neighbour = *it;
            if (neighbour->Blocked)
                continue;
            CAStarState* neighbourState = GetState(neighbour);
            if (neighbourState != 0) {
                if (state->ParentState == neighbourState || neighbourState->OnClosedList)
                    continue;
                float cost = GetCostFromNodeToNode(state->Node, neighbourState->Node, ignorePreferability) + state->CostToThisState;
                if (cost < neighbourState->CostToThisState) {
                    neighbourState->CostToThisState = cost;
                    neighbourState->TotalCost = GetHeuristic(neighbourState->Node) * 2.0f + cost;
                    neighbourState->ParentState = state;
                    OpenList.UpdateElement(neighbourState, CAStarStateTotalGreaterThan());
                }
            }
            else {
                CAStarState newState;
                newState.ParentState = state;
                newState.Node = neighbour;
                newState.CostToThisState = GetCostFromNodeToNode(state->Node, neighbour, ignorePreferability) + state->CostToThisState;
                newState.TotalCost = GetHeuristic(neighbour) * 2.0f + newState.CostToThisState;
                newState.SetOnOpenList();
                pair<CStateIterator, bool> result = States.insert(make_pair((long)neighbour, newState));
                neighbourState = &result.first->second;
                OpenList.Push(neighbourState, CAStarStateTotalGreaterThan());
            }
        }
        state->SetOnClosedList();
        if (NavigationManager->NumberOfNodeExaminationsLeftThisFrame <= 0) {
            PartWayThroughAStar = true;
            return false;
        }
    }
    return false;
}

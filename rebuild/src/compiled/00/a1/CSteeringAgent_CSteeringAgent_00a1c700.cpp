// CSteeringAgent::CSteeringAgent(const CNavigatorExternalInterface* navigatorInterface, bool dummyObstacle) -- build the three steering-behaviour lists, reset the steering state, and for a real (non-dummy) agent with an interface add the agent-avoidance and goal behaviours, the agent input and the corridor and agent constraints
// Leaf: ??0CSteeringAgent@@QAE@PBVCNavigatorExternalInterface@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1C700, 582 bytes, thiscall, `ret 8`, returns this.
// Name: Ego_r.pdb / FableWin.pdb ??0CSteeringAgent@@QAE@PBVCNavigatorExternalInterface@@_N@Z; callers CNavigatorBase ctor
//   0x00A47158 (`new` 0x68, PInterface, false) and CTCObstacle 0x004DF3A6. Label "CNavigatorBase_Constructor" is the caller's.
// Layout (Ego_r.pdb CSteeringAgent, retail 0x68 with STLport's 4-byte lists): NavigatorInterface +0, DummyObstacle +4,
//   RepulsionMultiplier +8, SteeringBehaviours +0xC, SteeringBehaviourInputs +0x10, SteeringBehaviourConstraints +0x14,
//   CurrentMaxSpeed +0x18, CurrentSpeed +0x1C, Goal +0x20, NextGoal +0x28, TimeToReacquireGoal +0x30, DistCutoff +0x34,
//   Steer +0x38, BestDistLeft +0x40 .. BestNextDistRight +0x4C, ProximityForArrival +0x50, TimeSinceBestApproach +0x54,
//   BestApproach +0x58, then the nine bools +0x5C..+0x64.
// Callees: malloc 0x00BFEA0E (STLport list sentinels and nodes), operator new 0x00BFEA1A, CSBAgents::CSBAgents 0x00A873C0
//   (vtable 0x0129DEBC, size 0x10), CSBCCorridor::CSBCCorridor 0x00A21F50 (vtable 0x0129CCB0, size 0x2C), the interface's
//   GetNavigatorObstacles() const (slot 12, +0x30). Inline ctors: CSBGoal (0x0129CC34, 0x24), CSBIAgents (0x0129CC94, 0x18),
//   CSBCAgents (0x0129CC7C, 0x1C).
// Shape: the three lists are the only member initialisers (sentinels malloc'd first); the scalars are body assignments in
//   retail's store order. push_back is the real STLport 4.6 `insert(end(), x)` with a by-value iterator: end() is read
//   before the node is allocated and the new behaviour pointer is homed in a frame word (the const-reference argument),
//   which frees ebp for the hoisted &BestDistLeft. Base CSB / CSBInput / CSBConstraint ctors are inline and empty, so only
//   the most-derived vptr is stored. Class layouts: Ego_r.pdb (CSB 0xC = vptr + CKinematic Output; CSBInput 0x14 + Input;
//   CSBConstraint 0x18 with MoveBeforeOrientation +0x14; CSBGoal / CSBIAgents / CSBCAgents members as stored here).
class C3DVector;
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    // FableWin.pdb ??0C2DVector@@QAE@ABV0@@Z (user copy ctor); the member-wise user assignment is what makes VC7.1
    // re-read NavigatorInterface / DummyObstacle for the `if` (retail `mov eax,[esi]` / `cmp byte ptr [esi+4],bl`).
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector& operator=(const C2DVector& v) { X = v.X; Y = v.Y; return *this; }
};
class CKinematic {
public:
    C2DVector Vector;
};

extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);                     // 0x00BFEA1A
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    template <class _Tp1> allocator(const allocator<_Tp1>&) {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) {}
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
    iterator end() { return this->_M_node._M_data; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
template <class _Tp, class _Alloc> class vector;
}

class CSteeringAgent;

class CNavigatorExternalInterface {                                         // vtable order from CGroundNavigatorExternalInterface 0x0125EC9C
public:
    virtual bool IsMovementBlockedAt(const C3DVector& from, const C3DVector& to, bool& blocked, bool& dynamic) const;  // 0
    virtual float GetTerrainDesirabilityAt(const C3DVector& pos) const;                    // 1
    virtual float GetTerrainHeightAt(const C3DVector& pos) const;                          // 2
    virtual float GetThemeHeightAt(const C3DVector& pos) const;                            // 3
    virtual float FindPosZAt(const C3DVector& pos, bool b) const;                          // 4
    virtual float FindPosZAtIncludingThemes(const C3DVector& pos, bool b) const;           // 5
    virtual float PeekNavigatorRadius(bool physical) const;                                // 6
    virtual C2DVector PeekNavigatorOrientation() const;                                    // 7
    virtual C2DVector PeekNavigatorPosition2D() const;                                     // 8
    virtual C3DVector PeekNavigatorPosition3D() const;                                     // 9
    virtual float PeekNavigatorMaxSpeed(const C3DVector& pos) const;                       // 10
    // Overloads take their slots in reverse declaration order: the const one (declared first) is slot 12 (+0x30).
    virtual const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >& GetNavigatorObstacles() const;  // 12
    virtual std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >& GetNavigatorObstacles();             // 11
    virtual std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >* GetPNavigatorObstacles();            // 13
    virtual const CSteeringAgent* GetPThingToNotAvoid() const;                             // 14
    virtual float GetProximityForDirectApproach() const;                                   // 15
    virtual long GetNavigationLevelAt(const C3DVector& pos) const;                         // 16
    virtual long GetConstantFPS() const;                                                   // 17
    virtual float GetMinPathCost() const;                                                  // 18
    virtual float GetMaxPathCost() const;                                                  // 19
    virtual bool IsPlayer() const;                                                         // 20
    virtual bool IsDead() const;                                                           // 21
    virtual float GetForwardNavigationWidthExtension() const;                              // 22
    virtual ~CNavigatorExternalInterface();                                                // 23
    virtual CNavigatorExternalInterface* Clone() const;                                    // 24
};

class CSB {
public:
    enum ESB_TYPE { };
    CKinematic Output;                                                     // +0x04
    CSB() {}
    virtual ~CSB();
    virtual bool Process(const CSteeringAgent* agent) = 0;
    virtual ESB_TYPE GetType() = 0;
    virtual void Reset();
    virtual void CreateLineList(std::vector<C3DVector, std::allocator<C3DVector> >& lines);
};

class CSBAgents : public CSB {
public:
    const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >* Agents;   // +0x0C
    CSBAgents(const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >& agents, const CSteeringAgent* self);  // 0x00A873C0
    virtual bool Process(const CSteeringAgent* agent);
    virtual ESB_TYPE GetType();
};

class CSBGoal : public CSB {
public:
    C2DVector* Goal;                // +0x0C
    C2DVector* NextGoal;            // +0x10
    float* BestDistLeft;            // +0x14
    float* BestDistRight;           // +0x18
    float* BestNextDistLeft;        // +0x1C
    float* BestNextDistRight;       // +0x20
    CSBGoal(C2DVector* goal, C2DVector* nextGoal, float* bestDistLeft, float* bestDistRight, float* bestNextDistLeft, float* bestNextDistRight)
        : Goal(goal), NextGoal(nextGoal), BestDistLeft(bestDistLeft), BestDistRight(bestDistRight),
          BestNextDistLeft(bestNextDistLeft), BestNextDistRight(bestNextDistRight) {}
    virtual bool Process(const CSteeringAgent* agent);
    virtual ESB_TYPE GetType();
};

class CSBInput : public CSB {
public:
    CKinematic Input;               // +0x0C
    CSBInput() {}
};

class CSBIAgents : public CSBInput {
public:
    const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >* Agents;   // +0x14
    CSBIAgents(const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >& agents) : Agents(&agents) {}
    virtual bool Process(const CSteeringAgent* agent);
    virtual ESB_TYPE GetType();
    virtual void Display(const CSteeringAgent* agent);
};

class CSBConstraint : public CSB {
public:
    CKinematic Constraint;          // +0x0C (unnamed in Ego_r.pdb)
    bool MoveBeforeOrientation;     // +0x14
    CSBConstraint() {}
};

class CSBCAgents : public CSBConstraint {
public:
    const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >* Agents;   // +0x18
    CSBCAgents(const std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >& agents) : Agents(&agents) {}
    virtual bool Process(const CSteeringAgent* agent);
    virtual ESB_TYPE GetType();
};

class CSBCCorridor : public CSBConstraint {
public:
    unsigned char _body[0x2C - 0x18];   // Walls (STLport list) +0x18, CurrentEndPoint +0x1C, BestLeft +0x24, BestRight +0x28
    CSBCCorridor();                                                        // 0x00A21F50
    virtual ~CSBCCorridor();
    virtual bool Process(const CSteeringAgent* agent);
    virtual ESB_TYPE GetType();
    virtual void Reset();
    virtual void CreateLineList(std::vector<C3DVector, std::allocator<C3DVector> >& lines);
};

class CSteeringAgent {
public:
    const CNavigatorExternalInterface* NavigatorInterface;                 // +0x00
    bool DummyObstacle;                                                    // +0x04
    float RepulsionMultiplier;                                             // +0x08
    std::list<CSB*> SteeringBehaviours;                                    // +0x0C
    std::list<CSBInput*> SteeringBehaviourInputs;                          // +0x10
    std::list<CSBConstraint*> SteeringBehaviourConstraints;                // +0x14
    float CurrentMaxSpeed;                                                 // +0x18
    float CurrentSpeed;                                                    // +0x1C
    C2DVector Goal;                                                        // +0x20
    C2DVector NextGoal;                                                    // +0x28
    int TimeToReacquireGoal;                                               // +0x30
    float DistCutoff;                                                      // +0x34
    C2DVector Steer;                                                       // +0x38
    float BestDistLeft;                                                    // +0x40
    float BestDistRight;                                                   // +0x44
    float BestNextDistLeft;                                                // +0x48
    float BestNextDistRight;                                               // +0x4C
    float ProximityForArrival;                                             // +0x50
    int TimeSinceBestApproach;                                             // +0x54
    float BestApproach;                                                    // +0x58
    bool Wandering;                                                        // +0x5C
    bool MovingForwards;                                                   // +0x5D
    bool RecalcWidth;                                                      // +0x5E
    bool AddWaypoint;                                                      // +0x5F
    bool AvoidDynamicObstacles;                                            // +0x60
    bool AvoidDynamicObstaclesUntilNextWaypoint;                           // +0x61
    bool HasWanderedDuringThisSection;                                     // +0x62
    bool FailPath;                                                         // +0x63
    bool OnDirectApproach;                                                 // +0x64
    CSteeringAgent(const CNavigatorExternalInterface* navigatorInterface, bool dummyObstacle);
};

CSteeringAgent::CSteeringAgent(const CNavigatorExternalInterface* navigatorInterface, bool dummyObstacle)
{
    NavigatorInterface = navigatorInterface;
    DummyObstacle = dummyObstacle;
    RepulsionMultiplier = 1.0f;
    CurrentMaxSpeed = 0.0f;
    CurrentSpeed = 0.0f;
    Goal = C2DVector(0.0f, 0.0f);
    TimeToReacquireGoal = 0;
    DistCutoff = 5.0f;
    Wandering = false;
    Steer = C2DVector(0.0f, 0.0f);
    MovingForwards = true;
    AvoidDynamicObstacles = true;
    AvoidDynamicObstaclesUntilNextWaypoint = true;
    BestDistLeft = 0.0f;
    BestDistRight = 0.0f;
    BestNextDistLeft = 0.0f;
    BestNextDistRight = 0.0f;
    TimeSinceBestApproach = 0;
    BestApproach = 0.0f;
    RecalcWidth = false;
    AddWaypoint = false;
    HasWanderedDuringThisSection = false;
    FailPath = false;
    ProximityForArrival = 0.0f;
    if (NavigatorInterface && !DummyObstacle) {
        SteeringBehaviours.push_back(new CSBAgents(NavigatorInterface->GetNavigatorObstacles(), this));
        SteeringBehaviours.push_back(new CSBGoal(&Goal, &NextGoal, &BestDistLeft, &BestDistRight, &BestNextDistLeft, &BestNextDistRight));
        SteeringBehaviourInputs.push_back(new CSBIAgents(NavigatorInterface->GetNavigatorObstacles()));
        SteeringBehaviourConstraints.push_back(new CSBCCorridor());
        SteeringBehaviourConstraints.push_back(new CSBCAgents(NavigatorInterface->GetNavigatorObstacles()));
    }
}
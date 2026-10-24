// CNavigatorAStar::CNavigatorAStar(CNavigatorManager& manager, const CNavigatorExternalInterface& iface) -- CNavigatorBase(manager, 3, iface), empty state map, unblocked nodes, empty open list and route, unknown last position
// Leaf: ??0CNavigatorAStar@@AAE@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1CA70, 194 bytes, `ret 8`.
// Identity: Ego_r.pdb `??0CNavigatorAStar@@AAE@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z` (private);
//   the vtable stored is CNavigatorAStar's 0x0129CCCC (rtti-vtables.tsv), the only caller is CNavigatorAStar::Create
//   0x00A1CBF0 (operator new 0xC4). The catalog label "_global::CNavigatorBase_ctor" names the base instead.
// Callees: CNavigatorBase::CNavigatorBase(CNavigatorManager&, unsigned long type = 3, const CNavigatorExternalInterface&)
//   0x00A47110, CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(uchar, uchar) 0x00A768C0 (x3), malloc 0x00BFEA0E.
// Layout (retail STLport, 0xC4 bytes; PDB is 0xD4 with Dinkumware containers): States map +0x24 (12 bytes, node
//   0x28), three CNavBlockedQuadTreeNode +0x30/+0x40/+0x50, OpenList (CPriorityQueue = STLport vector, 12 bytes)
//   +0x60, GoalNode +0x6C, GoalPosition +0x70, NodeAskedToGetTo +0x78, PosAskedToGetTo +0x7C,
//   LastNavigableNodeVisited +0x88, Route (STLport list, 4 bytes, node 0x14) +0x8C, CullFront/CullBack +0x90/+0x94,
//   LastWayPointVisited +0x98, StringLeadPosition +0xA0, LastKnownNavPos +0xA8, LastKnownNavSpeed +0xB4,
//   LastKnownNavLayer +0xB8, then the eight bool/uchar flags +0xBC..+0xC3 (Initialised +0xBD).
// Shape: the init list in member order; the STLport map and list constructors are inlined (header node malloc'd,
//   colour red, root 0, leftmost/rightmost = header; list sentinel self-linked). Built without /EHsc, so no unwind.
//   The generated engine/CNavigatorBase.h is a flat POD (void* vftable) and cannot host a constructor; the model
//   keeps a local polymorphic base.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E

class CNavigatorManager;
class CNavigatorExternalInterface;
class CANavQuadTreeNode;
class CSteeringAgent;

class C2DVector { public: float X, Y; C2DVector() {} };
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };

class CNavBlockedQuadTreeNode {                         // 0x10 bytes
public:
    unsigned char _body[0x10];
    CNavBlockedQuadTreeNode(unsigned char a, unsigned char b);   // 0x00A768C0
};

struct CRetailLess { };
class CStateMap {                                       // STLport map<long, CNavigatorAStar::CAStarState>, 12 bytes
public:
    struct node {
        char colour;
        node* parent;
        node* left;
        node* right;
        unsigned char value[0x18];                      // pair<const long, CAStarState>
    };
    node*         m_header;
    unsigned long m_nodeCount;
    CRetailLess   m_compare;
    CStateMap() : m_header(0)
    {
        m_header = (node*)malloc(sizeof(node));
        m_nodeCount = 0;
        m_header->colour = 0;
        m_header->parent = 0;
        m_header->left = m_header;
        m_header->right = m_header;
    }
};

class CStatePtrVector {                                 // STLport vector<CAStarState*>, 12 bytes
public:
    void** m_start;
    void** m_finish;
    void** m_endOfStorage;
    CStatePtrVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};

class CWayPointList {                                   // STLport list<CNavWayPoint>, 4 bytes
public:
    struct node {
        node* next;
        node* prev;
        unsigned char value[0xC];                       // CNavWayPoint
    };
    node* m_node;
    CWayPointList() : m_node(0)
    {
        node* n = (node*)malloc(sizeof(node));
        n->next = n;
        n->prev = n;
        m_node = n;
    }
};

class CNavigatorBase {                                  // 0x24 bytes
public:
    virtual void v00();
    unsigned char _base[0x20];
    CNavigatorBase(CNavigatorManager& manager, unsigned long type, const CNavigatorExternalInterface& iface);   // 0x00A47110
};

class CNavigatorAStar : public CNavigatorBase {         // vtable 0x0129CCCC, 0xC4 bytes
public:
    virtual void v00();
    CStateMap               States;                     // +0x24
    CNavBlockedQuadTreeNode BlockedStartNode;           // +0x30
    CNavBlockedQuadTreeNode BlockedEndNode;             // +0x40
    CNavBlockedQuadTreeNode BlockedNodeAskedToGetTo;    // +0x50
    CStatePtrVector         OpenList;                   // +0x60 CPriorityQueue<CAStarState*>
    CANavQuadTreeNode*      GoalNode;                   // +0x6C
    C2DVector               GoalPosition;               // +0x70
    CANavQuadTreeNode*      NodeAskedToGetTo;           // +0x78
    C3DVector               PosAskedToGetTo;            // +0x7C
    CANavQuadTreeNode*      LastNavigableNodeVisited;   // +0x88
    CWayPointList           Route;                      // +0x8C
    void*                   CullFront;                  // +0x90 list iterator
    void*                   CullBack;                   // +0x94 list iterator
    C2DVector               LastWayPointVisited;        // +0x98
    C2DVector               StringLeadPosition;         // +0xA0
    C3DVector               LastKnownNavPos;            // +0xA8
    float                   LastKnownNavSpeed;          // +0xB4
    long                    LastKnownNavLayer;          // +0xB8
    bool                    HeadingForActionPoint;      // +0xBC
    bool                    Initialised;                // +0xBD
    bool                    GotRoute;                   // +0xBE
    bool                    SlowedDownForLastPoint;     // +0xBF
    bool                    Culled;                     // +0xC0
    bool                    PartWayThroughAStar;        // +0xC1
    unsigned char           NoOfClockwiseTurns;         // +0xC2
    unsigned char           NoOfAntiClockwiseTurns;     // +0xC3
private:
    CNavigatorAStar(CNavigatorManager& manager, const CNavigatorExternalInterface& iface);
};

CNavigatorAStar::CNavigatorAStar(CNavigatorManager& manager, const CNavigatorExternalInterface& iface)
    : CNavigatorBase(manager, 3, iface)
    , BlockedStartNode(0, 0)
    , BlockedEndNode(0, 0)
    , BlockedNodeAskedToGetTo(0, 0)
    , LastNavigableNodeVisited(0)
    , LastKnownNavPos(-1.0f, -1.0f, -1.0f)
    , LastKnownNavSpeed(-1.0f)
    , LastKnownNavLayer(-1)
    , Initialised(false)
{
}
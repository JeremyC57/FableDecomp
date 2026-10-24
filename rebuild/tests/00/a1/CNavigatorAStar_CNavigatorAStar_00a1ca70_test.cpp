// Behaviour test for CNavigatorAStar::CNavigatorAStar (0x00A1CA70): base built with type 3, the three blocked-node
// members built unblocked (0,0), empty state map / open list / route, unknown last position (-1), not initialised.
#include <cstdio>
#include <cstdlib>

class CNavigatorManager { public: int Id; };
class CNavigatorExternalInterface { public: int Id; };
class CANavQuadTreeNode;

class C2DVector { public: float X, Y; C2DVector() {} };
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };

class CNavBlockedQuadTreeNode {
public:
    unsigned char A, B;
    int Built;
    CNavBlockedQuadTreeNode(unsigned char a, unsigned char b) : A(a), B(b), Built(1) {}
};

struct CRetailLess { };
class CStateMap {
public:
    struct node {
        char colour;
        node* parent;
        node* left;
        node* right;
        unsigned char value[0x18];
    };
    node*         m_header;
    unsigned long m_nodeCount;
    CRetailLess   m_compare;
    CStateMap() : m_header(0)
    {
        m_header = (node*)std::malloc(sizeof(node));
        m_nodeCount = 0;
        m_header->colour = 0;
        m_header->parent = 0;
        m_header->left = m_header;
        m_header->right = m_header;
    }
};

class CStatePtrVector {
public:
    void** m_start;
    void** m_finish;
    void** m_endOfStorage;
    CStatePtrVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};

class CWayPointList {
public:
    struct node {
        node* next;
        node* prev;
        unsigned char value[0xC];
    };
    node* m_node;
    CWayPointList() : m_node(0)
    {
        node* n = (node*)std::malloc(sizeof(node));
        n->next = n;
        n->prev = n;
        m_node = n;
    }
};

class CNavigatorBase {
public:
    virtual ~CNavigatorBase() {}
    CNavigatorManager* Manager;
    unsigned long Type;
    const CNavigatorExternalInterface* Interface;
    CNavigatorBase(CNavigatorManager& manager, unsigned long type, const CNavigatorExternalInterface& iface)
        : Manager(&manager), Type(type), Interface(&iface) {}
};

class CNavigatorAStar : public CNavigatorBase {
public:
    CStateMap               States;
    CNavBlockedQuadTreeNode BlockedStartNode;
    CNavBlockedQuadTreeNode BlockedEndNode;
    CNavBlockedQuadTreeNode BlockedNodeAskedToGetTo;
    CStatePtrVector         OpenList;
    CANavQuadTreeNode*      GoalNode;
    C2DVector               GoalPosition;
    CANavQuadTreeNode*      NodeAskedToGetTo;
    C3DVector               PosAskedToGetTo;
    CANavQuadTreeNode*      LastNavigableNodeVisited;
    CWayPointList           Route;
    void*                   CullFront;
    void*                   CullBack;
    C2DVector               LastWayPointVisited;
    C2DVector               StringLeadPosition;
    C3DVector               LastKnownNavPos;
    float                   LastKnownNavSpeed;
    long                    LastKnownNavLayer;
    bool                    HeadingForActionPoint;
    bool                    Initialised;
    bool                    GotRoute;
    bool                    SlowedDownForLastPoint;
    bool                    Culled;
    bool                    PartWayThroughAStar;
    unsigned char           NoOfClockwiseTurns;
    unsigned char           NoOfAntiClockwiseTurns;
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

static int nearf(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CNavigatorManager manager; manager.Id = 1;
    CNavigatorExternalInterface iface; iface.Id = 2;
    int ok = 1;
    CNavigatorAStar* nav = new CNavigatorAStar(manager, iface);
    if (nav->Manager != &manager || nav->Type != 3 || nav->Interface != &iface) ok = 0;
    if (nav->States.m_nodeCount != 0 || nav->States.m_header == 0) ok = 0;
    else if (nav->States.m_header->colour != 0 || nav->States.m_header->parent != 0 ||
             nav->States.m_header->left != nav->States.m_header || nav->States.m_header->right != nav->States.m_header) ok = 0;
    if (!nav->BlockedStartNode.Built || nav->BlockedStartNode.A || nav->BlockedStartNode.B) ok = 0;
    if (!nav->BlockedEndNode.Built || nav->BlockedEndNode.A || nav->BlockedEndNode.B) ok = 0;
    if (!nav->BlockedNodeAskedToGetTo.Built || nav->BlockedNodeAskedToGetTo.A || nav->BlockedNodeAskedToGetTo.B) ok = 0;
    if (nav->OpenList.m_start || nav->OpenList.m_finish || nav->OpenList.m_endOfStorage) ok = 0;
    if (nav->LastNavigableNodeVisited != 0) ok = 0;
    if (nav->Route.m_node == 0 || nav->Route.m_node->next != nav->Route.m_node || nav->Route.m_node->prev != nav->Route.m_node) ok = 0;
    if (!nearf(nav->LastKnownNavPos.X, -1.0f) || !nearf(nav->LastKnownNavPos.Y, -1.0f) || !nearf(nav->LastKnownNavPos.Z, -1.0f)) ok = 0;
    if (!nearf(nav->LastKnownNavSpeed, -1.0f) || nav->LastKnownNavLayer != -1 || nav->Initialised) ok = 0;
    std::free(nav->States.m_header);
    std::free(nav->Route.m_node);
    delete nav;

    if (!ok) { std::printf("00a1ca70_TEST FAIL\n"); return 1; }
    std::printf("00a1ca70_TEST PASS\n");
    return 0;
}
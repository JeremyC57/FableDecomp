// Behaviour test for CSteeringAgent::~CSteeringAgent (0x00A1C950): every list element is deleted and nulled,
// the agent is unregistered first and the navigator pointer cleared.
#include <cstdio>
#include <cstdlib>

static int g_deleted = 0;
static int g_removed = 0;
static int g_orderOk = 1;

class CSB { public: virtual ~CSB() { ++g_deleted; if (!g_removed) g_orderOk = 0; } };
class CSBInput { public: virtual ~CSBInput() { ++g_deleted; } };
class CSBConstraint { public: virtual ~CSBConstraint() { ++g_deleted; } };

template <class T>
struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T>
struct CPtrList {
    SListNode<T>* _M_node;
    SListNode<T>* begin() { return _M_node->_M_next; }
    SListNode<T>* end() { return _M_node; }
    void Init() { _M_node = (SListNode<T>*)std::malloc(sizeof(SListNode<T>)); _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; }
    void PushBack(T v)
    {
        SListNode<T>* n = (SListNode<T>*)std::malloc(sizeof(SListNode<T>));
        n->_M_data = v;
        n->_M_next = _M_node;
        n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n;
        _M_node->_M_prev = n;
    }
    void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            std::free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~CPtrList() { clear(); if (_M_node) std::free(_M_node); }
};

class CSteeringAgent;
static CSteeringAgent* g_removedAgent = 0;
static void RemoveAgentFromLists(CSteeringAgent* agent) { g_removedAgent = agent; g_removed = 1; }

static int g_navNullAfter = 0;
static int g_slotsNulled = 0;

class CSteeringAgent {
public:
    void*                    NavigatorInterface;
    bool                     DummyObstacle;
    float                    RepulsionMultiplier;
    CPtrList<CSB*>           SteeringBehaviours;
    CPtrList<CSBInput*>      SteeringBehaviourInputs;
    CPtrList<CSBConstraint*> SteeringBehaviourConstraints;
    ~CSteeringAgent()
    {
        RemoveAgentFromLists(this);
        NavigatorInterface = 0;
        for (SListNode<CSB*>* b = SteeringBehaviours.begin(); b != SteeringBehaviours.end(); b = b->_M_next) {
            delete b->_M_data;
            b->_M_data = 0;
        }
        for (SListNode<CSBInput*>* i = SteeringBehaviourInputs.begin(); i != SteeringBehaviourInputs.end(); i = i->_M_next) {
            delete i->_M_data;
            i->_M_data = 0;
        }
        for (SListNode<CSBConstraint*>* c = SteeringBehaviourConstraints.begin(); c != SteeringBehaviourConstraints.end(); c = c->_M_next) {
            delete c->_M_data;
            c->_M_data = 0;
        }
        // observe the post-body state before the member destructors run
        g_navNullAfter = (NavigatorInterface == 0);
        int n = 0;
        for (SListNode<CSB*>* b2 = SteeringBehaviours.begin(); b2 != SteeringBehaviours.end(); b2 = b2->_M_next) if (b2->_M_data == 0) ++n;
        for (SListNode<CSBInput*>* i2 = SteeringBehaviourInputs.begin(); i2 != SteeringBehaviourInputs.end(); i2 = i2->_M_next) if (i2->_M_data == 0) ++n;
        for (SListNode<CSBConstraint*>* c2 = SteeringBehaviourConstraints.begin(); c2 != SteeringBehaviourConstraints.end(); c2 = c2->_M_next) if (c2->_M_data == 0) ++n;
        g_slotsNulled = n;
    }
};

int main()
{
    CSteeringAgent* a = (CSteeringAgent*)std::malloc(sizeof(CSteeringAgent));
    a->NavigatorInterface = (void*)a;
    a->SteeringBehaviours.Init();
    a->SteeringBehaviourInputs.Init();
    a->SteeringBehaviourConstraints.Init();
    a->SteeringBehaviours.PushBack(new CSB);
    a->SteeringBehaviours.PushBack(0);
    a->SteeringBehaviours.PushBack(new CSB);
    a->SteeringBehaviourInputs.PushBack(new CSBInput);
    a->SteeringBehaviourConstraints.PushBack(new CSBConstraint);
    a->SteeringBehaviourConstraints.PushBack(new CSBConstraint);
    CSteeringAgent* expect = a;
    a->~CSteeringAgent();
    std::free(a);
    if (g_deleted != 5 || g_removedAgent != expect || !g_orderOk || !g_navNullAfter || g_slotsNulled != 6) {
        std::printf("00a1c950_TEST FAIL\n");
        return 1;
    }
    std::printf("00a1c950_TEST PASS\n");
    return 0;
}
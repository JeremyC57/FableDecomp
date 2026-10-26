// CSteeringAgent::~CSteeringAgent() -- unregister the agent, delete every behaviour/input/constraint object and free the three lists
// Leaf: ??1CSteeringAgent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1C950, 283 bytes (+5 int3 padding).
// Callees: RemoveAgentFromLists 0x00A47D10 (__fastcall, the agent in ecx), free 0x00BFEA14 (STLport node deallocation).
// Layout (retail): NavigatorInterface +0x00, DummyObstacle +0x04, RepulsionMultiplier +0x08, then three STLport
//   lists of pointers (4 bytes each: the sentinel node pointer) SteeringBehaviours +0x0c, SteeringBehaviourInputs +0x10,
//   SteeringBehaviourConstraints +0x14. Ego_r.pdb puts the lists at +0x0c/+0x18/+0x24 (12-byte Dinkumware lists); retail
//   STLport lists are 4 bytes, so `this` is a local model. No vtable: the class is not polymorphic.
// Shape: the body unregisters the agent, clears the navigator pointer, then walks each list deleting its polymorphic
//   element (`delete *it` = null test + scalar deleting destructor through vtable slot 0) and nulling the slot; the end
//   node is re-read after each virtual call. The implicit member destructors then run in reverse order (+0x14, +0x10,
//   +0x0c): STLport `_List_base::~_List_base` = inline clear() (free each node, reset the sentinel) + the allocator's
//   null-tested deallocate of the sentinel.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

class CNavigatorExternalInterface;
class CSB { public: virtual ~CSB(); };
class CSBInput { public: virtual ~CSBInput(); };
class CSBConstraint { public: virtual ~CSBConstraint(); };

template <class T>
struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T>
struct CPtrList {                                            // STLport list<T*> (4 bytes)
    SListNode<T>* _M_node;
    SListNode<T>* begin() { return _M_node->_M_next; }
    SListNode<T>* end() { return _M_node; }
    void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~CPtrList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};

class CSteeringAgent;
void __fastcall RemoveAgentFromLists(CSteeringAgent* agent);   // 0x00A47D10

class CSteeringAgent {
public:
    CNavigatorExternalInterface* NavigatorInterface;         // +0x00
    bool                         DummyObstacle;              // +0x04
    float                        RepulsionMultiplier;        // +0x08
    CPtrList<CSB*>               SteeringBehaviours;         // +0x0c
    CPtrList<CSBInput*>          SteeringBehaviourInputs;    // +0x10
    CPtrList<CSBConstraint*>     SteeringBehaviourConstraints; // +0x14
    ~CSteeringAgent();
};

CSteeringAgent::~CSteeringAgent()
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
}
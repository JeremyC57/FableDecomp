// Behaviour test for 0x00A66F20 CMouse::CMouse(CInputManager&, bool)
#include <cstdio>
#include <cstdlib>

static int g_baseCtors = 0;
static int g_eventCtors = 0;

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable() { ++g_baseCtors; }          // 0x0099A2F0 stand-in
    virtual ~CBaseClassNonCopyable() { }
};
class CInputManager {
public:
    int Id;
};
class CInputEvent {
public:
    long Kind;
    unsigned char Data[0x30];
    CInputEvent() : Kind(0) { ++g_eventCtors; }       // 0x00A04410 stand-in
};
class CMaintainedEvent {
public:
    unsigned char Data[0x38];
};
namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)std::malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { std::free(p); }
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
template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()                                   // test-only teardown: free every node and the sentinel
    {
        _List_node_base* n = _M_node._M_data->_M_next;
        while (n != _M_node._M_data) { _List_node_base* next = n->_M_next; std::free(n); n = next; }
        std::free(_M_node._M_data);
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) {}
};
}   // namespace _STL

class CMouse : public CBaseClassNonCopyable {
public:
    CInputEvent                     EventStore[256];
    long                            NoStoredEvents;
    long                            NoEventScansRunning;
    _STL::list<CMaintainedEvent>    MaintainedEventsList;
    CInputManager*                  InputManager;
    CMouse(CInputManager& inputManager, bool)
    {
        NoEventScansRunning = 0;
        NoStoredEvents = 0;
        InputManager = &inputManager;
    }
    virtual ~CMouse() { }
};

int main()
{
    CInputManager manager;
    manager.Id = 7;
    CMouse* device = new CMouse(manager, true);
    _STL::_List_node_base* s = device->MaintainedEventsList._M_node._M_data;
    bool ok = g_baseCtors == 1 && g_eventCtors == 256 && device->EventStore[255].Kind == 0 && device->NoStoredEvents == 0 &&
              device->NoEventScansRunning == 0 && s != 0 && s->_M_next == s && s->_M_prev == s &&
              device->InputManager == &manager && device->InputManager->Id == 7;
    delete device;
    if (!ok) { std::printf("00a66f20_TEST FAIL\n"); return 1; }
    std::printf("00a66f20_TEST PASS\n");
    return 0;
}
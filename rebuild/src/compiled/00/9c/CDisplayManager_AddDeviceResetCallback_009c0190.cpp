// CDisplayManager::AddDeviceResetCallback(CDeviceResetCallback* callback) -- append a device-reset listener to the manager's STLport callback list
// Leaf: ?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C0190, 47 bytes (+1 int3), thiscall `ret 4`.
// Name: FableWin.pdb `?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z`; the landed
// CSystemManager::Initialise (0x009A6610) already calls 0x009C0190 under that name. Other callers: the
// CUserProfileManager ctor (0x0040CEC0), CMainGameComponent ctor (0x00418DCA), CEngine::Initialise (0x00B26360).
// Label correction: catalogued as `CMainGameComponent_AddComponentToList`.
// Callee: malloc @ 0x00BFEA0E (STLport node allocator, 12-byte node).
// Layout: Ego_r.pdb puts `list<CDeviceResetCallback*> DeviceResetCallbacks` at +612 (0x264), as retail reads it; the
// retail STLport list is one word (the sentinel pointer). No engine header for CDisplayManager: local model.
// Shape: STLport `push_back(x) { insert(end(), x); }` fully inlined: end()'s sentinel is read before the malloc,
// `_M_create_node` null-tests &node->_M_data (placement new), then next/prev/prev->next/sentinel->prev in the
// STLport 4.x insert order.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CDeviceResetCallback;

namespace _STL {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;

    iterator end() { return iterator(_M_node); }

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }

    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }

    void push_back(const _Tp& __x) { insert(end(), __x); }
};

}

class CDisplayManager {
public:
    void AddDeviceResetCallback(CDeviceResetCallback* callback);
private:
    unsigned char _pad000[0x264];
    _STL::list<CDeviceResetCallback*> DeviceResetCallbacks;     // +0x264
};

void CDisplayManager::AddDeviceResetCallback(CDeviceResetCallback* callback)
{
    DeviceResetCallbacks.push_back(callback);
}
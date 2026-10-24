// CDirectInputDeviceLists_00a61b10::ReleaseAll() -- descriptive name: empty both STLport lists of DirectInput interfaces, second list first: while a list is non-empty, COM Release() the interface in its last element, then pop_back (unlink + free the node)
// Leaf: ?ReleaseAll@CDirectInputDeviceLists_00a61b10@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A61A80, 131 bytes (+13 int3).
// Caller: the landed destructor @ 0x00A61B10 (call at 0x00A61B13). Callees: IUnknown::Release (vtable slot 2, __stdcall),
//   free @ 0x00BFEA14 (the node deallocate).
// Label: the manifest's `DoubleLinkedList_ClearAndFree` is descriptive; no PDB names this member class.
// Shape: `while (list.size() > 0) { list.back().Interface->Release(); list.pop_back(); }` -- STLport size() is the
//   inline node count (`jbe` on the unsigned compare), back()/pop_back() go through --end().
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

struct IUnknown {                                      // COM: Release is vtable slot 2 (__stdcall, `push p ; call [vtbl+8]`)
    virtual long __stdcall QueryInterface(const void* iid, void** out) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct SDirectInputInterfaceEntry {                    // 8-byte list element (node 0x10): the interface first
    IUnknown*     Interface;                           // +0x00
    unsigned long Data;                                // +0x04
};

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { free(p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp> struct _Nonconst_traits;
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
    _Self& operator--() { this->_M_decr(); return *this; }
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
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef unsigned int size_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) {}
    ~list();                                           // 0x00A61D20
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = 0;
        for (_List_node_base* __n = this->_M_node._M_data->_M_next; __n != this->_M_node._M_data; __n = __n->_M_next)
            ++__result;
        return __result;
    }
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void pop_back() { iterator __tmp = end(); erase(--__tmp); }
};

}   // namespace _STL

class CDirectInputDeviceLists_00a61b10 {               // CInputManagerDX member at +0x2B0 (0x10 bytes)
public:
    _STL::list<SDirectInputInterfaceEntry> m_devices;   // +0x00
    _STL::list<SDirectInputInterfaceEntry> m_effects;   // +0x04
    long          m_enabled;                            // +0x08 (1 after construction)
    long          m_count;                              // +0x0c (0 after construction)
    CDirectInputDeviceLists_00a61b10();
    void ReleaseAll();
};

void CDirectInputDeviceLists_00a61b10::ReleaseAll()
{
    while (m_effects.size() > 0) {
        m_effects.back().Interface->Release();
        m_effects.pop_back();
    }
    while (m_devices.size() > 0) {
        m_devices.back().Interface->Release();
        m_devices.pop_back();
    }
}
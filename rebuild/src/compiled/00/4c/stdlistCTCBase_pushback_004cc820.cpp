// std::list<CTCBase*>::push_back(CTCBase* const& x) -- STLport: append a thing-component pointer (a 12-byte node linked before the sentinel)
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CC820, length 45 bytes, `ret 4` (thiscall: the value reference).
// Identity: CThing::UpdateComponentsList (+0x54, list<CTCBase*>, Ego_r.pdb); the single caller 0x004C9F48 is the landed
// CThing::AddTC 0x004C9D60, which already names this callee "list<CTCBase*>::push_back @ 0x004CC820". The catalog
// "ListNode_InsertTCNode" is behavioural.
// Callees: malloc @ 0x00BFEA0E.
// Shape: STLport 4.x `push_back(x) { insert(end(), x); }` with insert and `_M_create_node` inlined (as 0x004C0050): the
// sentinel loaded into esi first, the placement copy null-tested, prev re-read after the allocation.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CTCBase;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return static_cast<T*>(malloc(n * sizeof(T))); }
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    __forceinline _List_iterator(_List_node_base* x) : _M_node(x) {}
    __forceinline _List_iterator(const _List_iterator& x) : _M_node(x._M_node) {}
};
template <class T> class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    struct : allocator<_Node> { _List_node_base* _M_data; } _M_node;   // _STLP_alloc_proxy: the sentinel
    __forceinline iterator end() { return iterator(_M_node._M_data); }
    __forceinline _Node* _M_create_node(const T& x)
    {
        _Node* p = _M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    __forceinline iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};

template void list<CTCBase*>::push_back(CTCBase* const& x);
}
// std::list<pair<CCharString,long>>::push_back(const pair<CCharString,long>& value) -- STLport: malloc a 16-byte node, copy the (CCharString, 4-byte) element into it and link it before the sentinel
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- a user-declared forceinline _List_iterator copy ctor (the 0x004C0050 fix) keeps ebp saved in the prologue
// Leaf: ?push_back@?$list@U?$pair@VCCharString@@J@std@@@std@@QAEXABU?$pair@VCCharString@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BA420, length 63 bytes, `ret 4` (thiscall: value).
// Callees: malloc @ 0x00BFEA0E, CCharString copy constructor @ 0x0099EC30.
// Label: the catalog "StdList_PushBack" is behavioural. STLport 4.x `push_back(x) { insert(end(), x); }` with insert and
// _M_create_node inlined: the sentinel (end()) is read before the allocation, the element is placement-copied
// (null test on &node->_M_data, CCharString copy ctor, then the 4-byte second member), then
// `tmp->next = n ; tmp->prev = p ; p->next = tmp ; n->prev = tmp` with p = n->_M_prev in a local.
// No direct callers in the image (identical-code folding / table use); the second member's type is not observable,
// `long` stands for it.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData;
class CCharString {
public:
    CCharString(const CCharString& other);      // 0x0099EC30
private:
    CCharStringData* m_pData;
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

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
    __forceinline _List_iterator(const _List_iterator& x) : _M_node(x._M_node) {}   // user copy ctor (the 0x004C0050 fix)
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
    void push_back(const T& x);
};

template <class T> void list<T>::push_back(const T& x)
{
    insert(end(), x);
}

template class list<pair<CCharString, long> >;
}
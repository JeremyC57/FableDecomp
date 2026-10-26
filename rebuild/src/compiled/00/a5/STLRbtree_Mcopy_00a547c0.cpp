// std::_Rb_tree::_M_copy(_Rb_tree_node* x, _Rb_tree_node* p) -- STLport structural tree copy: clone x under p, recurse into each right subtree, walk the left spine iteratively; returns the cloned top
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A547C0, length 207 bytes, `ret 8`.
// Callers: itself (0x00A54816, 0x00A54868) and the tree's operator= 0x00A54A90 (landed as CRetailTree_00a547c0::operator=,
//   which calls this address as Copy(src, parent)). Callees: malloc 0x00BFEA0E (STLport's malloc node allocator, 0x18-byte
//   nodes), CCharString copy ctor 0x0099EC30.
// Node (0x18 bytes): color byte +0, parent +4, left +8, right +0xC, value pair<const CCharString, 4-byte value> at +0x10
//   (key string +0x10, value +0x14). The map's mapped type is not recoverable from these bytes (copied as one word):
//   modelled as `long` (descriptive).
// Label correction: catalogued as `TreeNode_CloneRecursive`; the body is STLport `_Rb_tree::_M_copy`.
// Shape: STLport 4.x `_M_copy` verbatim with `_M_clone_node` / `_M_create_node` inlined: allocate, placement copy of the
//   value (null test on &node->value), then the color byte and the two zero child links, parent link; `this` is parked in
//   the `push ecx` frame word and `__top` in the dead `x` argument slot across the loop.
extern "C" void* __cdecl malloc(unsigned int);       // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
};

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp>
class less {
public:
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};

template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    char                _M_color;                    // +0x00
    _Rb_tree_node_base* _M_parent;                   // +0x04
    _Rb_tree_node_base* _M_left;                     // +0x08
    _Rb_tree_node_base* _M_right;                    // +0x0C
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    unsigned int _M_node_count;                      // +0x04
    _Compare     _M_key_compare;                     // +0x08

    static _Link_type& _S_left(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_right); }
protected:
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = _M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
private:
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
};

template <class _Key, class _Value, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _Compare, _Alloc>::_M_copy(_Link_type __x, _Link_type __p)
{
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;

    if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
    __p = __top;
    __x = _S_left(__x);

    while (__x != 0) {
        _Link_type __y = _M_clone_node(__x);
        __p->_M_left = __y;
        __y->_M_parent = __p;
        if (__x->_M_right)
            __y->_M_right = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
    }
    return __top;
}

template class _Rb_tree<CCharString, pair<const CCharString, long>, less<CCharString>, allocator<pair<const CCharString, long> > >;
}
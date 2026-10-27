// std::_Rb_tree::_M_copy(_Rb_tree_node<CUnprovenValue_00999e50>* x, _Rb_tree_node<CUnprovenValue_00999e50>* p) -- (_Rb_tree<CUnprovenKey_00999e50, CUnprovenValue_00999e50, CUnprovenKeyOfValue_00999e50, less<CUnprovenKey_00999e50>, allocator<CUnprovenValue_00999e50> >) STLport structural tree copy: clone x under parent p, recurse down every right subtree and iterate down the left spine
// Leaf: ?_M_copy@?$_Rb_tree@UCUnprovenKey_00999e50@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099A140, 169 bytes, `ret 8` (thiscall: x, p; returns the cloned top).
// Identity (probable): the instantiation of the landed 0x00999E50 (_M_erase) / 0x00999FE0 (_Rb_tree ctor) / 0x0099A0B0
//   (container ctor) family: its callers are itself (0x0099A186, 0x0099A1CE) and the tree operator= at 0x0099A1F0, which
//   first erases through 0x00999E50 and then copies through this body. The key/value stay the family's placeholders, but
//   the node value is ONE byte here (`mov cl,[edi+10h] ; mov [eax],cl`), not the family header's `long` (see NOTES).
// Callee: malloc @ 0x00BFEA0E (node allocate(1) = malloc(0x14)).
// Shape: the landed set<long> _M_copy 0x005114F0 (same 169 bytes) verbatim over a 1-byte value.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
struct CUnprovenKey_00999e50 { char _M_unproven; };                     // one byte: the node value copy is a byte move
inline bool operator<(const CUnprovenKey_00999e50& __x, const CUnprovenKey_00999e50& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenValue_00999e50 { CUnprovenKey_00999e50 _M_key; };
struct CUnprovenKeyOfValue_00999e50 { const CUnprovenKey_00999e50& operator()(const CUnprovenValue_00999e50& __v) const { return __v._M_key; } };

namespace std {

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                           // +0x00
    _Base_ptr   _M_parent;                          // +0x04
    _Base_ptr   _M_left;                            // +0x08
    _Base_ptr   _M_right;                           // +0x0C
};

template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

template <class T> struct _Identity { const T& operator()(const T& x) const { return x; } };
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Node* _Link_type;

    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;     // +0x00
    unsigned int _M_node_count;                                 // +0x04
    Compare _M_key_compare;                                     // +0x08

    static _Link_type& _S_left(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_right); }

protected:
    _Link_type _M_create_node(const Value& x)
    {
        _Link_type tmp = _M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
private:
    _Link_type _M_copy(_Link_type x, _Link_type p);
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Link_type
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_copy(_Link_type x, _Link_type p)
{
    _Link_type top = _M_clone_node(x);
    top->_M_parent = p;
    if (x->_M_right)
        top->_M_right = _M_copy(_S_right(x), top);
    p = top;
    x = _S_left(x);
    while (x != 0) {
        _Link_type y = _M_clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right)
            y->_M_right = _M_copy(_S_right(x), y);
        p = y;
        x = _S_left(x);
    }
    return top;
}

typedef _Rb_tree<CUnprovenKey_00999e50, CUnprovenValue_00999e50, CUnprovenKeyOfValue_00999e50, less<CUnprovenKey_00999e50>,
                 allocator<CUnprovenValue_00999e50> > _Tree_00999e50;
template _Tree_00999e50::_Link_type _Tree_00999e50::_M_copy(_Tree_00999e50::_Link_type x, _Tree_00999e50::_Link_type p);

}
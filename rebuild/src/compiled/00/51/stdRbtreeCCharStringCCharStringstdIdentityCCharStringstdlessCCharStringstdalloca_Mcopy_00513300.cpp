// std::_Rb_tree<CCharString,CCharString,std::_Identity<CCharString>,std::less<CCharString>,std::allocator<CCharString> >::_M_copy(_Rb_tree_node<CCharString>* x, _Rb_tree_node<CCharString>* p) -- STLport structural copy of a string-set tree: clone x under parent p (copy-constructing the string), recurse down every right subtree and iterate down the left spine
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513300, 182 bytes, `ret 8` (thiscall: x, p; returns the cloned top).
// Callers: itself (0x0051334A, 0x0051339B), 0x0051445B and 0x006B9E64 (string-set copy paths). The value is a
//   CCharString copy-constructed out of line, so the body is set<CCharString>'s (or any 4-byte CCharString-valued
//   tree folded into it). Catalog label "CCharStringTreeNode_DeepClone" is descriptive.
// Callees: malloc @ 0x00BFEA0E (node allocate(1) = malloc(0x14)); CCharString::CCharString(const CCharString&) @
//   0x0099EC30.
// Node: STLport _Rb_tree_node_base {bool _M_color +0; _M_parent +4; _M_left +8; _M_right +0xC} + _M_value_field +0x10.
// Shape: the CCharString twin of 0x005114F0: STLport 4.5 `_M_copy` with `_M_clone_node` inlined at both sites (the
//   placement copy constructor behind the null test, the colour, both children zeroed); the 16-byte aligned left-spine
//   loop head is entered by a `jmp` over the padding.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    bool operator<(const CCharString& other) const;
};

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

typedef _Rb_tree<CCharString, CCharString, _Identity<CCharString>, less<CCharString>, allocator<CCharString> > _StringTree;
template _StringTree::_Link_type _StringTree::_M_copy(_StringTree::_Link_type x, _StringTree::_Link_type p);

}
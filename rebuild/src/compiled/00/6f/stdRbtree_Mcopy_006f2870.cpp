// std::_Rb_tree::_M_copy(_Rb_tree_node* x, _Rb_tree_node* p) -- STLport structural copy of a UID-keyed tree: clone x under p, recurse right, walk left
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F2870, 193 bytes, `ret 8`.
// Label correction: catalog "BinaryTree_CloneNodeRecursive" is propagated; this is STLport _Rb_tree::_M_copy of the
//   map<unsigned __int64, CThingCreatureBase*> (0x20-byte nodes).
// Callees: malloc 0x00BFEA0E (node allocation), itself (right subtrees).
// Shape: STLport _M_copy verbatim: _M_clone_node = _M_create_node (malloc + placement copy of the value) + colour +
//   null children; the pair must have STLport's explicit copy constructor (memberwise: three dword copies, not a
//   4-dword block copy).
extern "C" void* __cdecl malloc(unsigned int size);      // MSVCR71 import thunk 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CThingCreatureBase;

namespace std {
template<class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}   // STLport's explicit copy constructor
};

template<class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template<class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

template<class T> class allocator { public: allocator() {} };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0c
};

template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                                 // +0x10
};

template<class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node<Value>* x) : _M_node(x) {}
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = allocator<Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    typedef _Rb_tree_iterator<Value> iterator;
    _Link_type _M_header;                                 // +0x00
    unsigned int _M_node_count;                           // +0x04
    Compare _M_key_compare;                               // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header->_M_parent; }
    static _Link_type& _S_left(_Link_type x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Link_type x) { return (_Link_type&)(x->_M_right); }
    static const Key& _S_key(_Link_type x) { return KeyOfValue()(x->_M_value_field); }

    static bool& _S_color(_Link_type x) { return x->_M_color; }
protected:
    _Link_type _M_create_node(const Value& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<Value>));
        new (&tmp->_M_value_field) Value(x);
        return tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        _S_color(tmp) = _S_color(x);
        _S_left(tmp) = 0;
        _S_right(tmp) = 0;
        return tmp;
    }
private:
    _Link_type _M_copy(_Link_type x, _Link_type p);
};
}

namespace std {
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
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
}

typedef std::pair<const unsigned __int64, CThingCreatureBase*> SCreatureByUID;
template class std::_Rb_tree<unsigned __int64, SCreatureByUID, std::_Select1st<SCreatureByUID>, std::less<unsigned __int64> >;
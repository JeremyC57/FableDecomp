// _Rb_tree::_M_clone_node(_Rb_tree_node<pair<const unsigned __int64, CThingPosAndOrientation> >* x) -- STLport node clone for the ThingsToMove map: allocate a node, copy-construct the value, copy the colour and clear both children
// Leaf: ?_M_clone_node@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00825A80, 93 bytes (+int3 pad), thiscall `ret 4` (`this` unused).
// Callee: malloc @ 0x00BFEA0E (0x30-byte node).
// Caller: _Rb_tree::_M_copy 0x008260C0 (its top clone, 0x008260D0).
// Label: the catalog's "Linked_CloneNode" is descriptive; the body is STLport `_Rb_tree::_M_clone_node` of
//   map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>.
// Shape: `tmp = _M_create_node(x->_M_value_field); _S_color(tmp) = _S_color(x); _S_left(tmp) = 0; _S_right(tmp) = 0;
//   return tmp;` -- `_M_create_node` = `malloc(sizeof(node))` + the null-tested placement copy (`lea ecx,[eax+10h] ;
//   cmp ecx,edx`): the key and Position move directly, FacingAngles through its own address; one zero register (edx)
//   serves the null test and both child stores.
struct C3DVector {                                          // user memberwise copy constructor
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct C3DAngles {                                          // trivially copyable
    float X, Y, Z;
};
struct CThingPosAndOrientation {                            // CScriptedMapBrush::CThingPosAndOrientation, 24 bytes
    C3DVector Position;                                     // +0x00
    C3DAngles FacingAngles;                                 // +0x0C
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};
}
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

struct _Rb_tree_node_base {                                 // 16 bytes
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class T, class _Node> struct _STLP_alloc_proxy : allocator<_Node> {
    T _M_data;
};

template <class _Value> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, _Node> _M_header;             // +0x00
};

template <class _Key, class _Value, class _Compare> class _Rb_tree : public _Rb_tree_base<_Value> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    static _Link_type& _S_left(_Link_type x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Link_type x) { return (_Link_type&)(x->_M_right); }
    static bool& _S_color(_Link_type x) { return x->_M_color; }
protected:
    _Link_type _M_create_node(const _Value& x)
    {
        _Link_type tmp = this->_M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
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

template <class _Key, class _Value, class _Compare>
typename _Rb_tree<_Key, _Value, _Compare>::_Link_type _Rb_tree<_Key, _Value, _Compare>::_M_copy(_Link_type x, _Link_type p)
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

template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
}

template class std::_Rb_tree<unsigned __int64, std::pair<const unsigned __int64, CThingPosAndOrientation>, std::less<unsigned __int64> >;
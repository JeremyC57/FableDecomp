// _Rb_tree::_M_copy(_Rb_tree_node<pair<const unsigned __int64, CThingPosAndOrientation> >* x, _Rb_tree_node<...>* p) -- STLport structural copy of a map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> subtree: clone x under p, recurse down every right child and walk the left spine iteratively
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008260C0, 192 bytes, thiscall `ret 8`; recursive (0x008260E9 / 0x00826165).
// Callees: _Rb_tree::_M_clone_node @ 0x00825A80 (the TOP clone stays out of line, `ret 4`), malloc @ 0x00BFEA0E (the
//   inlined clone in the loop: 0x30-byte nodes), itself.
// Caller: the ThingsToMove map copy constructor 0x008267C0 (0x0082681B; the landed CScriptedMapBrush copy ctor 0x00827040).
// Label: the catalog's "Linked_CloneNodeLarge" is descriptive; the body is STLport `_Rb_tree::_M_copy`.
// Layout: node = STLport _Rb_tree_node_base (16 bytes) + pair<const unsigned __int64, CThingPosAndOrientation> (0x20);
//   Ego_r.pdb CScriptedMapBrush::CThingPosAndOrientation = {C3DVector Position +0, C3DAngles FacingAngles +0xC}.
// Shape: STLport 4.x verbatim (the 0x008259C0 set twin); VC7.1 keeps the first `_M_clone_node(x)` as a call and inlines
//   the one in the loop by itself. The pair's memberwise copy moves the key and the Position (C3DVector's memberwise
//   user copy ctor: direct [ecx+N] stores) and copies the trivially copyable FacingAngles through its own address
//   (`lea ; add ecx,14h`).
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
namespace _STL {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};
}
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace _STL {
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
    _Link_type _M_create_node(const _Value& x)
    {
        _Link_type tmp = this->_M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        _S_color(tmp) = _S_color(x);
        _S_left(tmp) = 0;
        _S_right(tmp) = 0;
        return tmp;
    }
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

template class _STL::_Rb_tree<unsigned __int64, _STL::pair<const unsigned __int64, CThingPosAndOrientation>, _STL::less<unsigned __int64> >;
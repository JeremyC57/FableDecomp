// _Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>& v, _Rb_tree_node_base* w) -- STLport node insert for the brush's thing-movement map: create the node under y, keep leftmost/rightmost, rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KVCThingPosAndOrientation@CScriptedMapBrush@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082F7B0, 163 bytes (+13 int3), thiscall `ret 14h` (hidden iterator, x, y, v, w).
// Identity: the 0x30-byte node (pair<const u64, CThingPosAndOrientation> at +0x10) is CScriptedMapBrush::ThingsToMove
//   (its tree mallocs a 0x30 header, 0x0082E600); the sibling set<u64> instantiation is 0x0082F150. The catalog label
//   "RBTree_Insert" is descriptive.
// Callees: _Rb_tree::_M_create_node(const value_type&) 0x00824FB0 (landed as CRetailTree_00824fb0::BuyNode: malloc 0x30
//   and the pair's memberwise copy), _Rb_global<bool>::_Rebalance(z, root&) 0x0042971D (__cdecl).
// Shape: STLport 4.x `_M_insert` verbatim, as 0x0082F150, with the node factory a call: VC7.1 inlines the u64 set's
//   two-dword copy but not this 32-byte pair copy (a same-TU noinline body here; only _M_insert is graded).
class C3DVector {                                       // user memberwise copy constructor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C3DAngles {                                       // trivially copyable
public:
    float X, Y, Z;
};
class CScriptedMapBrush {
public:
    class CThingPosAndOrientation {                     // Ego_r.pdb, 24 bytes
    public:
        C3DVector Position;                             // +0x00
        C3DAngles FacingAngles;                         // +0x0c
    };
};
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& x) const { return x.first; } };
template <class A, class B> struct pair {
    typedef A first_type;
    typedef B second_type;
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
};

template <class _Value, class _Node, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr x) { return x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr x) { return x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr x) { return x->_M_parent; }
    static const _Key& _S_key(_Base_ptr x) { return _KeyOfValue()(((_Link_type)x)->_M_value_field); }
protected:
    __declspec(noinline) _Link_type _M_create_node(const _Value& x)   // 0x00824FB0 for the movement map
    {
        _Link_type tmp = this->_M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
private:
    iterator _M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const _Value& v, _Rb_tree_node_base* w = 0);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_,
                                                                const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}

template class std::_Rb_tree<unsigned __int64, std::pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>,
                              std::_Select1st<std::pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> >,
                              std::less<unsigned __int64> >;
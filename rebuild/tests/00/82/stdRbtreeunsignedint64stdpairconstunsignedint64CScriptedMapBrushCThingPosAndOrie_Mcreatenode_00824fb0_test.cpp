// Behaviour test for 0x00824FB0 _Rb_tree<u64, pair<const u64, CScriptedMapBrush::CThingPosAndOrientation> >::_M_create_node:
// links the reconstructed object (malloc is the CRT's); the node is 48 bytes and holds a copy of the value at +0x10.
#include <cstdio>
#include <cstdlib>
int main();
class C3DVector {                                       // user memberwise copy constructor
public:
    float X, Y, Z;
    C3DVector() { }
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
    friend int ::main();
protected:
    _Link_type _M_create_node(const _Value& x);   // the source object's instantiation
};
}


typedef std::pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> CThingMoveValue;
typedef std::_Rb_tree<unsigned __int64, CThingMoveValue, std::_Select1st<CThingMoveValue>, std::less<unsigned __int64> > CThingMoveTree;
int main()
{
    CScriptedMapBrush::CThingPosAndOrientation po;
    po.Position.X = 1.0f; po.Position.Y = 2.0f; po.Position.Z = 3.0f;
    po.FacingAngles.X = 4.0f; po.FacingAngles.Y = 5.0f; po.FacingAngles.Z = 6.0f;
    CThingMoveValue v(0x123456789ABCDEF0ULL, po);
    CThingMoveTree* t = 0;              // the member reads no tree state (allocate is stateless)
    CThingMoveTree tree;
    CThingMoveTree::_Link_type n = tree._M_create_node(v);
    bool ok = n != 0 && (char*)&n->_M_value_field - (char*)n == 0x10 && sizeof(*n) == 0x30
        && n->_M_value_field.first == 0x123456789ABCDEF0ULL
        && n->_M_value_field.second.Position.Y == 2.0f && n->_M_value_field.second.FacingAngles.Z == 6.0f;
    (void)t;
    std::free(n);
    if (!ok) { std::printf("00824fb0_TEST FAIL\n"); return 1; }
    std::printf("00824fb0_TEST PASS\n");
    return 0;
}

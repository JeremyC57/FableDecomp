// std::_Rb_tree<ETableSprites,pair<const ETableSprites,long>,_Select1st<..>,less<ETableSprites>,allocator<..> >::_M_copy(_Rb_tree_node* x, _Rb_tree_node* p) -- STLport structural tree copy for the UI def's sprite map: clone x under parent p, recurse down every right subtree and iterate down the left spine
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00631020, 183 bytes (+ int3 padding), `ret 8` (thiscall: x, p; returns the
// cloned top).
// Identity: callers are itself (0x0063106C, 0x006310BD) and the tree operator= 0x00631220 (landed as the generic
// CRetailTree_00631020::operator=), which the CUIDef copy-assignment 0x00631720 runs on +0x64 = PDB CUIDef::Sprites,
// a map<NUISystem::ETableSprites, long> (re-based -8), and the 0x006313C0 wrapper reaches. Every tree with an 8-byte
// POD pair value folds into this COMDAT (map<unsigned long, unsigned long> ActionMapAliases has the same body). The
// catalog label "TreeNode_CloneWithInt" is descriptive.
// Callee: malloc @ 0x00BFEA0E (node allocate(1) = malloc(0x18)).
// Node: STLport _Rb_tree_node_base {bool _M_color +0; _M_parent +4; _M_left +8; _M_right +0xC} + the pair at +0x10.
// Shape: STLport 4.x `_M_copy` with `_M_clone_node` / `_M_create_node` inlined at both sites (the same source as the
// landed set<long> instance 0x005114F0): the null-tested placement copy of the pair (memberwise STLport pair copy
// constructor, two dword moves), the colour copied, both children zeroed, the top's parent stored after the clone; the
// left-spine loop links `p->_M_left = y ; y->_M_parent = p`. Exceptions are off (no unwind). VC parks `this` in the
// `push ecx` frame word because ebx is reused for `p`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

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

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
    pair(const pair& p) : first(p.first), second(p.second) { }
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

template <class Pair> struct _Select1st { const typename Pair::first_type& operator()(const Pair& x) const { return x.first; } };
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

    _Link_type _M_create_node(const Value& x)
    {
        _Link_type tmp = _M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
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

}

namespace NUISystem {
enum ETableSprites { TABLE_SPRITE_0 };
}

template std::_Rb_tree<NUISystem::ETableSprites, std::pair<const NUISystem::ETableSprites, long>,
                       std::_Select1st<std::pair<const NUISystem::ETableSprites, long> >,
                       std::less<NUISystem::ETableSprites>,
                       std::allocator<std::pair<const NUISystem::ETableSprites, long> > >;
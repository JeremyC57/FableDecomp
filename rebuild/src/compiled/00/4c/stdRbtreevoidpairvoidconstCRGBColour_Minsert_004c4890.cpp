// std::_Rb_tree<void*, pair<void* const, CRGBColour>, ...>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport: link a new tint-colour node left or right of y, fix the header links, rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@PAXU?$pair@QAXVCRGBColour@@@std@@U?$_Select1st@U?$pair@QAXVCRGBColour@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCRGBColour@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@QAXVCRGBColour@@@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@QAXVCRGBColour@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C4890, length 208 bytes (194 + int3 padding), `ret 14h` (hidden iterator result, x, y, v, w).
// Identity: the tree behind CTCGraphicAppearance::TintColours (+0x18, map<void*, CRGBColour>): the landed SetTintColour
// 0x004C00B0 calls its operator[] 0x004C5890, which calls insert_unique(hint) 0x004C4D50, which calls this row (and
// insert_unique(v) 0x004C4960 does too). The catalog "ListNode_InsertCombatAnimation" is a propagated label.
// Callees: malloc @ 0x00BFEA0E, _Rb_tree::_M_create_node(const value_type&) @ 0x004C41D0 (thiscall, the right-hand
// site only), _Rb_global<bool>::_Rebalance(node, root&) @ 0x0042971D (__cdecl).
// Shape: STLport 4.x _M_insert. VC7.1 inlined _M_create_node at the left-hand site and called the out-of-line copy at
// the right-hand site (the per-function inline budget; GOTCHAS "Retail inlines only one of two push_backs"), so the left
// site uses a __forceinline copy of the node build and the right one the declared member. Keys compare unsigned (`jb`).
// The 4-byte CRGBColour (Ego_r.pdb: union of B,G,R,A bytes) copies through Set(R, G, B, A) (as landed 0x00B1FE80): a
// memberwise copy ctor gives a different byte order and register choice.
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CRGBColour {                                           // Ego_r.pdb, 4 bytes
public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) {}
};

struct _Rb_tree_node_base {
    bool                _M_color;                            // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0c
};

template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                        // +0x10
};

template <class B> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};

template <class T> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
};

template <class T> class allocator {};
template <class T> struct less {};
template <class P> struct _Select1st {};

template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    typedef _Rb_tree_iterator<V> iterator;
    struct : allocator<V> { _Rb_tree_node_base* _M_data; } _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    C            _M_key_compare;                                        // +0x08

protected:
    _Link_type _M_create_node(const V& x);                              // 0x004C41D0
public:
    __forceinline _Link_type _M_create_node_inline(const V& x)
    {
        _Link_type tmp = static_cast<_Link_type>(malloc(sizeof(_Rb_tree_node<V>)));
        new (&tmp->_M_value_field) V(x);
        return tmp;
    }
    _Rb_tree_node_base*& _M_root() const { return _M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header._M_data->_M_right; }
    static const K& _S_key(_Rb_tree_node_base* x) { return static_cast<_Link_type>(x)->_M_value_field.first; }

private:
    iterator _M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const V& v, _Rb_tree_node_base* w);
};

template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::iterator
_Rb_tree<K, V, KoV, C, A>::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const V& v, _Rb_tree_node_base* w_)
{
    _Link_type w = static_cast<_Link_type>(w_);
    _Link_type x = static_cast<_Link_type>(x_);
    _Link_type y = static_cast<_Link_type>(y_);
    _Link_type z;

    if (y == _M_header._M_data || (w == 0 && (x != 0 || v.first < _S_key(y)))) {
        z = _M_create_node_inline(v);
        y->_M_left = z;
        if (y == _M_header._M_data) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(z, _M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

template class _Rb_tree<void*, pair<void* const, CRGBColour>, _Select1st<pair<void* const, CRGBColour> >, less<void*>,
                        allocator<pair<void* const, CRGBColour> > >;
}
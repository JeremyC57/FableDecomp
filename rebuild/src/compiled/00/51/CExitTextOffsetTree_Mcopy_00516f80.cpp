// CExitTextOffsetTree::_M_copy(_Link_type x, _Link_type p) -- STLport _Rb_tree::_M_copy of map<CCharString, C2DVector>: structurally clone the subtree at x under parent p (right subtrees by recursion, the left spine by iteration)
// Leaf: ?_M_copy@CExitTextOffsetTree@@
//
// Retail 0x00516F80, 224 bytes, `ret 8` (thiscall; `this` is kept in the `push ecx` slot for the recursive calls).
// Callees: 0x00BFEA0E malloc (0x1c-byte nodes), 0x0099EC30 CCharString::CCharString(const CCharString&), itself.
// Callers: 0x00518E80 (the map copy constructor, called from CRegion::CRegion(const CRegion&) 0x0051A900) and 0x00517370
//   (the map's operator=; the landed source models it as CRetailTree_00516f80).
// Element: CRegion::MiniMapRegionExitTextOffsets (Ego_r.pdb map<CCharString, C2DVector>): node {color, parent, left,
//   right, key CCharString +0x10, C2DVector +0x14} = 0x1c bytes; the class name is descriptive (a non-template stand-in
//   for the STLport _Rb_tree instance).
// Shape: STLport 4.x verbatim: `top = _M_clone_node(x); top->_M_parent = p; if (x->_M_right) top->_M_right =
//   _M_copy(_S_right(x), top); p = top; x = _S_left(x); while (x) { y = _M_clone_node(x); p->_M_left = y; y->_M_parent =
//   p; if (x->_M_right) y->_M_right = _M_copy(_S_right(x), y); p = y; x = _S_left(x); } return top;` with
//   _M_clone_node / _M_create_node inline (the placement-new null test on node+0x10; no /EHsc, so no _STLP_UNWIND).
// Label: catalog "CCharStringTreeNode_DeepClone_Extended" (descriptive); the STLport name is _Rb_tree::_M_copy.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* p) throw() { return p; }

class CCharString {
public:
    const char* PString;
    CCharString(const CCharString& other);     // 0x0099EC30
};
class C2DVector {                              // Ego_r.pdb: float X, Y
public:
    float X;
    float Y;
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
}

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CExitTextOffsetTree {                    // STLport _Rb_tree<CCharString, pair<const CCharString, C2DVector>, ...>
public:
    typedef std::pair<const CCharString, C2DVector> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Node* _Link_type;
    _Node*        _M_header;                   // +0x0
    unsigned long _M_node_count;               // +0x4
    unsigned long _M_key_compare;              // +0x8 (empty less<CCharString>, padded)

    static _Link_type _S_left(_Link_type x) { return (_Link_type)x->_M_left; }
    static _Link_type _S_right(_Link_type x) { return (_Link_type)x->_M_right; }
    static _Node* allocate(unsigned int n) { return (_Node*)malloc(n * sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& x)
    {
        _Link_type tmp = allocate(1);
        std::_Construct(&tmp->_M_value_field, x);
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
CExitTextOffsetTree::_Link_type CExitTextOffsetTree::_M_copy(_Link_type x, _Link_type p)
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
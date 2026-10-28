// std::_Rb_tree<void*, pair<void* const, CRGBColour>, ...>::_M_create_node(const value_type&) -- STLport: allocate a tint-colour map node and copy-construct the pair into it
// Leaf: ?_M_create_node@?$_Rb_tree@PAXU?$pair@QAXVCRGBColour@@@std@@U?$_Select1st@U?$pair@QAXVCRGBColour@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCRGBColour@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@QAXVCRGBColour@@@std@@@2@ABU?$pair@QAXVCRGBColour@@@2@@Z
// Retail: Fable.exe @ 0x004C41D0, 68 bytes (16-aligned /O2 entry), thiscall `ret 4` (this unused), returns the node.
// Identity: the out-of-line _M_create_node of the tree behind CTCGraphicAppearance::TintColours (map<void*, CRGBColour>):
// its only caller is that tree's _M_insert 0x004C4890 (landed; it calls this at its right-hand site and inlines the
// left-hand copy). The catalog label CCreatureAction_PlayCombatAnimation::Clone is propagated across a byte-similar body.
// Callee: malloc 0x00BFEA0E (the 0x18-byte node: _Rb_tree_node_base 0x10 + pair<void* const, CRGBColour> 8).
// Shape: STLport 4.6 `_Link_type __tmp = _M_header.allocate(1); _Construct(&__tmp->_M_value_field, __x);` -- placement
// new keeps its null test; the pair copy copies the key, then CRGBColour copies through Set(R, G, B, A) (as landed
// 0x00B1FE80 / 0x004C4890), which spills one channel byte to the stack because ebx is the only free byte register.
// Model: the landed _M_insert 0x004C4890's tree, with STLport's allocator class-keyed (`V`, as the PDB name sets spell
// every allocator); the landed _M_insert declares it `struct` (see NOTES.md "Review at landing").
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
template <class T> class allocator { public:
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> struct less {};
template <class P> struct _Select1st {};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class T1, class T2>
inline void _Construct(T1* p, const T2& val) { new (p) T1(val); }

template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>  _Node;
    typedef _Rb_tree_node<V>* _Link_type;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                      // +0x04
    C            _M_key_compare;                                     // +0x08
protected:
    _Link_type _M_create_node(const V& x);
};
template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::_Link_type _Rb_tree<K, V, KoV, C, A>::_M_create_node(const V& x)
{
    _Link_type tmp = this->_M_header.allocate(1);
    _Construct(&tmp->_M_value_field, x);
    return tmp;
}
template class _Rb_tree<void*, pair<void* const, CRGBColour>, _Select1st<pair<void* const, CRGBColour> >, less<void*>, allocator<pair<void* const, CRGBColour> > >;
}
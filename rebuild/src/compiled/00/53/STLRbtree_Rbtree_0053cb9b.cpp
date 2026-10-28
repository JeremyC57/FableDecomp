// std::_Rb_tree::_Rb_tree(const _Rb_tree& x) -- copy constructor of the map<unsigned long, NUISystem::CUIState> red-black tree: allocate the header node, then either mark the tree empty or clone the other tree's nodes and set leftmost/rightmost, finally take the node count
// Leaf: ??0?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CB9B, length 116 bytes, `ret 4`.
// Callees: malloc @ 0x00BFEA0E (the 0x38-byte header node: 0x10 node base + the 0x28-byte pair<const unsigned long,
//   CUIState>; `__cdecl`, `pop ecx`), _Rb_tree::_M_copy @ 0x0042CB90 (landed; thiscall, ret 8),
//   _Rb_tree_node_base::_S_minimum / _S_maximum @ 0x0041C7E0 / 0x0041C800 (landed; out of line here, `__cdecl`, the two
//   argument pops deferred to `pop ecx ; pop ecx`).
// Label: catalog "std::list_construct_from_list" is descriptive and wrong -- the body is the tree copy constructor
//   (same tree as the landed operator= 0x0042CE7A and _M_copy 0x0042CB90, which names 0x0053CBDC as a caller).
// Shape: STLport 4.x `_Rb_tree(const _Rb_tree& __x) : _Rb_tree_base(__x.get_allocator()), _M_node_count(0) { if
//   (__x._M_root() == 0) _M_empty_initialize(); else { color(header) = red; root = _M_copy(__x._M_root(), header);
//   leftmost = _S_minimum(root); rightmost = _S_maximum(root); } _M_node_count = __x._M_node_count; }` in a size TU
//   (`optimize("s", on)`, as the landed operator=). The base constructor is `__forceinline` and zeroes the header word
//   before the allocation (as the landed 0x004F9960 relative); the red colour store is common to both arms and hoisted
//   above the branch; the root/leftmost/rightmost accessors return references, so the header is re-read before each call.
#pragma optimize("s", on)
namespace NUISystem {
class CUIState {                                         // 0x24 bytes (Ego_r.pdb 36)
public:
    unsigned long StateChangeFlag;
    long          StateChangeType;
    float         Position[2];
    float         Zoom[2];
    unsigned long Colour;
    float         UpdateTime;
    bool          LinearChange;
    CUIState(const CUIState& s);                         // 0x00429E29 (out of line)
};
}
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
    static _Rb_tree_node_base* _S_minimum(_Rb_tree_node_base* x);   // 0x0041C7E0 (out of line, __cdecl)
    static _Rb_tree_node_base* _S_maximum(_Rb_tree_node_base* x);   // 0x0041C800 (out of line, __cdecl)
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    _Rb_tree_node_base* _M_header;                       // +0x00 (_M_header._M_data)
    __forceinline _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(sizeof(_Rb_tree_node<V>)); }
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    unsigned int        _M_node_count;                   // +0x04 (the empty comparator follows)
    protected:
    _Rb_tree_node_base*& _M_root() const { return this->_M_header->_M_parent; }
    public:
    protected:
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header->_M_left; }
    public:
    protected:
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header->_M_right; }
    public:
    static bool& _S_color(_Rb_tree_node_base* x) { return x->_M_color; }
    static _Link_type _S_minimum(_Link_type x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(x); }
    static _Link_type _S_maximum(_Link_type x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(x); }
private:
    _Link_type _M_copy(_Link_type x, _Link_type p);      // 0x0042CB90
public:
    _Rb_tree(const _Rb_tree& x);
};
template <class K, class V, class KoV, class C, class A>
_Rb_tree<K, V, KoV, C, A>::_Rb_tree(const _Rb_tree<K, V, KoV, C, A>& x)
    : _Rb_tree_base<V, A>(), _M_node_count(0)
{
    if (x._M_root() == 0)
    {
        _S_color(this->_M_header) = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header;
        _M_rightmost() = this->_M_header;
    }
    else
    {
        _S_color(this->_M_header) = false;
        _M_root() = _M_copy((_Link_type)x._M_root(), (_Link_type)this->_M_header);
        _M_leftmost() = _S_minimum((_Link_type)_M_root());
        _M_rightmost() = _S_maximum((_Link_type)_M_root());
    }
    _M_node_count = x._M_node_count;
}
}

typedef std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > > TUIStateTree;
template TUIStateTree::_Rb_tree(const TUIStateTree& x);
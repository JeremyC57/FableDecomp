// std::_Rb_tree::_Rb_tree(const _Compare& comp, const allocator_type& a) -- STLport tree constructor for the combat manager's combatant map (map<unsigned __int64, CCombatantInfo>): allocate the 0x88-byte header node and make it an empty red sentinel
// Leaf: ??0?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F1AE0, 59 bytes (64 with int3 padding), thiscall, `ret 8` (the compare and the
//   allocator references), returns this.
// Identity: the 0x88-byte node is { colour, parent, left, right } + pair<const unsigned __int64, CCombatantInfo> (8-byte key
//   at +0x10, the 0x70-byte CCombatantInfo at +0x18) -- the CCombatManager::Combatants map whose inline construction is
//   0x006ED3F0 (this chunk). No call or data reference reaches it (an out-of-line copy kept in the image), so the
//   `_Rb_tree(comp, a)` vs `map(comp, a)` spelling is not decided by the bytes; the sibling 0x006F1B20 is the same
//   constructor for the 0x18-byte RegisteredCombatSequences node. The catalog's "TreeHeader_InitSentinel_0x88" is
//   descriptive.
// Callees: malloc @ 0x00BFEA0E (STLport's allocator<_Node>::allocate(1)).
// Shape: STLport 4.x `_Rb_tree(const _Compare& __comp, const allocator_type& __a) : _Rb_tree_base(__a),
//   _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }`: the header proxy is zeroed, then assigned
//   the allocation; the count is zeroed; the header is coloured red with no parent and points left and right at itself.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> struct less { };
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    public:
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };
}

struct CCombatantInfo {                                     // 0x70 bytes in retail
    unsigned long _data[0x70 / 4];
};

typedef std::pair<const unsigned __int64, CCombatantInfo> SCombatantEntry;
template std::_Rb_tree<unsigned __int64, SCombatantEntry, std::_Select1st<SCombatantEntry>, std::less<unsigned __int64>,
                       std::allocator<SCombatantEntry> >::_Rb_tree(const std::less<unsigned __int64>&,
                                                                   const std::allocator<SCombatantEntry>&);
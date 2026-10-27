// std::map<unsigned long, CCodeSectionManager::CSectionDesc>::erase(iterator position) -- STLport erase of one node: unlink and rebalance, destroy the (key, CCodeSectionManager::CSectionDesc) value, free the node, --count
// Leaf: ?erase@?$map@KUCSectionDesc@CCodeSectionManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F0E80, 78 bytes, thiscall, `ret 4` (the iterator by value).
// Identity (probable): only caller the landed CCodeSectionManager::RemoveSection 0x009F03B0 (it calls this erase on the index map after its inline find); Ego_r.pdb CCodeSectionManager::SectionDesc is map<unsigned long, CCodeSectionManager::CSectionDesc>, whose 16-byte value destroys LongName (+4) then ShortName (+0); container proven, the map-vs-tree wrapper level probable.
// Callees: _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (cdecl), CCharString::~CCharString 0x0099EAE0 (LongName, then ShortName), free 0x00BFEA14.
// Shape (family K025, representative 0x007921C0): STLport 4.6 `_Link_type __y = _Rebalance_for_erase(__position._M_node,
//   _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right); _Destroy(&__y->_M_value_field);
//   _M_header.deallocate(__y, 1); --_M_node_count;` -- the value's implicit destructor runs its members in reverse; the
//   malloc allocator's deallocate null-tests the node. This row is map::erase(iterator) = `_M_t.erase(__position);` with the tree's erase inlined.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
class CCharString {
public:
    void* PData;                                         // +0x00
    ~CCharString();                                      // 0x0099EAE0
};
class CCodeSectionManager {
public:
    struct CSectionDesc {                                // Ego_r.pdb, 16 bytes (class key U)
        CCharString ShortName;                           // +0x00
        CCharString LongName;                            // +0x04
        bool Loaded;                                     // +0x08
        long Size;                                       // +0x0C
    };
};
typedef unsigned long _Key;
typedef CCodeSectionManager::CSectionDesc _Mapped;

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost,
                                                            _Rb_tree_node_base*& __rightmost);              // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator { };
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
protected:
    unsigned int _M_node_count;
    _Compare _M_key_compare;
public:
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                           this->_M_header._M_data->_M_parent,
                                                                           this->_M_header._M_data->_M_left,
                                                                           this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
private:
    _Rep_type _M_t;
public:
    void erase(iterator __position) { _M_t.erase(__position); }
};
typedef pair<const _Key, _Mapped> _Val;
template void map<_Key, _Mapped>::erase(map<_Key, _Mapped>::iterator);
} // namespace std
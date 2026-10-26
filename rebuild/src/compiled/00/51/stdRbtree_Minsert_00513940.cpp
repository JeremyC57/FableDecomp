// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- (set<unsigned long>) STLport 4.6 node insert: create a node for v, link it left or right of y, fix root / leftmost / rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@QAE?AU?$_Rb_tree_iterator@KU?$_Nonconst_traits@K@std@@@2@PAU_Rb_tree_node_base@2@0ABK0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513940, 179 bytes (+ int3 padding to 192), thiscall `ret 14h` (the hidden
//   iterator result pointer first, then x, y, &v, w).
// Identity (proven): set<unsigned long>; evidence: its callers are the set's insert_unique 0x00514C10 and a second inserting caller 0x00AD28E0 in the quadric-optimiser TU (Ego_r.pdb uses insert_iterator<set<unsigned long> > there); 0x00514C10 is reached from many TUs: CCRCSymbolMap::ParseEnum (via 0x00A385A0; Ego_r.pdb CCRCSymbolMap::NamesUsed is set<unsigned long>), the quadric optimiser (0x00AC8360 / 0x00AC8840 / COptimisedPrimitive::Initialise; Ego_r.pdb CVertexInfo::FaceList and CIndexList::Indices are set<unsigned long>), CDataBank::GetAllTextEntriesForNarrator, CTCInventoryItem::OnDie and 0x004FE7F0 -- a COMDAT shared by unrelated users, the common set<unsigned long>
// Label: the manifest's "RedBlackTree_InsertNode" is descriptive (family H075: six set<4-byte key> node inserts; retail has no identical-code
//   folding, so each address is one instantiation); this is STLport 4.6 `_Rb_tree::_M_insert` (`_tree.c`), as H011.
// Callees: malloc 0x00BFEA0E (inline allocate, 0x14-byte node), _Rb_global<bool>::_Rebalance 0x0042971D (__cdecl).
// Model: H011's (the landed _M_insert 0x006BD140, STLport 4.6 verbatim) with a set's value: _M_create_node's placement copy
//   is the null-checked dword store; keys compare unsigned (`jb`).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }


namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);                         // 0x0042971D
};

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> _Self;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Rb_tree(const _Self& __x);                                    // out of line where used (not here)
    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v,
                                                                 _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        __y->_M_left = __z;
        if (__y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = __z;
            this->_M_header._M_data->_M_right = __z;
        }
        else if (__y == this->_M_header._M_data->_M_left)
            this->_M_header._M_data->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == this->_M_header._M_data->_M_right)
            this->_M_header._M_data->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}
}   // namespace std

typedef std::_Rb_tree<unsigned long, unsigned long, std::_Identity<unsigned long >, std::less<unsigned long >, std::allocator<unsigned long > > TTree;
template TTree::iterator TTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, unsigned long const&,
                                          std::_Rb_tree_node_base*);
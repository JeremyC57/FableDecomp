// std::_Rb_tree<CCharString, pair<const CCharString, vector<CCharString> >, ...>::erase(iterator position) -- STLport map node erase: unlink and rebalance, destroy the (key, string vector) value, free the node, drop the count
// Leaf: ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@std@@@std@@@3@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@std@@@std@@@2@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AE1E0, 120 bytes, thiscall, `ret 4` (the iterator by value).
// Identity: the manifest's "std::map::_Erase_node" is descriptive. The only caller is this tree's erase(first, last)
//   0x008AE380 (this chunk), used by the CTCAIScratchpad gossip maps (map<CCharString, vector<CCharString> >, the
//   statics at 0x013BAE44 / 0x013BAE50 / 0x013BAE5C; landed operator[] 0x007AEBE0, _M_erase 0x007AE0E0). The node is
//   {colour, parent, left, right, key +0x10, vector +0x14 (start / finish / end of storage)}.
// Callees: _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (__cdecl; the manifest's
//   "CMemoryAllocatorVariableSize::Erase" is propagated), ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Shape: STLport 4.6 `y = _Rebalance_for_erase(pos._M_node, root, leftmost, rightmost); _Destroy(&y->_M_value_field);
//   _M_header.deallocate(y, 1); --_M_node_count;` with the pair's implicit destructor inline: ~vector (_Destroy over
//   [start, finish) with finish/start in ebx/esi, then the null-tested free of the re-read start), then the key's
//   ~CCharString; the node free is null-tested. Four callee-saved registers are busy, so `this` lives in the
//   `push ecx` slot.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;   // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
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

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root,
                                                  _Base_ptr& __leftmost, _Base_ptr& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;

    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08

    void erase(iterator __position);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __position)
{
    _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                        this->_M_header._M_data->_M_parent,
                                                                        this->_M_header._M_data->_M_left,
                                                                        this->_M_header._M_data->_M_right);
    _Destroy(&__y->_M_value_field);
    this->_M_header.deallocate(__y, 1);
    --_M_node_count;
}

typedef pair<const CCharString, vector<CCharString> > _GossipValue;
template void _Rb_tree<CCharString, _GossipValue, _Select1st<_GossipValue>, less<CCharString> >::erase(iterator);

} // namespace std
// std::map::operator[](const unsigned long& k) -- map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo>::operator[]: find a bone group's block-assignment record, inserting a default one (Count 0, BlockIndex -1) when absent
// Leaf: ??A?$map@KVCGroupInfo@CVertexGroupBlockOptimiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADBFF0, 0x78 bytes, thiscall, `ret 4`, returns &(*i).second.
// Identity: node key at +0x10 compared unsigned (`jb`), mapped value at +0x14; the callee 0x00ADBA40 is the landed
//   _Rb_tree::insert_unique(iterator, const value_type&) of pair<unsigned, CVertexGroupBlockOptimiser::CGroupInfo>
//   (CGroupInfo() = {Count 0, BlockIndex -1}); Ego_r.pdb has map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo>
//   ::operator[]. Catalog "BinTree_SearchOrInsert" is descriptive.
// Shape: STLport `i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k, _Tp()));
//   return (*i).second;`. Recipe (retry pass 3): a same-TU noinline insert_unique stand-in that READS v but never keeps
//   its address schedules the late pair stores after the hidden-result push as retail (cf. landed 0x009B7BC0).
class CVertexGroupBlockOptimiser {
public:
    class CGroupInfo {                       // Ego_r.pdb, 8 bytes
    public:
        unsigned long Count;                 // +0x0
        long          BlockIndex;            // +0x4
        CGroupInfo() : Count(0), BlockIndex(-1) { }
    };
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_root() const { return _M_header->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Rb_tree_node<_Value>*)__x)->_M_value_field.first; }
    _Compare key_comp() const { return _M_key_compare; }
    iterator end() { return iterator(_M_header); }
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const
    {
        _Rb_tree_node_base* __y = _M_header;
        _Rb_tree_node_base* __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    iterator insert_unique(iterator __position, const _Value& __v);   // 0x00ADBA40
private:
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    _Compare            _M_key_compare;                 // +0x08
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x00ADBA40 (reads v, never keeps its address).
template <class _Key, class _Value, class _Compare>
__declspec(noinline) typename _Rb_tree<_Key, _Value, _Compare>::iterator
_Rb_tree<_Key, _Value, _Compare>::insert_unique(iterator __position, const _Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return __position;
}
#pragma auto_inline(on)

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    iterator end() { return _M_t.end(); }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k);
private:
    _Rep_type _M_t;
};

template <class _Key, class _Tp, class _Compare> _Tp& map<_Key, _Tp, _Compare>::operator[](const _Key& __k)
{
    iterator __i = lower_bound(__k);
    if (__i == end() || key_comp()(__k, (*__i).first))
        __i = insert(__i, value_type(__k, _Tp()));
    return (*__i).second;
}

template class map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo>;
}